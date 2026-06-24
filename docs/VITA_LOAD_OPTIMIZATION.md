# PS Vita Save Load Optimization Notes

This note records the PS Vita save-load optimization work done on the `vita-cn-port` branch in June 2026. It is intended as a technical reference for future performance work.

## Summary

The test case was loading save slot 11 on real PS Vita hardware. The user prioritized smooth gameplay after entering the map over reducing the loading screen at all costs, so art preloading was kept enabled.

Observed progression on the same slow Vita setup:

| Stage | Profiled load time | Notes |
| --- | ---: | --- |
| Initial problematic build | ~126-136s | Severe random-looking I/O stalls. |
| Skip Vita path case-resolution scans | ~40s | Largest win; removed repeated directory walking. |
| Small compressed entry buffering | ~44.6s profiled | Removed zlib tiny-read storm; one run had abnormal map copy time. |
| Preload before light rebuild | ~40.7s profiled | Improved post-load smoothness and lowered light rebuild cost. |
| DBase stream pool | ~29.2s profiled | Removed archive reopen cost. |
| Same build, no profile marker | ~25s perceived | User reported good in-map experience and repeated save/load stability. |

The final tested build kept `ux0:data/Fallout2/profile_load.txt` absent for normal play. With the marker absent, load profiling and the I/O benchmark do not run.

## Profiling Setup

Profiling is enabled only on Vita when this marker exists:

```text
ux0:data/Fallout2/profile_load.txt
```

Output is appended to:

```text
ux0:data/Fallout2/f2ce_load_profile.log
```

Creating `f2ce_load_profile.log` alone does not enable profiling. The marker file also enables a one-time-per-process I/O benchmark, which adds several seconds to the profiled load and should not be used for pure experience tests.

Important profiled areas:

- `src/loadsave.cc`
  - save-slot scan
  - `lsgLoadGameInSlot()` handlers
  - `_PrepLoad()` / `_EndLoad()`
  - `_SlotMap2Game()` erase/copy/map load phases
  - Vita sequential and small-file I/O benchmark
- `src/map.cc`
  - `mapLoad()` phase timings
  - scoped `dfileProfileReset()` / `dfileProfileReport()` around object load and art preload
- `src/object.cc`
  - `objectLoadAllInternal()` phase timings
  - `_obj_preload_art_cache()` object/tile/wall art timing
- `src/dfile.cc`
  - DAT/ZIP entry open/pooled-open/seek/read counts

## Optimization 1: Skip Vita Path Resolution Directory Walks

Commit:

```text
4e9b3692 Optimize Vita save load I/O
```

### Problem

Vita3K trace logs showed repeated directory scans before simple file opens:

```text
Dopen ux0:/data/Fallout2
Dread ux0:/data/Fallout2/...
Dclose
Dopen ux0:/data/Fallout2/data
...
```

The source was `compat_prepare_native_path()` calling `compat_resolve_path()` for every `fopen`, `stat`, `remove`, and `access`. On non-Windows platforms, `compat_resolve_path()` performs case-insensitive path matching by opening each directory level and scanning entries.

On Vita this was catastrophic: map loading performs many small file/archive operations, so repeated path resolution became repeated directory traversal.

### Fix

- `src/platform_compat.cc`
  - Under `__vita__`, skip `compat_resolve_path()` in `compat_prepare_native_path()`.
- `src/file_find.cc`
  - Under `__vita__`, skip the extra `compat_resolve_path(basePath)` in `fileFindFirst()`.

### Result

Representative profile:

```text
load slot 11 finished in 40060 ms
mapLoad objects: 16653 ms
mapLoad art preload: 12173 ms
mapLoad final tile refresh: 135 ms
```

This dropped the user's real-hardware load from ~136s to ~40s with profiling enabled.

## Optimization 2: Object Read and Insert Cleanup

Commit:

```text
4e9b3692 Optimize Vita save load I/O
```

### Fixes

- `src/object.cc`
  - `objectRead()` reads the 18 object header integers in one `fileReadInt32List()` call instead of 18 individual reads.
  - `_obj_insert()` no longer locks art for an unfinished/no-op draw-order comparison. The placeholder did not affect behavior but caused expensive art access during map load.

### Result

These are low-risk supporting optimizations. The major win in this commit was the Vita path-resolution change, but these changes reduce per-object overhead and avoid useless art cache pressure.

## Optimization 3: Fine-Grained Profiling

Uncommitted during investigation, then included with the later performance commit.

### Findings Before Further Optimizing

A detailed profile showed:

```text
OBJECT PROFILE load: top=3184 inventory=145 scripted=84 alloc=24 ms read=23059 ms script=24 ms fix=31 ms insert=20 ms inventory=2241 ms light=24536 ms total=50057 ms
DFILE PROFILE mapLoad objects: opens=1513 open=3011 ms seek=370 ms plain_reads=44 plain_bytes=176 plain=15 ms compressed_reads=2421335 compressed_bytes=2637208 compressed=23412 ms
OBJECT PROFILE preload: fids=3184 object_locks=262/262 object=2978 ms tile_locks=304/304 tile=8087 ms wall_locks=200/200 wall=1769 ms square=0 ms sort=2 ms total=12836 ms
DFILE PROFILE mapLoad art preload: opens=960 open=1900 ms seek=224 ms plain_reads=0 plain_bytes=0 plain=0 ms compressed_reads=38296 compressed_bytes=993893 compressed=3932 ms
```

Interpretation:

- `_obj_insert()` was not a bottleneck (`~20 ms`).
- Art preload was already mostly unique-lock based, not repeatedly loading the same FID in the preload pass.
- The biggest issue was a zlib tiny-read storm: millions of compressed-read calls for only a few megabytes of DAT data.
- DAT archive open/seek was also visible: several seconds were spent repeatedly opening and seeking in `master.dat`/`critter.dat`.

## Optimization 4: Small Compressed Entry Memory Buffering

### Problem

DAT/ZIP compressed entries were read through zlib in very small increments. For the object-load test case this caused more than 2.4 million compressed-read calls.

### Fix

- `src/dfile.cc` / `src/dfile.h`
  - On Vita, compressed entries with uncompressed size up to 512 KiB are fully decompressed into a per-`DFile` memory buffer when opened.
  - Subsequent `dfileRead()`, `dfileReadChar()`, and `dfileSeek()` operate on memory.
  - `dfileClose()` frees the buffer.
  - The cache is per open `DFile`, not a global LRU cache, keeping behavior simple and bounded.
- Profiling overhead was reduced by removing per-compressed-read timing calls while keeping count/byte statistics.

### Result

Representative profile after buffering:

```text
DFILE PROFILE mapLoad objects: opens=1513 open=2837 ms seek=268 ms plain_reads=44 plain_bytes=176 plain=12 ms compressed_reads=1509 compressed_bytes=3114157
DFILE PROFILE mapLoad art preload: opens=960 open=1801 ms seek=171 ms plain_reads=0 plain_bytes=0 plain=0 ms compressed_reads=960 compressed_bytes=1059582
```

Compressed-read calls dropped from millions to roughly one per compressed entry. This validated the strategy.

## Optimization 5: Preload Art Before Light Rebuild

### Problem

`objectLoadAllInternal()` rebuilt map lighting before `_obj_preload_art_cache()` ran in `mapLoad()`. Light rebuild calls `_obj_rebuild_all_light()`, which calls `objectGetRect()`, which calls `artLock(obj->fid)`. Therefore light rebuild was cold-loading lots of art before the explicit preload pass.

### Fix

- `src/object.cc`
  - Call `_obj_preload_art_cache(gMapHeader.flags)` after all objects are read but before `_obj_rebuild_all_light()`.
  - Keep the later `mapLoad()` call in place as a safe no-op. `_obj_preload_art_cache()` frees `gObjectFids`, so the later call returns immediately.

### Result

Representative profile:

```text
OBJECT PROFILE preload: fids=3184 object_locks=262/262 object=7370 ms tile_locks=304/304 tile=7807 ms wall_locks=200/200 wall=5068 ms square=1 ms sort=2 ms total=20248 ms
OBJECT PROFILE load: top=3184 inventory=145 scripted=84 alloc=18 ms read=4439 ms script=23 ms fix=20 ms insert=21 ms inventory=1854 ms light=2495 ms total=29286 ms
DFILE PROFILE mapLoad art preload: opens=0 open=0 ms seek=0 ms plain_reads=0 plain_bytes=0 plain=0 ms compressed_reads=0 compressed_bytes=0
mapLoad art preload: 12 ms
```

Light rebuild dropped from about 9.6s to about 2.5s in the comparable buffered-entry profile. The user also reported that entering the map felt smoother.

Tradeoff: cold preload cost moved into `mapLoad objects`, so total cold load did not improve much by this step alone. Its main value was smoother map entry and avoiding cold art loads during light rebuild.

## Optimization 6: DBase Archive Stream Pool

### Problem

After compressed-entry buffering, repeated archive open/close became a visible cost:

```text
DFILE PROFILE mapLoad objects: opens=2475 open=4908 ms seek=602 ms compressed_reads=2471 compressed_bytes=4177281
```

Each DAT/ZIP entry open still opened `master.dat` or `critter.dat`, sought to the entry, read/decompressed, then closed the archive stream.

### Fix

- `src/dfile.h`
  - `DBase` now has a small idle stream pool: `streamPool[4]` and `streamPoolLength`.
- `src/dfile.cc`
  - `dfileOpenInternal()` gets a stream from the pool first; only opens the archive file when the pool is empty.
  - `dfileClose()` returns the archive stream to the pool instead of closing it when there is room.
  - Small-entry full decompression also returns the stream to the pool immediately after the memory buffer is populated.
  - `dbaseClose()` closes all pooled streams.
  - `DFILE PROFILE` reports `pooled=` in addition to true `opens=`.

### Result

Representative profile:

```text
DFILE PROFILE mapLoad objects: opens=0 pooled=2475 open=0 ms seek=3834 ms plain_reads=44 plain_bytes=176 plain=0 ms compressed_reads=2471 compressed_bytes=4177281
OBJECT PROFILE preload: fids=3184 object_locks=262/262 object=4748 ms tile_locks=304/304 tile=4424 ms wall_locks=200/200 wall=2850 ms square=0 ms sort=2 ms total=12024 ms
OBJECT PROFILE load: top=3184 inventory=145 scripted=84 alloc=25 ms read=3234 ms script=18 ms fix=24 ms insert=20 ms inventory=1237 ms light=1830 ms total=18570 ms
mapLoad objects: 18592 ms
mapLoad art preload: 25 ms
mapLoad total: 19865 ms
load slot 11 finished in 29168 ms
```

The pool eliminated archive open time for this test (`opens=0`, `pooled=2475`). Profiled load dropped to ~29.2s, and pure no-profile user testing was about 25s.

## Final Test Status

User real-hardware test after the stream-pool build:

- Pure perceived save-load time: ~25s.
- In-map experience: good/smooth.
- Multiple save/load cycles: normal.
- No crashes observed.

This met the phase goal.

## Lessons Learned

- On Vita, avoid hidden directory scans in generic path helpers. Case-insensitive path resolution was more expensive than the actual data read in the original slow profile.
- Keep art preload for smoothness. Skipping it shortens load but causes post-load camera/view stutter.
- Fine-grained profiling can distort timing. Count-based profiling was more reliable than per-read timing in hot paths.
- For DAT/ZIP compressed entries, reducing call count can matter more than raw bytes read.
- Reusing archive streams is valuable on Vita because repeated `fopen` of large DAT archives is expensive.
- Moving work earlier can improve experience even when cold-load time is neutral. Preloading before light rebuild reduced cold art loads during lighting and improved perceived map entry smoothness.

## Remaining Optimization Ideas

These are not required for the current phase:

- Reduce remaining DAT `seek` cost (`seek=3834 ms` in the final profiled run), possibly via resource-level cache or entry locality improvements.
- Consider a bounded global resource LRU cache for small DAT entries if memory allows.
- Optimize `SlotMap2Game` save staging only if necessary; current erase/copy usually costs a few seconds and is not the primary bottleneck anymore.
- Lazy scan save slots in the load UI; current 100-slot scan is under 1s after path-resolution fixes.
