# Fallout 2 Community Edition port for PS Vita With TrueType Font Support

## TrueType与中文支持

归功于https://github.com/sonilyan/fallout2-ce 项目中对TrueType的支持Commit，我们拥有了可以在psv运行的中文版辐射2

https://github.com/Northfear/fallout2-ce-vita/commit/50431b1e27f200e066c414136122b5a8d9c90a1f

此后版本发布同步上游代码，包括崩溃等问题同步优化.

## Install
Download fallout2-ce.vpk file and install it to your PS Vita.

Copy ```master.dat```, ```critter.dat```, ```patch000.dat```, ```data``` and ```sound``` folders from the installed Fallout 2 game folder into to ```ux0:data/fallout2/```. Copy ```fallout2.cfg``` too, if you're using non-english Fallout 2 version (or make sure that ```language``` setting is properly set in it (```language=german```, ```language=french```, etc)).

[FdFix](https://github.com/TheOfficialFloW/FdFix) plugin is required for proper suspend/resume support.

## Building

### Prerequisites
- VitaSDK
- SDL2

### Build
The lastest vitasdk seems to break the build,so we need to use old version in docker:)
```
docker  run -v "$PWD:/build/git" -it --rm  gnuton/vitasdk-docker:20240612
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake -DCMAKE_BUILD_TYPE=None
make
```

## Port info

### Controls

- Left analog stick - Cursor movement
- Right analog stick - Map scrolling
- × - Left mouse button
- ○ - Right mouse button
- □ - Skill list/selection
- △ - Inventory
- D-Pad Up - Character screen
- D-Pad Down - Pip-Boy
- D-Pad Left - Start combat
- D-Pad Right - End turn
- L1 - Toggle active item
- R1 (hold) - Cursor movement speedup
- SELECT - Esc
- START - On screen keyboard
- ○ + L1 - Quick save
- ○ + R1 - Quick load

### Touchpad controls

You can change the control mode of front or rear touchpad by editing ```ux0:data/fallout2/f2_res.ini``` and changing ```FRONT_TOUCH_MODE``` and ```REAR_TOUCH_MODE``` parameters.

Following touchpad modes are supported:

```0``` - Touchpad disabled

```1``` - Direct control (front touchpad only)

```2``` - Trackpad control

In trackpad control mode you can use double tap to "click" left mouse button in the current cursor position and triple tap to "click" right mouse button.

### Other

You can change the game resolution by editing ```ux0:data/fallout2/f2_res.ini``` file and modifying ```SCR_WIDTH``` and ```SCR_HEIGHT``` parameters. Recommended resolutions are ```640x480```, ```848x480``` and ```960x544```.

Cursor speed can be changed with ```MOUSE SENSITIVITY``` slider in game preferences.

Fallout 2 CE is still in early stages of development, so all kinds of bugs and crashes are to be expected.

Savegames can get corrupted sometimes, so you should try saving on the different save slots to avoid losing the game progress. Rewriting the "corrupted" savegame slot might break the new savegame too. Corrupted save folder from the ```ux0:data/fallout2/data/SAVEGAME``` needs to be deleted before it's reused again.

Another Fallout 2 CE port for PS Vita (functionally the same except for different controls):
https://github.com/isage/fallout2-ce

Thanks to @isage and aforementioned F2 port for help with figuring out the save game bug.

# Fallout 2 Community Edition

Fallout 2 Community Edition is a fully working re-implementation of Fallout 2, with the same original gameplay, engine bugfixes, and some quality of life improvements, that works (mostly) hassle-free on multiple platforms.

Popular Fallout 2 total conversion mods are partially supported. Original versions of Nevada and Sonora (that do not rely on extended features provided by Sfall) likely work, although there is no complete walkthrough confirmation yet. [Fallout 2 Restoration Project](https://github.com/BGforgeNet/Fallout2_Restoration_Project), [Fallout Et Tu](https://github.com/rotators/Fo1in2) and [Olympus 2207](https://olympus2207.com) are not yet supported. Other mods (particularly Resurrection and Yesterday) are not tested.

There is also [Fallout Community Edition](https://github.com/alexbatalov/fallout1-ce).

## Installation

You must own the game to play. Purchase your copy on [GOG](https://www.gog.com/game/fallout_2) or [Steam](https://store.steampowered.com/app/38410). Download latest [release](https://github.com/alexbatalov/fallout2-ce/releases) or build from source. You can also check latest [debug](https://github.com/alexbatalov/fallout2-ce/actions) build intended for testers.

### Windows

Download and copy `fallout2-ce.exe` to your `Fallout2` folder. It serves as a drop-in replacement for `fallout2.exe`.

### Linux

- Use Windows installation as a base - it contains data assets needed to play. Copy `Fallout2` folder somewhere, for example `/home/john/Desktop/Fallout2`.

- Download and copy `fallout2-ce` to this folder.

- Install [SDL2](https://libsdl.org/download-2.0.php):

```console
$ sudo apt install libsdl2-2.0-0
```

- Run `./fallout2-ce`.

### macOS

> **NOTE**: macOS 10.11 (El Capitan) or higher is required. Runs natively on Intel-based Macs and Apple Silicon.

- Use Windows installation as a base - it contains data assets needed to play. Copy `Fallout2` folder somewhere, for example `/Applications/Fallout2`.

- Alternatively you can use Fallout 2 from Macplay/The Omni Group as a base - you need to extract game assets from the original bundle. Mount CD/DMG, right click `Fallout 2` -> `Show Package Contents`, navigate to `Contents/Resources`. Copy `GameData` folder somewhere, for example `/Applications/Fallout2`.

- Download and copy `fallout2-ce.app` to this folder.

- Run `fallout2-ce.app`.

### Android

> **NOTE**: Fallout 2 was designed with mouse in mind. There are many controls that require precise cursor positioning, which is not possible with fingers. Current control scheme resembles trackpad usage:
- One finger moves mouse cursor around.
- Tap one finger for left mouse click.
- Tap two fingers for right mouse click (switches mouse cursor mode).
- Move two fingers to scroll current view (map view, worldmap view, inventory scrollers).

> **NOTE**: From Android standpoint release and debug builds are different apps. Both apps require their own copy of game assets and have their own savegames. This is intentional. As a gamer just stick with release version and check for updates.

- Use Windows installation as a base - it contains data assets needed to play. Copy `Fallout2` folder to your device, for example to `Downloads`. You need `master.dat`, `critter.dat`, `patch000.dat`, and `data` folder. Watch for file names - keep (or make) them lowercased (see [Configuration](#configuration)).

- Download `fallout2-ce.apk` and copy it to your device. Open it with file explorer, follow instructions (install from unknown source).

- When you run the game for the first time it will immediately present file picker. Select the folder from the first step. Wait until this data is copied. A loading dialog will appear, just wait for about 30 seconds. If you're installing total conversion mod or localized version with a large number of unpacked resources in `data` folder it can take up to 20 minutes. Once copied, the game will start automatically.

### iOS

> **NOTE**: See Android note on controls.

- Download `fallout2-ce.ipa`. Use sideloading applications ([AltStore](https://altstore.io/) or [Sideloadly](https://sideloadly.io/)) to install it to your device. Alternatively you can always build from source with your own signing certificate.

- Run the game once. You'll see error message saying "Couldn't find/load text fonts". This step is needed for iOS to expose the game via File Sharing feature.

- Use Finder (macOS Catalina and later) or iTunes (Windows and macOS Mojave or earlier) to copy `master.dat`, `critter.dat`, `patch000.dat`, and `data` folder to "Fallout 2" app ([how-to](https://support.apple.com/HT210598)). Watch for file names - keep (or make) them lowercased (see [Configuration](#configuration)).

## Configuration

The main configuration file is `fallout2.cfg`. There are several important settings you might need to adjust for your installation. Depending on your Fallout distribution main game assets `master.dat`, `critter.dat`, `patch000.dat`, and `data` folder might be either all lowercased, or all uppercased. You can either update `master_dat`, `critter_dat`, `master_patches` and `critter_patches` settings to match your file names, or rename files to match entries in your `fallout2.cfg`.

The `sound` folder (with `music` folder inside) might be located either in `data` folder, or be in the Fallout folder. Update `music_path1` setting to match your hierarchy, usually it's `data/sound/music/` or `sound/music/`. Make sure it matches your path exactly (so it might be `SOUND/MUSIC/` if you've installed Fallout from CD). Music files themselves (with `ACM` extension) should be all uppercased, regardless of `sound` and `music` folders.

The second configuration file is `f2_res.ini`. Use it to change game window size and enable/disable fullscreen mode.

```ini
[MAIN]
SCR_WIDTH=1280
SCR_HEIGHT=720
WINDOWED=1
```

Recommendations:
- **Desktops**: Use any size you see fit.
- **Tablets**: Set these values to logical resolution of your device, for example iPad Pro 11 is 1668x2388 (pixels), but it's logical resolution is 834x1194 (points).
- **Mobile phones**: Set height to 480, calculate width according to your device screen (aspect) ratio, for example Samsung S21 is 20:9 device, so the width should be 480 * 20 / 9 = 1067.

In time this stuff will receive in-game interface, right now you have to do it manually.

The third configuration file is `ddraw.ini` (part of Sfall). There are dozens of options that adjust or override engine behaviour and gameplay mechanics. This file is intended for modders and advanced users. Currently only a small subset of these settings are actually implemented.

## Contributing

Integrating Sfall goodies is the top priority. Quality of life updates are OK too. Please no large scale refactorings at this time as we need to reconcile changes from Reference Edition, which will make this process slow and error-prone. In any case open up an issue with your suggestion or to notify other people that something is being worked on.

### Intergrating Sfall

There are literally hundreds if not thousands of fixes and features in sfall. I guess not all of them are needed in Community Edition, but for the sake of compatibility with big mods out there, let's integrate them all.

## License

The source code is this repository is available under the [Sustainable Use License](LICENSE.md).
