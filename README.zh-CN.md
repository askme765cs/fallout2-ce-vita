# Fallout 2 Community Engine / 辐射 2 社区引擎

[English](README.md) | [中文](README.zh-CN.md)

## Fallout 2 CE Vita 中文移植版

本分支是 Fallout 2 Community Engine 的 PS Vita + 中文移植版。它加入了 Vita 构建与 VPK 打包、Vita 按键/触摸输入、基于 TrueType/FreeType 的中文字体渲染、GBK 文本处理，以及 Vita IME 中文输入支持。

### 致谢

- **PS Vita 支持** 基于 [Northfear/fallout2-ce-vita](https://github.com/Northfear/fallout2-ce-vita)。
- **中文 / TrueType 字体支持** 基于 [sonilyan/fallout2-ce](https://github.com/sonilyan/fallout2-ce)。

后续引擎更新和修复会跟随仍在活跃维护的上游 fork：[fallout2-ce/fallout2-ce](https://github.com/fallout2-ce/fallout2-ce)。

### PS Vita 安装

下载或自行构建 `fallout2-ce.vpk`，然后安装到 PS Vita。

将已安装的 Fallout 2 游戏资源复制到：

```text
ux0:data/fallout2/
```

需要复制的文件和目录：

- `master.dat`
- `critter.dat`
- `patch000.dat`
- `data`
- `sound`
- 如果使用本地化/非英文版本，请复制 `fallout2.cfg`，或确认其中的 `language` 设置正确。

实机建议安装 [FdFix](https://github.com/TheOfficialFloW/FdFix)，以获得更可靠的挂起/恢复行为。

### PS Vita 构建

依赖：

- VitaSDK
- SDL2 for Vita

本仓库使用 VitaSDK 作为交叉编译工具链。项目内的 `vita/vita.cmake` 负责 VPK 打包逻辑，并会在设置 `-DVITA=ON` 时自动引入。

```console
$ mkdir -p build_vita
$ cd build_vita
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake -DCMAKE_BUILD_TYPE=None -DVITA=ON
$ make -j4
```

构建产物：

- `build_vita/fallout2-ce.self`
- `build_vita/fallout2-ce.vpk`

### PS Vita 按键

- 左摇杆 - 移动鼠标指针
- 右摇杆 - 滚动地图
- 叉 - 鼠标左键
- 圈 - 鼠标右键
- 方块 - 技能列表/选择
- 三角 - 物品栏
- 方向键上 - 角色界面
- 方向键下 - Pip-Boy
- 方向键左 - 开始战斗
- 方向键右 - 结束回合
- L1 - 切换当前物品
- R1（按住） - 鼠标加速
- SELECT - Esc / 退出键
- START - 屏幕键盘 / IME
- 圈 + L1 - 快速存档
- 圈 + R1 - 快速读档

### 触摸控制

Vita 正面触摸屏可用于触摸输入。具体触摸行为可根据当前构建和配置文件在游戏配置中调整。

## 上游项目说明

Fallout 2 Community Engine 是 Fallout 2 引擎的完整重实现，面向多平台提供更顺畅的体验，并包含高分辨率支持、易用性改进和大量错误修复。

本仓库基于原始 Fallout2: CE 项目，并从活跃维护的 [fallout2-ce/fallout2-ce](https://github.com/fallout2-ce/fallout2-ce) fork 同步更新。

常见的 Fallout 2 大型改造 Mod 有部分支持。Nevada 和 Sonora 的原版通常可用；[Fallout 2 Restoration Project](https://github.com/BGforgeNet/Fallout2_Restoration_Project) 处于 Beta 支持状态；[Fallout Et Tu](https://github.com/rotators/Fo1in2) 和 [Olympus 2207](https://olympus2207.com) 暂未支持。其他 Mod 尚未充分测试。

Fallout2: CE 对 [Sfall](https://github.com/sfall-team/sfall) 脚本扩展提供了较广泛但并非完整的兼容性，许多传统 Fallout Mod 可以直接运行。

另有 [Fallout 1 Community Edition](https://github.com/alexbatalov/fallout1-ce) 项目（与本 fork 无直接隶属关系）。

## 安装

你必须拥有正版游戏才能游玩。可在 [GOG](https://www.gog.com/game/fallout_2)、[Epic Games](https://store.epicgames.com/p/fallout-2) 或 [Steam](https://store.steampowered.com/app/38410) 购买 Fallout 2。可以下载 [fallout2-ce/fallout2-ce releases](https://github.com/fallout2-ce/fallout2-ce/releases) 或自行从源码构建。

### Windows

下载发行包并解压到 `Fallout2` 目录，然后运行 `fallout2-ce.exe`。

### Linux

- 使用 Windows 版安装目录作为基础，它包含运行所需的数据资源。将 `Fallout2` 目录复制到合适位置，例如 `/home/john/Desktop/Fallout2`。
- 也可以用 `innoextract` 从 GOG 安装包中提取资源。
- 下载 Linux 发行包，将 `fallout2-ce` 和 `ce.dat` 复制到该目录。
- 运行 `./fallout2-ce`。

### macOS

macOS 10.11 或更高版本可用，支持 Intel Mac 和 Apple Silicon。使用 Windows 版或 Macplay/The Omni Group 版资源作为基础，将发行包中的应用复制到 Fallout 2 数据目录后运行。

### Android / iOS / Browser

这些平台的安装方式请参考英文 README 对应章节。移动端主要采用触摸板式控制：单指移动鼠标，单指点击为左键，双指点击为右键，双指拖动用于滚动。

## 配置

主配置文件是 `fallout2.cfg`。根据你的游戏资源大小写情况，可能需要调整 `master_dat`、`critter_dat`、`master_patches` 和 `critter_patches` 等路径设置，或直接重命名资源文件以匹配配置。

音乐目录可能位于 `data/sound/music/` 或 `sound/music/`。请确保 `music_path1` 与实际目录完全一致。音乐文件本身通常应保持大写 `.ACM` 文件名。

屏幕分辨率、UI 自定义和地图选项已整合到 `fallout2.cfg` 中。启动时如果检测到旧的 `f2_res.ini`，会自动迁移相关设置。

## 相比原版的易用性改进

- 高分辨率支持
- 2 列物品栏
- 4 行交易界面
- 扩展 AP 条
- 队友可代替主角拾取和交易
- 更准确的寻路
- 交易、拾取、偷窃时支持快捷移动物品
- 跨地图保持音乐播放
- 自动开门
- 支持 44.1 kHz 立体声音乐/音效，支持 `.ogg`、`.wav` 和传统 `.acm`
- 记住上次使用的存档槽
- 物品/尸体/容器/生物高亮

## 贡献

构建说明和贡献者备注见 [CONTRIBUTING.md](CONTRIBUTING.md)。当前重点是整合 Sfall 兼容功能和必要的易用性改进。当前 Sfall 兼容状态见 [SFALL_COMPATIBILITY.md](SFALL_COMPATIBILITY.md)。

## Mod 作者

- 较完整但非 100% 的 Sfall opcode 和 hook 支持
- 地图音乐支持 `.ogg` / `.wav`
- 静态资源支持 8 位索引色 `.png`
- 支持 `.zip` 格式数据包
- 可用的 BIS mapper

## 许可证

本仓库源码使用 [Sustainable Use License](LICENSE.md)。
