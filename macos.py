import os

# os.system("/Users/colorswind/Qt/6.2.4/macos/bin/macdeployqt "
#           "/Users/colorswind/PonyPlayer/cmake-build-release/src/PonyPlayer.app "
#           "-qmldir=/Users/colorswind/PonyPlayer/src/view "
#           "-appstore-compliant -hardened-runtime -timestamp")
# os.system("codesign --remove-signature --deep /Users/colorswind/PonyPlayer/cmake-build-release/src/PonyPlayer.app")
# os.system("codesign --sign - --timestamp --force --deep --all-architecture /Users/colorswind/PonyPlayer/cmake-build-release/src/PonyPlayer.app")
# os.mkdir("/Users/colorswind/PonyPlayer/cmake-build-release/package")
# os.renames("/Users/colorswind/PonyPlayer/cmake-build-release/src/PonyPlayer.app", "/Users/colorswind/PonyPlayer/cmake-build-release/package/PonyPlayer.app")
try:
    os.remove("PonyPlayer.dmg")
except:
    pass

command = ('create-dmg '
           '--volname "PonyPlayer Installer" '
           '--background "/Users/colorswind/PonyPlayer/assets/macosInstaller.png" '
           '--window-pos 200 120 '
           '--window-size 995 650 '
           '--icon-size 96 '
           '--icon PonyPlayer.app 220 300 '
           '--hide-extension PonyPlayer.app '
           '--app-drop-link 780 300 '
           '--no-internet-enable '
           # '--eula "/Users/colorswind/PonyPlayer/docs/LICENSE.RTF" '
           '--add-file LICENSE "/Users/colorswind/PonyPlayer/docs/LICENSE.RTF" 900 500 '
           # '--skip-jenkins '
           # '--hdiutil-quiet '
           'PonyPlayer.dmg '
           '/Users/colorswind/PonyPlayer/cmake-build-release/package/PonyPlayer.app')
print(command)
os.system(command)
