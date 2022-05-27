from argparse import ArgumentParser
import platform
import shutil
import subprocess
import pathlib
import json
import sys
import os


def execute(command: str):
    print(f"Execute {command}")
    os.system(command)


if __name__ == "__main__":
    parser = ArgumentParser(description="Build cmake project. Default: :\n"
                                        "python .py --qt_path /Users/username/Qt/6.2.4/macos\n"
                                        "python setup.py --qt_path C:\\Qt\\6.2.4\\mingw8_64")
    group = parser.add_mutually_exclusive_group()
    group.add_argument("-clean", type=bool, help="Clean generated files.", default=False)
    group.add_argument("-update", type=bool, help="Do incremental compilation.", default=False)
    args = parser.parse_args()
    self_path = pathlib.Path(sys.argv[0]).absolute()
    build_dir = self_path.absolute().parent.parent / "build"
    if args.clean:
        shutil.rmtree(build_dir)
        build_dir.mkdir(exist_ok=False)
        exit(0)
    elif args.update:
        build_dir.mkdir(exist_ok=True)
    else:
        if build_dir.exists():
            shutil.rmtree(build_dir)
        build_dir.mkdir(exist_ok=True)

    with open(self_path.parent.parent / "config" / "user.json", "r", encoding="utf-8-sig") as f:
        conf = json.load(f)
        qt_path = conf["qt_path"].replace("\\", "/")
        ninja_path = conf['ninja_path'].replace("\\", "/")
        compiler_path = conf['compiler_path'].replace("\\", "/")

    toolset = os.path.join(qt_path, "lib/cmake/Qt6/qt.toolchain.cmake")

    if platform.system() == 'Windows':
        os_dependent_args = "-DCMAKE_CXX_FLAGS=\"${CMAKE_CXX_FLAGS} -mwindows\" "
    elif platform.system() == 'Darwin':
        os_dependent_args = "-DCMAKE_OSX_ARCHITECTURES=\"x86_64;arm64\" "
    else:
        os_dependent_args = ""

    cmake_command = f"cmake -DCMAKE_MAKE_PROGRAM={ninja_path} -G Ninja -DCMAKE_TOOLCHAIN_FILE={toolset} {os_dependent_args}" \
                    f".."

    command = f"cd {build_dir} " \
              f"&& {cmake_command} " \
              f"&& cmake --build . --config Release"
    print("Execute: " + command)
    if platform.system() == 'Windows':
        modify_environment = os.environ.copy()
        compiler_path = compiler_path.replace("\\", "/")
        modify_environment['PATH'] = compiler_path + ";" + modify_environment['PATH']
        p = subprocess.Popen(command, shell=True, env=modify_environment)
        p.wait()
    else:
        os.system(command)
