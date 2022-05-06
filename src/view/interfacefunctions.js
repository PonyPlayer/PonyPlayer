
function mytest(path){
    console.log(path)
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

function playOrPauseFunction(){
    console.log(videoArea.getVideoDuration())
    if(!mainWindow.isPlay){
        if(mainWindow.endTime!==0.0){
            mainWindow.isPlay=true
            if(mainWindow.currentTime===mainWindow.endTime){
                mainWindow.currentTime=0.0
                videoSlide.value=0.0
            }
        }
        mainWindow.start()
    }
    else{
        mainWindow.isPlay=false
        mainWindow.stop()
    }
}
function solveStateChanged(){
    if(videoArea.state==1){
        console.log("lei fu kai state == loading")
        videoInit()
    }
    else if(videoArea.state==2){
        console.log("lei fu kai state == invalid")
        mainWindow.endTime=0
        videoInit()
        return
    }
    mainWindow.endTime=Math.floor(videoArea.getVideoDuration())
    let tmp=videoArea.getPTS()
    mainWindow.currentTime=tmp
    videoSlide.value=tmp

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
function screenSizeFunction(){
    mainWindow.isVideoListOpen=false
    if(mainWindow.isFullScreen){
        mainWindow.showNormal()
        mainWindow.isFullScreen=false
        mainWindow.isFooterVisable=true
    }
    else{
        mainWindow.isFullScreen=true
        mainWindow.visibility=showFullScreen()
    }
}
function toPause(){
    mainWindow.cease()
    mainWindow.stop()
    videoInit()
    videoArea.seek(0)
}
function openWave(){
    wavewindow.show()
}
function videoInit(){
    mainWindow.isPlay=false
    mainWindow.currentTime=0
    videoSlide.value=0
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
    if(playMode.state==="ordered"){
        playMode.state="single"
    }
    else if(playMode.state==="single"){
        playMode.state="random"
    }
    else{
        playMode.state="ordered"
    }
    mainWindow.playModeChange(playMode.state)
}
function videoSpeedOnClicked(){
    if(videoSpeed.state==="speed1"){
        videoSpeed.state="speed2"
        mainWindow.speed=2.0
    }
    else if(videoSpeed.state==="speed2"){
        videoSpeed.state="speed4"
        mainWindow.speed=4.0
    }
    else if(videoSpeed.state==="speed4"){
        videoSpeed.state="speed8"
        mainWindow.speed=8.0
    }
    else{
        videoSpeed.state="speed1"
        mainWindow.speed=1.0
    }
    mainWindow.setSpeed(mainWindow.speed)
}
function invertedOnClicked(){
    if(mainWindow.isInverted){
        mainWindow.isInverted=false
    }
    else{
        mainWindow.isInverted=true
    }
    //mainWindow.inverted(mainWindow.step)
}
function videoSlideOnValueChanged(){
    if(mainWindow.currentTime<0){
        mainWindow.currentTime=0
        videoSlide.value=0
    }
    else if(mainWindow.currentTime>=mainWindow.endTime){
        mainWindow.currentTime=0
        videoSlide.value=0
        mainWindow.isPlay=false
        mainWindow.stop()
        console.log("lei fu kai use seek")
        videoArea.seek(0)
    }
    mainWindow.currentTime=videoSlide.value
    mainWindow.currentTimeChange(videoSlide.value)
}
function fileListOnClicked(){
    if(mainWindow.isVideoListOpen){
        mainWindow.isVideoListOpen=false
    }
    else{
        mainWindow.isVideoListOpen=true
    }
}
function timerOnTriggered(){
    if(mainWindow.currentTime<=0.0&&mainWindow.isInverted){
        mainWindow.isPlay=false
    }
    else if(mainWindow.currentTime>=mainWindow.endTime&&!mainWindow.isInverted){
        mainWindow.isPlay=false
    }
    else{
        mainWindow.currentTime=mainWindow.currentTime+1.0
        videoSlide.value=mainWindow.currentTime
    }
    console.log("time trigger")
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
function videoAreaOnVolumeChangedFail(){
    console.log("volume fail" + current)
    mainWindow.volumn=current
    mainWindow.beforeMute=current
    volumnSlide.value=current*100
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
    mediaLibController.getFile(fileDialog.currentFile);
    console.log(videoArea.getVideoDuration())
}