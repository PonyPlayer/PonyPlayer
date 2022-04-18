import platform
from argparse import ArgumentParser
import pathlib
import json
import os
import sys


PREFIX = "PonyPlayer"
VERSION = "0.0.1-alpha"

def convert_and_check_exists(path: os.PathLike, name: str) -> pathlib.Path:
    if os.path.exists(path):
        return pathlib.Path(path)
    else:
        print(f"Not the {name} path: {path}.")
        exit(-1)

def convert_and_check_exists_dir(path: os.PathLike, name: str) -> pathlib.Path:
    if os.path.isdir(path):
        return pathlib.Path(path)
    else:
        print(f"Not the {name} dir: {path}.")
    exit(-1)

def check_auto_detect(path: os.PathLike, name: str):
    if not os.path.exists(path):
        print(f"Cannot auto detect {name}, please manually specify.")
        exit(-1)


if __name__ == "__main__":
    print("Python: " + sys.version)
    parser = ArgumentParser(description="Setup cmake project. Example:\n"
                                        "python setup.py --qt_path /Users/username/Qt/6.2.4/macos\n"
                                        "python setup.py --qt_path C:\\Qt\\6.2.4\\mingw8_64")
    parser.add_argument("--qt_path", type=str, help="Indicate the Qt6 library path.", required=True)
    parser.add_argument("--ffmpeg_path", type=str, help="Indicate the FFmpeg path.", required=True)
    parser.add_argument("--ninja_path", type=str, help="Indicate the compiler path.", default='')
    parser.add_argument("--compiler_path", type=str, help="Indicate the compiler bin path.", default='')
    args = parser.parse_args()
    # check qt path
    qt_path = convert_and_check_exists_dir(args.qt_path, "Qt6")
    qt_library_cmake_dir = qt_path / "lib" / "cmake"
    if not qt_library_cmake_dir.is_dir():
        print(f"Not the Qt6 library path: {args.qt_path}.")
        exit(-1)
    print(f"Found CMake configuration: {qt_library_cmake_dir}.")

    # check ninja path
    if len(args.ninja_path) > 0:
        ninja_path = convert_and_check_exists(args.ninja_path, "Ninja")
    else:
        ninja_dir = qt_path.parent.parent / "Tools" / "Ninja"
        if platform.system() == 'Windows':
            ninja_path = ninja_dir / "ninja.exe"
        else:
            # Linux and Darwin
            ninja_path = ninja_dir / "ninja"
        check_auto_detect(ninja_path, "Ninja")
    print(f"Found Ninja: {ninja_path}")

    # check compiler path
    if len(args.compiler_path) > 0:
        compiler_path = convert_and_check_exists_dir(args.compiler_path, "compiler path")
    else:
        if platform.system() == 'Windows':
            compiler_path = qt_path.parent.parent / "Tools" / "mingw1120_64" / "bin"
            check_auto_detect(compiler_path, "compiler path")
        else:
            compiler_path = "AUTO"
    print(f"Found compiler: {compiler_path}")

    # check ffmpeg path
    ffmpeg_path = convert_and_check_exists_dir(args.ffmpeg_path, "FFmpeg")
    if not (ffmpeg_path / "bin" / "ffmpeg.exe").exists() and not (ffmpeg_path / "bin" / "ffmpeg").exists():
        print(f"Not the FFmpeg path {ffmpeg_path}.")
        exit(-1)
    print(f"Found FFmpeg: {ffmpeg_path}.")



    # write config and cmake
    self_path = pathlib.Path(sys.argv[0]).absolute()
    cmake_find_qt_path = self_path.parent.parent / "cmake" / "find_dependence.cmake"
    with open(cmake_find_qt_path, "w", encoding="utf-8-sig") as f:
        f.write('set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};%s")' % str(qt_library_cmake_dir).replace('\\', '/') + "\n")
        f.write('set(FFMPEG_PREFIX_PATH "%s")' % str(ffmpeg_path).replace('\\', '/') + "\n")
    print(f"Write to {cmake_find_qt_path}.")

    config_folder = self_path.parent.parent / "config"
    if not config_folder.exists():
        config_folder.mkdir()
    config_path = config_folder / "user.json"
    with open(config_path, "w", encoding="utf-8-sig") as f:
        json.dump({
            "qt_path": str(qt_path),
            "ffmpeg_path": str(ffmpeg_path),
            "ninja_path": str(ninja_path),
            "compiler_path": str(compiler_path),
            "prefix": PREFIX,
            "version": VERSION,
        }, f)
    print(f"Write to {config_path}.")

    print("=" * 5 + "Setup Info" + "=" * 5)
    print(f"Write to {cmake_find_qt_path}.")
    print(f"QT_PATH: {qt_path}")
    print(f"FFMPEG_PATH: {ffmpeg_path}")
    print(f"NINJA_PATH: {ninja_path}")
    print(f"COMPILER_PATH: {compiler_path}")
