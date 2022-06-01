videoSlider:
---
back5(使用快捷键):    改变current，再用currrnt改变value(到达边界时停留在边界)

back1(使用快捷键):    ^

front5(使用快捷键):    ^

front1(使用快捷键):    ^

playOrPauseFunction(点击播放或暂停):    当到达endtime时，将current和value设为0

solveStateChange(hurricane状态变化时):    获取pos，将current和value设为pos

videoinit:    设为暂停，将current和value设为0

toPause(触发停止):    使用init，seek(0)

videoSlideOnValueChanged(播放条的val改变时):    到达边界时使用videoinit，将current设为value

timerOnTriggered(计时器触发):    到达边界时暂停，否则current++，value=current
