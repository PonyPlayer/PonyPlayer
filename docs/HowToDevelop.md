# How to Develop this Project

## 1 配置环境

### 项目依赖

- Qt 6.2.4
- FFmpeg 5.0.1
- CMake 3.16或以上版本
- Ninja
- Python 3.7或以上版本
- 基于cmake的IDE，推荐使用Clion
- Figma （UI原型设计工具）
- Qt Designer Studio （QML设计工具）

### Windows 配置步骤

1. 安装 Qt 6.2.4 

下载官方MaintainTool https://www.qt.io/download，安装：

```
Qt -> Qt 6.2.4 -> mingw11
Developer and Designer Tools -> CMake
Developer and Designer Tools -> Ninja
Developer and Designer Tools -> mingw11
```

2. FFmpeg

官方推荐使用 [gyan.dev](https://www.gyan.dev/ffmpeg/builds/) 提供的预编译包。可以点击 [点击这里](https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-full-shared.7z) `ffmpeg-release-full-shared.7z`。
解压到`C:/ProgramData/FFmpeg`

备用方案：通过`chocolatey`安装

```bash
choco install ffmpeg-shared
```

https://community.chocolatey.org/packages/ffmpeg

3. 设置项目，结合实际修改

```bash
python scripts/setup.py --qt_path C:/ProgramData/Qt/6.2.4/mingw11 --ffmpeg_path C:/ProgramData/FFmpeg
```

### macOS 配置步骤

1. 安装 Qt 6.2.4 

下载官方MaintainTool https://www.qt.io/download，安装：

```
Qt -> Qt 6.2.4 -> macos
Developer and Designer Tools -> CMake
Developer and Designer Tools -> Ninja
```

2. FFmpeg

对于开发，我们推荐使用`homebrew`安装FFmpeg



然而，如果需要打包成`universal binrary`，需要`universal binrary`版本的FFmpeg。我们自己编译了FFmpeg：

https://github.com/ColorsWind/FFmpeg-macOS/releases
```bash
brew install ffmpeg
```

3. 设置项目，结合实际修改

```bash
python scripts/setup.py --qt_path /Users/runner/Qt/6.2.4/macos --ffmpeg_path /opt/homebrew
```

### Linux 配置步骤

*coming soon...*



## 2 新建文件

### .h 头文件

应当在`include`目录创建；

### .cpp 源文件

在`src`相应文件中创建，并修改相应的`CMakeList.txt`