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
        package_app = app_dir / "PonyPlayer.app"
        package_dmg = app_dir / f"{prefix}-OSX-Universal-{version}.dmg"
        project_dir = app_dir.parent
        shutil.copytree(build_app, package_app)
        execute(f"{deploy_tools} {package_app} "
                f"-qmldir={build_dir / 'src' / 'view'} "
                f"-codesign=- "
                f"-hardened-runtime "
                f"-timestamp")
        execute(f"codesign --remove-signature --deep {package_app}")
        execute(f"codesign --sign - --timestamp --force --deep --all-architecture {package_app}")
        execute("create-dmg "
                "--volname \"PonyPlayer Installer\" "
                f"--background \"{project_dir / 'assets' / 'macosInstaller.png' }\" "
                "--window-pos 200 120 "
                "--window-size 995 640 "
                "--icon-size 96 "
                "--icon PonyPlayer.app 220 275 "
                "--hide-extension PonyPlayer.app "
                "--app-drop-link 780 275 "
                "--no-internet-enable "
                f"--eula \"{project_dir / 'docs' / 'LICENSE.RTF' }\" "
                f"--add-file LICENSE \"{project_dir / 'docs' / 'LICENSE.RTF' }\" 900 465 "
                "--hdiutil-quiet "
                f"{package_dmg} "
                f"{package_app}")

    elif platform.system() == 'Windows':
        execute(f"cd {build_dir} && cmake --build . --target package")
        for package_file in build_dir.glob("*.*"):
            if package_file.suffix in {'.zip', '.msi'}:
                shutil.copy(package_file, app_dir / f"{prefix}-win64-{version}{package_file.suffix}")

    elif platform.system() == 'Linux':
        execute(f"cd {build_dir} && cmake --install . --prefix AppDir/usr")
        appdir_dir = build_dir / "AppDir"
        recipe_template = build_dir.parent / "config" / "AppImageBuilder.yml"
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

    apply_packaging()
