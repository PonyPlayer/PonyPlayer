function mytest(path) {
    console.log(path)
}

//动态加载滤镜
function loadingFilters() {
    let fileNames = ["Contrast", "Flim", "Video"]
    let component = Qt.createComponent("FilterItem.qml")
    let prefix = videoArea.filterPrefix
    let origin = component.createObject(filter, {"filterName": "origin", "image": (prefix + "/origin.jpg"), "lut": ""})
    filter.addItem(origin)
    let jsons = videoArea.filterJsons
    for (let i = 0; i < jsons.length; i++) {
        var json = JSON.parse(jsons[i]);
        console.log(json.length)
        for (let j = 0; j < json.length; j++) {
            let item = component.createObject(filter, {
                "filterName": (fileNames[i] + ":  " + j),
                "image": ("file://" + prefix + '/' + json[j].image),
                "lut": json[j].lut
            })
            item.sentFilterLut.connect(videoArea.setLUTFilter)
            filter.addItem(item)
        }
    }
}

function forwardOneSecond() {
    if (mainWindow.endTime == 0.0) {
        return
    }
    if (mainWindow.endTime > mainWindow.currentTime) {
        mainWindow.currentTime = mainWindow.currentTime + 1.0
    } else {
        mainWindow.currentTime = mainWindow.endTime
    }
    videoSlide.value = mainWindow.currentTime
    videoArea.seek(mainWindow.currentTime)
}

function forwardFiveSeconds() {
    if (mainWindow.endTime == 0.0) {
        return
    }
    if ((mainWindow.endTime - mainWindow.currentTime) > 5.0) {
        mainWindow.currentTime = mainWindow.currentTime + 5.0
    } else {
        mainWindow.currentTime = mainWindow.endTime
    }
    videoSlide.value = mainWindow.currentTime
    videoArea.seek(mainWindow.currentTime)
}

function backOneSecond() {
    if (mainWindow.currentTime == 0.0) {
        return
    }
    if (mainWindow.currentTime > 1.0) {
        mainWindow.currentTime = mainWindow.currentTime - 1.0
    } else {
        mainWindow.currentTime = 0.0
    }
    videoSlide.value = mainWindow.currentTime
    videoArea.seek(mainWindow.currentTime)
}

function backFiveSeconds() {
    if (mainWindow.currentTime == 0.0) {
        return
    }
    if (mainWindow.currentTime > 5.0) {
        mainWindow.currentTime = mainWindow.currentTime - 5.0
    } else {
        mainWindow.currentTime = 0.0
    }
    videoSlide.value = mainWindow.currentTime
    videoArea.seek(mainWindow.currentTime)
}

function volumnUp() {
    if (mainWindow.volumn < 0.9) {
        mainWindow.volumn = mainWindow.volumn + 0.1
        mainWindow.beforeMute = mainWindow.volumn
        volumnSlider.value = mainWindow.volumn * 100
    } else {
        mainWindow.volumn = 1
        mainWindow.beforeMute = 1
        volumnSlider.value = 100
    }
    mainWindow.volumnChange(mainWindow.volumn)
    videoArea.setVolume(mainWindow.volumn)
}

function volumnDown() {
    if (mainWindow.volumn < 0.1) {
        mainWindow.volumn = 0
        mainWindow.beforeMute = 0
        volumnSlider.value = 0
    } else {
        mainWindow.volumn = mainWindow.volumn - 0.1
        mainWindow.beforeMute = mainWindow.volumn
        volumnSlider.value = mainWindow.volumn * 100
    }
    mainWindow.volumnChange(mainWindow.volumn)
    videoArea.setVolume(mainWindow.volumn)
}

function volumeSliderOnMoved() {
    mainWindow.volumn = volumnSlider.value / 100
    mainWindow.beforeMute = volumnSlider.value / 100
    mainWindow.volumnChange(mainWindow.volumn)
    videoArea.setVolume(mainWindow.volumn)
}

function speakerOnClicked() {
    if (mainWindow.volumn === 0) {
        mainWindow.volumn = mainWindow.beforeMute
        volumnSlider.value = Math.floor(mainWindow.volumn * 100)
    } else {
        mainWindow.beforeMute = mainWindow.volumn
        mainWindow.volumn = 0
        volumnSlider.value = 0
    }
    mainWindow.volumnChange(mainWindow.volumn)
    videoArea.setVolume(mainWindow.volumn)
}

function playModeOnClicked() {
    if (mainWindow.playState === "ordered") {
        mainWindow.playState = "single"
    } else if (mainWindow.playState === "single") {
        mainWindow.playState = "random"
    } else {
        mainWindow.playState = "ordered"
    }
    mainWindow.playModeChange(playState)
}

function invertedOnClicked() {
    if (mainWindow.isInverted) {
        mainWindow.isInverted = false
        videoArea.forward();
    } else {
        mainWindow.isInverted = true
        videoArea.backward();
    }
    //mainWindow.inverted(mainWindow.step)
}

function fileListOnClicked() {
    if (mainWindow.isVideoListOpen) {
        mainWindow.isVideoListOpen = false
    } else {
        mainWindow.isVideoListOpen = true
    }
}

function videoSlideDistance(flag) {
    let tmp
    if (flag) {
        tmp = Math.floor(mainWindow.currentTime)
    } else {
        tmp = Math.floor(mainWindow.endTime - mainWindow.currentTime)
    }
    if (tmp < 60) {
        return tmp + ''
    } else if (tmp >= 60 && tmp < 3600) {
        let tal = tmp % 60
        let mid = Math.floor(tmp / 60)
        if (tal < 10) {
            tal = '0' + tal
        }
        return mid + ':' + tal
    } else {
        let tal = tmp % 60
        let had = Math.floor(tmp / 3600)
        let mid = (Math.floor(tmp / 60)) % 60
        if (tal < 10) {
            tal = '0' + tal
        }
        if (mid < 10) {
            mid = '0' + mid
        }
        return had + ':' + mid + ':' + tal
    }
}

function videoAreaOnClicked() {
    if (mainWindow.isPlay) {
        mainWindow.isPlay = false
        mainWindow.stop()
    } else {
        mainWindow.isPlay = true
        mainWindow.start()
    }
}

function mainAreaInit() {
    mainWindow.start.connect(videoArea.start)
    mainWindow.stop.connect(videoArea.pause)
    mainWindow.openFile.connect(videoArea.openFile)
    mainWindow.setSpeed.connect(videoArea.setSpeed)
}


function isBoundary() {
    //左边界
    if (mainWindow.isInverted && (mainWindow.currentTime <= 0)) {
        toVideoBegining()
        operationFailedDialogText.text = "已到达开头，无法继续倒放"
        operationFailedDialog.open()
        return true
    }
    //右边界
    else if (!mainWindow.isInverted && (mainWindow.currentTime >= mainWindow.endTime)) {
        toVideoEnd()
        nextOnClicked()
        toVideoBegining()
        return true
    }
    return false
}

function timerOnTriggered() {
    mainWindow.currentTime = videoArea.getPTS()
    if (!isBoundary()) {
        videoSlide.value = mainWindow.currentTime
    }
}

function toVideoBegining() {
    mainWindow.isPlay = false
    mainWindow.currentTime = 0
    mainWindow.wakeSlide()
}

function toVideoEnd() {
    mainWindow.isPlay = false
    mainWindow.currentTime = mainWindow.endTime
    videoSlide.value = mainWindow.endTime
}

function toPause() {
    toVideoBegining()
    mainWindow.cease()
    mainWindow.stop()
    videoArea.seek(0)
}

function playOrPauseFunction() {
    if (!mainWindow.isPlay) {
        if ((mainWindow.endTime !== 0.0) && !isBoundary()) {
            mainWindow.isPlay = true
            mainWindow.start()
        }
    } else {
        mainWindow.isPlay = false
        mainWindow.stop()
    }
}

function solveStateChanged() {
    if (videoArea.state == 1) {
        toVideoBegining()
    } else if (videoArea.state == 2) {
        mainWindow.endTime = 0
        toVideoBegining()
        return
    } else if (videoArea.state == 4) {
        mainWindow.isPlay = true
    } else if (videoArea.state == 6) {
        mainWindow.isPlay = false
        IF.makeTrackMenu()
    }
}

function nextOnClicked() {
    console.log("playState:", mainWindow.playState)
    if (mainWindow.playState === "ordered")
        listview.currentIndex = (listview.currentIndex + 1) % listview.count

    else if (mainWindow.playState === "random")
        listview.currentIndex = (listview.currentIndex + Math.floor(Math.random() * listview.count)) % listview.count
    else ;
    console.log("index:", listview.currentIndex)
    mainWindow.openFile(listModel.get(listview.currentIndex).filePath);
    mainWindow.endTime = Math.floor(videoArea.getVideoDuration())
}

function makeDeviceMenu(list) {
    if (mainWindow.devicesMenuStation) {
        mainWindow.devicesMenuStation.destroy()
    }
    mainWindow.devicesMenuStation = Qt.createQmlObject('import QtQuick 2.13; import QtQuick.Controls 2.13; Menu{}', menu)
    menu.addItem(mainWindow.devicesMenuStation)
    let component = Qt.createComponent("OutputDevice.qml")
    for (let i = 0; i < list.length; i++) {
        let item = component.createObject(mainWindow.devicesMenuStation, {"text": list[i], "deviceName": list[i]})
        item.selectDevice.connect(videoArea.setSelectedAudioOutputDevice)
        devicesMenu.addItem(item)
    }
}

function makeTrackMenu() {
    if (mainWindow.trackMenu) {
        mainWindow.trackMenu.destroy()
    }
    var tmpList = videoArea.getTracks()
    mainWindow.trackMenu = Qt.createQmlObject('import QtQuick 2.13; import QtQuick.Controls 2.13; Menu{}', menu)
    menu.addItem(mainWindow.trackMenu)
    let component = Qt.createComponent("TrackItem.qml")
    for (let i = 0; i < tmpList.length; i++) {
        let item = component.createObject(mainWindow.trackMenu, {"text": tmpList[i], "trackID": i})
        item.setTrack.connect(videoArea.setTrack)
        trackmenu.addItem(item)
    }
}

function makeFileList() {
    if (mainWindow.currentFilePathStation) {
        mainWindow.currentFilePathStation.destroy()
    }
    var tmpList = mediaLibController.getRecentFiles()
    mainWindow.currentFilePathStation = Qt.createQmlObject('import QtQuick 2.13; import QtQuick.Controls 2.13; Menu{}', menu)
    menu.addItem(mainWindow.currentFilePathStation)
    let component = Qt.createComponent("CurrentFileItem.qml")
    for (let i = 0; i < tmpList.length; i++) {
        let item = component.createObject(mainWindow.currentFilePathStation, {
            "text": tmpList[i][0],
            "filePath": tmpList[i][1],
            "fileName": tmpList[i][0]
        })
        item.addFilePath.connect(videoListOperatorOnAccepted)
        currentFilePathList.addItem(item)
    }
}

function videoListOperatorOnAccepted(path = "", name = "") {
    let acceptedFileName = fileDialog.currentFile;
    let acceptedFileFold = fileDialog.currentFolder;
    if (path != "") {
        acceptedFileName = path
        let folder = path.replace(name, "")
        folder = folder.substring(0, folder.length - 1)
        acceptedFileFold = folder
    }
    mediaLibController.updateRecentFile(acceptedFileName);
    mainWindow.openFile(acceptedFileName);
    mainWindow.endTime = Math.floor(videoArea.getVideoDuration());
    var exists = false;
    for (var i = 0; i < listModel.count; i++) {
        if (listModel.get(i).filePath == acceptedFileName) {
            listview.currentIndex = i;
            exists = true;
            break;
        }
    }
    if (!exists) {
        let selectedFileName = acceptedFileName.toString().substring(
            acceptedFileFold.toString().length + 1)
        mediaLibController.getFile(selectedFileName, acceptedFileName);
        listModel.append({
            "fileName": selectedFileName,
            "filePath": acceptedFileName,
            "iconPath": "interfacepics/defaultlogo"
        });
        listview.currentIndex = listModel.count - 1;
    }

    console.log("QML D:", listModel.get(listview.currentIndex).filePath);
}

function trans(path, name) {
    let folder = path.replace(name, "")
    console.log(folder)
}

function hideComponents() {
    mainWindow.isVideoListOpen = false
    mainWindow.isFooterVisible = false
    mainWindow.isTopBarVisible = false
    mainWindow.mouseFlag = true
}

function showComponents() {
    holder.restart()
    mainWindow.isFooterVisible = true
    mainWindow.isTopBarVisible = true
}

function screenSizeFunction() {
    mainWindow.isFullScreen = false
    if (mainWindow.visibility === 2) {
        mainWindow.visibility = 4
        mainWindowReduction.imageSource = "interfacepics/mainWindowReduction"
    } else {
        mainWindow.visibility = 2
        mainWindowReduction.imageSource = "interfacepics/mainWindowMaximize"
    }
}
function footerOnCompleted(){
    mainWindow.wakeSlide.connect(sliderToFront)
    mainWindow.mainWindowLostFocus.connect(lostFocus)
}
function sliderToFront(){
    console.log("slider    vake    now")
    videoSlide.value=0.0
}
function lostFocus(){
    previewRect.visible=false
}