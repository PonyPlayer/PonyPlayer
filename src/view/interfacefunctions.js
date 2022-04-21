
function mytest(path){
    console.log(path)
}
function forwardOneSecond(){
    if(mainWindow.isPlay){
        mainWindow.isPlay=false
    }
    if(mainWindow.endTime==0){
        return
    }
    if(mainWindow.endTime>mainWindow.currentTime){
        mainWindow.currentTime=mainWindow.currentTime+1
    }
    else{
        mainWindow.currentTime=mainWindow.endTime
    }
    videoSlide.value=mainWindow.currentTime
}
function forwardFiveSeconds(){
    if(mainWindow.isPlay){
        mainWindow.isPlay=false
    }

    if(mainWindow.endTime==0){
        return
    }
    if((mainWindow.endTime-mainWindow.currentTime)>5){
        mainWindow.currentTime=mainWindow.currentTime+5
    }
    else{
        mainWindow.currentTime=mainWindow.endTime
    }
    videoSlide.value=mainWindow.currentTime
}
function backOneSecond(){
    if(mainWindow.isPlay){
        mainWindow.isPlay=false
    }
    if(mainWindow.currentTime==0){
        return
    }
    if(mainWindow.currentTime>1){
        mainWindow.currentTime=mainWindow.currentTime-1
    }
    else{
        mainWindow.currentTime=0
    }
    videoSlide.value=mainWindow.currentTime
}
function backFiveSeconds(){
    if(mainWindow.isPlay){
        mainWindow.isPlay=false
    }
    if(mainWindow.currentTime==0){
        return
    }
    if(mainWindow.currentTime>5){
        mainWindow.currentTime=mainWindow.currentTime-5
    }
    else{
        mainWindow.currentTime=0
    }
    videoSlide.value=mainWindow.currentTime
}

function playOrPauseFunction(){
    if(!mainWindow.isPlay){
        if(mainWindow.endTime!==0){
            mainWindow.isPlay=true
            if(mainWindow.currentTime===mainWindow.endTime){
                videoSlide.value=0
                mainWindow.currentTime=0
            }
        }
        mainWindow.start()
    }
    else{
        mainWindow.isPlay=false
        mainWindow.stop()
    }
}

function volumnUp(){
    if(volumnSlider.value<90){
        volumnSlider.value=volumnSlider.value+10
        mainWindow.volumn=volumnSlider.value
        mainWindow.beforeMute=volumnSlider.value
        mainWindow.volumnChange(volumnSlider.value)
    }
    else{
        mainWindow.volumn=100
        mainWindow.beforeMute=100
        volumnSlider.value=100
        mainWindow.volumnChange(volumnSlider.value)
    }
}
function volumnDown(){
    if(volumnSlider.value<10){
        mainWindow.volumn=0
        mainWindow.beforeMute=0
        volumnSlider.value=0
        mainWindow.volumnChange(volumnSlider.value)
    }
    else{
        volumnSlider.value=volumnSlider.value-10
        mainWindow.volumn=volumnSlider.value
        mainWindow.beforeMute=volumnSlider.value
        mainWindow.volumnChange(volumnSlider.value)
    }
}
function screenSizeFunction(){
    if(mainWindow.isFullScreen){
        mainWindow.showNormal()
        mainWindow.isFullScreen=false
    }
    else{
        mainWindow.isFullScreen=true
        mainWindow.visibility=showFullScreen()
    }
}


