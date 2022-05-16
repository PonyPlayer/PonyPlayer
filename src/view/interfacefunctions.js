
function mytest(path){
    console.log(path)
}


//下面三个函数中的url设定为json文件在view目录下的情况，根据实际情况需要额外添加前缀
//例如在第20行的......"image":json[i].image  会把展示的image的路径读进对像中作为属性，更具实际需求可在json[i].image前添加前缀，才能正确读取到图片
//例如在第20行的......"lut":json[i].lut  会把lut的路径读进对像中作为属性，更具实际需求可在json[i].lut前添加前缀
function loadingFilterContrast() {
    let component=Qt.createComponent("FilterItem.qml")
    let url = "contrast.json"
    let request = new XMLHttpRequest();
    request.open("get", url);
    request.send(null);
    request.onload = function () {
        if (request.status == 200) {
            var json = JSON.parse(request.responseText);
            for(let i=0;i<json.length;i++){
                let item=component.createObject(filtercontrast,{"filterName":("contrast:  "+i),"image":json[i].image,"lut":json[i].lut})
                filtercontrast.addItem(item)
            }
        }
    }
}
function loadingFilterFlim() {
    let component=Qt.createComponent("FilterItem.qml")
    let url = "flim.json"
    let request = new XMLHttpRequest();
    request.open("get", url);
    request.send(null);
    request.onload = function () {
        if (request.status == 200) {
            var json = JSON.parse(request.responseText);
            for(let i=0;i<json.length;i++){
                let item=component.createObject(filterflim,{"filterName":("flim:  "+i),"image":json[i].image,"lut":json[i].lut})
                filterflim.addItem(item)
            }
        }
    }
}
function loadingFilterVideo() {
    let component=Qt.createComponent("FilterItem.qml")
    let url = "video.json"
    let request = new XMLHttpRequest();
    request.open("get", url);
    request.send(null);
    request.onload = function () {
        if (request.status == 200) {
            var json = JSON.parse(request.responseText);
            for(let i=0;i<json.length;i++){
                let item=component.createObject(filtervideo,{"filterName":("video:  "+i),"image":json[i].image,"lut":json[i].lut})
                filtervideo.addItem(item)
            }
        }
    }
}
function forwardOneSecond(){
    if(mainWindow.endTime==0.0){
        return
    }
    if(mainWindow.endTime>mainWindow.currentTime){
        mainWindow.currentTime=mainWindow.currentTime+1.0
    }
    else{
        mainWindow.currentTime=mainWindow.endTime
    }
    videoSlide.value=mainWindow.currentTime
    videoArea.seek(mainWindow.currentTime)
}
function forwardFiveSeconds(){
    if(mainWindow.endTime==0.0){
        return
    }
    if((mainWindow.endTime-mainWindow.currentTime)>5.0){
        mainWindow.currentTime=mainWindow.currentTime+5.0
    }
    else{
        mainWindow.currentTime=mainWindow.endTime
    }
    videoSlide.value=mainWindow.currentTime
    videoArea.seek(mainWindow.currentTime)
}
function backOneSecond(){
    if(mainWindow.currentTime==0.0){
        return
    }
    if(mainWindow.currentTime>1.0){
        mainWindow.currentTime=mainWindow.currentTime-1.0
    }
    else{
        mainWindow.currentTime=0.0
    }
    videoSlide.value=mainWindow.currentTime
    videoArea.seek(mainWindow.currentTime)
}
function backFiveSeconds(){
    if(mainWindow.currentTime==0.0){
        return
    }
    if(mainWindow.currentTime>5.0){
        mainWindow.currentTime=mainWindow.currentTime-5.0
    }
    else{
        mainWindow.currentTime=0.0
    }
    videoSlide.value=mainWindow.currentTime
    videoArea.seek(mainWindow.currentTime)
}
function volumnUp(){
    if(mainWindow.volumn<0.9){
        mainWindow.volumn=mainWindow.volumn+0.1
        mainWindow.beforeMute=mainWindow.volumn
        volumnSlider.value=mainWindow.volumn*100
    }
    else{
        mainWindow.volumn=1
        mainWindow.beforeMute=1
        volumnSlider.value=100
    }
    mainWindow.volumnChange(mainWindow.volumn)
    videoArea.setVolume(mainWindow.volumn)
}
function volumnDown(){
    if(mainWindow.volumn<0.1){
        mainWindow.volumn=0
        mainWindow.beforeMute=0
        volumnSlider.value=0
    }
    else{
        mainWindow.volumn=mainWindow.volumn-0.1
        mainWindow.beforeMute=mainWindow.volumn
        volumnSlider.value=mainWindow.volumn*100
    }
    mainWindow.volumnChange(mainWindow.volumn)
    videoArea.setVolume(mainWindow.volumn)
}
function volumeSliderOnMoved(){
    mainWindow.volumn=volumnSlider.value/100
    mainWindow.beforeMute=volumnSlider.value/100
    mainWindow.volumnChange(mainWindow.volumn)
    videoArea.setVolume(mainWindow.volumn)
}
function speakerOnClicked(){
    if(mainWindow.volumn===0){
        mainWindow.volumn=mainWindow.beforeMute
        volumnSlider.value=Math.floor(mainWindow.volumn*100)
    }
    else{
        mainWindow.beforeMute=mainWindow.volumn
        mainWindow.volumn=0
        volumnSlider.value=0
    }
    mainWindow.volumnChange(mainWindow.volumn)
    videoArea.setVolume(mainWindow.volumn)
}
function playModeOnClicked(){
    if(mainWindow.playState==="ordered"){
        mainWindow.playState="single"
    }
    else if(mainWindow.playState==="single"){
        mainWindow.playState="random"
    }
    else{
        mainWindow.playState="ordered"
    }
    mainWindow.playModeChange(playState)
}
function invertedOnClicked(){
    if(mainWindow.isInverted){
        mainWindow.isInverted=false
        videoArea.forward();
    }
    else{
        mainWindow.isInverted=true
        videoArea.backward();
    }
    //mainWindow.inverted(mainWindow.step)
}
function fileListOnClicked(){
    if(mainWindow.isVideoListOpen){
        mainWindow.isVideoListOpen=false
    }
    else{
        mainWindow.isVideoListOpen=true
    }
}
function videoSlideDistance(flag){
    let tmp
    if(flag){tmp=Math.floor(mainWindow.currentTime)}
    else{tmp=Math.floor(mainWindow.endTime-mainWindow.currentTime)}
    if(tmp<60){
        return tmp+''
    }
    else if(tmp>=60&&tmp<3600){
        let tal=tmp%60
        let mid=Math.floor(tmp/60)
        if(tal<10){
            tal='0'+tal
        }
        return mid+':'+tal
    }
    else{
        let tal=tmp%60
        let had=Math.floor(tmp/3600)
        let mid=(Math.floor(tmp/60))%60
        if(tal<10){
            tal='0'+tal
        }
        if(mid<10){
            mid='0'+mid
        }
        return had+':'+mid+':'+tal
    }
}
function videoAreaOnClicked(){
    if(mainWindow.isPlay){
        mainWindow.isPlay=false
        mainWindow.stop()
    }
    else{
        mainWindow.isPlay=true
        mainWindow.start()
    }
}
function mainAreaInit(){
    mainWindow.start.connect(videoArea.start)
    mainWindow.stop.connect(videoArea.pause)
    mainWindow.openFile.connect(videoArea.openFile)
    mainWindow.setSpeed.connect(videoArea.setSpeed)
}
function videoListOperatorOnAccepted(){
    mainWindow.openFile(fileDialog.currentFile);
    mainWindow.endTime=Math.floor(videoArea.getVideoDuration())
    let selectedFileName = fileDialog.currentFile.toString().substring(
        fileDialog.currentFolder.toString().length + 1)
    mediaLibController.getFile(selectedFileName, fileDialog.currentFile);
    listModel.append({"fileName":selectedFileName,"filePath":fileDialog.currentFile,"iconPath":"interfacepics/defaultlogo"})
    console.log(videoArea.getVideoDuration())
}




function isBoundary(){
    //左边界
    if(mainWindow.isInverted&&(mainWindow.currentTime<=0)){
        toVideoBegining()
        operationFailedDialogText.text="已到达开头，无法继续倒放"
        operationFailedDialog.open()
        return true
    }
    //右边界
    else if(!mainWindow.isInverted&&(mainWindow.currentTime>=mainWindow.endTime)){
        toVideoEnd()
        nextOnClicked()
        toVideoBegining()
        return true
    }
    return false
}
function timerOnTriggered(){
    if(mainWindow.isInverted){
        mainWindow.currentTime=mainWindow.currentTime-1.0
    }
    else {
        mainWindow.currentTime=mainWindow.currentTime+1.0
    }
    if(!isBoundary()){
        videoSlide.value=mainWindow.currentTime
    }
}
function toVideoBegining(){
    mainWindow.isPlay=false
    mainWindow.currentTime=0
    // videoSlide.value=0
    mainWindow.wakeSlide()
    mainWindow.lastStep=0.0
}
function toVideoEnd(){
    mainWindow.isPlay=false
    mainWindow.currentTime=mainWindow.endTime
    videoSlide.value=mainWindow.endTime
}
function toPause(){
    toVideoBegining()
    mainWindow.cease()
    mainWindow.stop()
    videoArea.seek(0)
}
function playOrPauseFunction(){
    if(!mainWindow.isPlay){
        if((mainWindow.endTime!==0.0)&&!isBoundary()){
            mainWindow.isPlay=true
            mainWindow.start()
        }
    }
    else{
        mainWindow.isPlay=false
        mainWindow.stop()
    }
}
function solveStateChanged(){
    if(videoArea.state==1){
        toVideoBegining()
    }
    else if(videoArea.state==2){
        mainWindow.endTime=0
        toVideoBegining()
        return
    }
    else if(videoArea.state==4){
        mainWindow.isPlay=true
    }
    else if(videoArea.state==6){
        mainWindow.isPlay=false
        IF.makeTrackMenu()
    }
    // mainWindow.endTime=Math.floor(videoArea.getVideoDuration())
    //let tmp=videoArea.getPTS()
    //mainWindow.currentTime=tmp
    //videoSlide.value=tmp
}
function nextOnClicked(){
    console.log("playState:",mainWindow.playState)
    if(mainWindow.playState === "ordered")
        listview.currentIndex = (listview.currentIndex + 1)%listview.count

    else if(mainWindow.playState === "random")
        listview.currentIndex = (listview.currentIndex + Math.floor(Math.random()*listview.count))%listview.count
    else;
    console.log("index:",listview.currentIndex)
    mainWindow.openFile(listModel.get(listview.currentIndex).filePath);
    mainWindow.endTime=Math.floor(videoArea.getVideoDuration())
}
function makeTrackMenu(){
    if(mainWindow.trackMenu){
        mainWindow.trackMenu.destroy()
    }
    var tmpList=videoArea.getTracks()
    console.log(tmpList)
    mainWindow.trackMenu = Qt.createQmlObject('import QtQuick 2.13; import QtQuick.Controls 2.13; Menu{}',menu)
    mainWindow.trackMenu.title="testtrack"
    menu.addItem(mainWindow.trackMenu)
    let component=Qt.createComponent("TrackItem.qml")
    for(let i=0;i<tmpList.length;i++){
        let item = component.createObject(mainWindow.trackMenu,{"text":tmpList[i],"trackID":i})
        item.setTrack.connect(videoArea.setTrack)
        trackmenu.addItem(item)
    }
}
function hideComponents(){
    mainWindow.isVideoListOpen=false
    mainWindow.isFooterVisible=false
    mainWindow.isTopBarVisible=false
    mainWindow.mouseFlag=true
}
function showComponents(){
    holder.restart()
    mainWindow.isFooterVisible=true
    mainWindow.isTopBarVisible=true
}
function screenSizeFunction(){
    if(mainWindow.isFullScreen){
        mainWindow.isFullScreen=false
        mainWindow.showNormal()
        showComponents()
        holder.stop()
    }
    else{
        mainWindow.isFullScreen=true
        mainWindow.isVideoListOpen=false
        mainWindow.showFullScreen()
        holder.start()
    }
}