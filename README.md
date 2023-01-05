# Fallout 2 Community Edition port for PS Vita

## Install
Download fallout2-ce.vpk file and install it to your PS Vita.

Copy ```master.dat```, ```critter.dat```, ```patch000.dat```, ```data``` and ```sound``` folders from the installed Fallout 2 game folder into to ```ux0:data/fallout2/```. Copy ```fallout2.cfg``` too, if you're using non-english Fallout 2 version (or make sure that ```language``` setting is properly set in it (```language=german```, ```language=french```, etc)).

## Building

### Prerequisites
- VitaSDK
- SDL2

### Build
```
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

> **NOTE**: macOS 10.11 (El Capitan) or higher is required. Runs natively on Intel-based Macs and Apple Silicon.

- Use Windows installation as a base - it contains data assets needed to play. Copy `Fallout2` folder somewhere, for example `/Applications/Fallout2`.

- Alternatively you can use Fallout 2 from Macplay/The Omni Group as a base - you need to extract game assets from the original bundle. Mount CD/DMG, right click `Fallout 2` -> `Show Package Contents`, navigate to `Contents/Resources`. Copy `GameData` folder somewhere, for example `/Applications/Fallout2`.

- Download and copy `fallout2-ce.app` to this folder.

- Run `fallout2-ce.app`.

### Android

> **NOTE**: Fallout 2 was designed with mouse in mind. There are many controls that require precise cursor positioning, which is not possible with fingers. When playing on Android you'll use fingers to move mouse cursor, not a character, or a map. Double tap to "click" left mouse button in the current cursor position, triple tap to "click" right mouse button. It might feel awkward at first, but it's super handy - you can play with just a thumb. This is not set in stone and might change in the future.

- Use Windows installation as a base - it contains data assets needed to play. Copy `Fallout2` folder to your device, for example to `Downloads`. You need `master.dat`, `critter.dat`, `patch000.dat`, and `data` folder.

- Download `fallout2-ce.apk` and copy it to your device. Open it with file explorer, follow instructions (install from unknown source).

- When you run the game for the first time it will immediately present file picker. Select the folder from the first step. Wait until this data is copied. A loading dialog will appear, just wait for about 30 seconds. The game will start automatically.

### iOS

> **NOTE**: See Android note on controls.

- Download `fallout2-ce.ipa`. Use sideloading applications ([AltStore](https://altstore.io/) or [Sideloadly](https://sideloadly.io/)) to install it to your device. Alternatively you can always build from source with your own signing certificate.

- Run the game once. You'll see error message saying "Couldn't find/load text fonts". This step is needed for iOS to expose the game via File Sharing feature.

- Use Finder (macOS Catalina and later) or iTunes (Windows and macOS Mojave or earlier) to copy `master.dat`, `critter.dat`, `patch000.dat`, and `data` folder to "Fallout 2" app ([how-to](https://support.apple.com/HT210598)).

## Contributing

Integrating Sfall goodies is the top priority. Quality of life updates are OK too. Please no large scale refactorings at this time as we need to reconcile changes from Reference Edition, which will make this process slow and error-prone. In any case open up an issue with your suggestion or to notify other people that something is being worked on.

### Intergrating Sfall

There are literally hundreds if not thousands of fixes and features in sfall. I guess not all of them are needed in Community Edition, but for the sake of compatibility with big mods out there, let's integrate them all.

## License

The source code is this repository is available under the [Sustainable Use License](LICENSE.md).
