import os
import pathlib
import shutil
import json
import platform
import sys


def execute(command: str):
    print(f"Execute {command}")
    os.system(command)


def apply_packaging():
    if platform.system() == 'Darwin':
        deploy_tools = qt_path / "bin" / "macdeployqt"
        build_apps = list(build_dir.glob("*/*.app"))
        print(f"Found: {build_apps}")
        if len(build_apps) != 1:
            print("Expect length = 1, exit.")
            exit(-1)

        build_app = build_apps[0]
        package_app = app_dir / f"{prefix}-OSX-Universal-{version}.app"
        shutil.copytree(build_app, package_app)
        execute(f"{deploy_tools} {package_app} -dmg -qmldir={build_dir / 'src' / 'view'}")
        output_name = package_app.name[:-4] + ".dmg"

    elif platform.system() == 'Windows':
        deploy_tools = qt_path / "bin" / "windeployqt.exe"
        build_exes = list(build_dir.glob("*/*.exe"))
        print(f"Found: {build_exes}")
        if len(build_exes) != 1:
            print("Expect length = 1, exit.")
            exit(-1)

        build_exe = build_exes[0]
        tmp_dir = app_dir / f"{prefix}-Windows-x64-{version}"
        tmp_dir.mkdir(exist_ok=True)
        package_exe = tmp_dir / (tmp_dir.name + ".exe")
        shutil.copy(build_exe, package_exe)
        execute(f"{deploy_tools} {package_exe} --release --qmldir={build_dir / 'src' / 'view'}")
        shutil.make_archive(str(app_dir / tmp_dir.name), 'zip', tmp_dir)
        output_name = package_exe.name[:-4] + ".zip"
    elif platform.system() == 'Linux':
        execute(f"cd {build_dir} && cmake --install . --prefix AppDir")
        appdir_dir = build_dir / "AppDir"
        recipe_template = build_dir.parent / "config" / "linux-x64" / "AppImageBuilder.yml"
        with open(recipe_template, "r", encoding="utf-8-sig") as f:
            content = f.read()
        variables = {
            "{VERSION}": "0.0.1",
            "{QT_DIR}": str(qt_path),
            "{APP_DIR}": str(appdir_dir),
            # "{APT_SRC}": "https://mirrors.tuna.tsinghua.edu.cn/ubuntu/",
            "{APT_SRC}": "http://archive.ubuntu.com/ubuntu/",
        }
        for k, v in variables.items():
            content = content.replace(k, v)
        recipe_generate = build_dir / "AppImageBuilder.yml"
        with open(recipe_generate, "w", encoding="utf-8-sig") as f:
            f.write(content)
        execute(f"cd {build_dir} && appimage-builder --recipe {recipe_generate} --skip-test")
        generate_app_images = list(build_dir.glob("*.AppImage"))
        if len(generate_app_images) != 1:
            raise RuntimeError(f"Incorrect number of AppImage: {generate_app_images}.")
        target_app_image = app_dir / f"{prefix}-Linux-{version}.AppImage"
        generate_app_images[0].rename(target_app_image)
        output_name = target_app_image.name
    else:
        raise RuntimeError("Unsupported cmake: " + platform.system())

    # print(f"::set-output name=app_file::{output_name}")


if __name__ == "__main__":
    self_path = pathlib.Path(sys.argv[0]).absolute()
    with open(self_path.parent.parent / "config" / "user.json", "r", encoding="utf-8-sig") as f:
        conf = json.load(f)
        qt_path = conf["qt_path"]
        prefix = conf["prefix"]
        version = conf["version"]
    qt_path = pathlib.Path(qt_path)

    app_dir = self_path.parent.parent / "app"
    if app_dir.exists():
        shutil.rmtree(app_dir)
    app_dir.mkdir()

    build_dir = self_path.parent.parent / "build"

    print(f"BUILD AIR: {build_dir}")
    print(f"APP DIR: {app_dir}")
    # apply_packaging()

    command = f"cd {build_dir} && cmake --build . --target package"
    print(f"Execute {command}.")
    os.system(command)
    for package_file in build_dir.glob("*.*"):
        if package_file.suffix in {'.zip', '.msi'}:
            shutil.copy(package_file, app_dir / package_file.name)
