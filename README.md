# Fallout 2 Community Edition port for PS Vita

## Install
Download fallout2-ce.vpk file and install it to your PS Vita.

Copy ```master.dat```, ```critter.dat```, ```patch000.dat```, ```data``` and ```sound``` folders from the installed Fallout 2 game folder into to ```ux0:data/fallout2/```.

## Building

### Prerequisites
- VitaSDK
- SDL2

### Build
```
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake -DCMAKE_BUILD_TYPE=Release
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

### Other

You can change the game resolution by editing ```ux0:data/fallout2/f2_res.ini``` file and modifying ```SCR_WIDTH``` and ```SCR_HEIGHT``` parameters. Recommended resolutions are ```640x480```, ```848x480``` and ```960x544```.

Cursor speed can be changed with ```MOUSE SENSITIVITY``` slider in game preferences.

Fallout 2 CE is still in early stages of development, so all kinds of bugs and crashes are to be expected.

Another Fallout 2 CE port for PS Vita (functionally the same except for different controls):
https://github.com/isage/fallout2-ce

Thanks to @isage and aforementioned F2 port for help with figuring out the save game bug.

# Fallout 2 Community Edition

## Installation

You must own the game to play. Purchase your copy on [GOG](https://www.gog.com/game/fallout_2) or [Steam](https://store.steampowered.com/app/38410). Download latest release or build from source.

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

> **NOTE**: macOS 11 or higher is required.

- Use Windows installation as a base - it contains data assets needed to play. Copy `Fallout2` folder somewhere, for example `/Applications/Fallout2`.

- Alternatively you can use Fallout 2 from Macplay/The Omni Group as a base - you need to extract game assets from the original bundle. Mount CD/DMG, right click `Fallout 2` -> `Show Package Contents`, navigate to `Contents/Resources`. Copy `GameData` folder somewhere, for example `/Applications/Fallout2`.

- Download and copy `fallout2-ce.app` to this folder.

- Run `fallout2-ce.app`.

## Contributing

Integrating Sfall goodies is the top priority. Quality of life updates are OK too. Please no large scale refactorings at this time as we need to reconcile changes from Reference Edition, which will make this process slow and error-prone. In any case open up an issue with your suggestion or to notify other people that something is being worked on.

### Intergrating Sfall

There are literally hundreds if not thousands of fixes and features in sfall. I guess not all of them are needed in Community Edition, but for the sake of compatibility with big mods out there, let's integrate them all.

## Legal & License

See [Fallout 2 Reference Edition](https://github.com/alexbatalov/fallout2-re). Same conditions apply until the source code in this repository is changed significantly.
