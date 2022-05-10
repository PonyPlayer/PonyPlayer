#include "wave.h"
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>


WaveView::WaveView(QQuickItem *parent) : QQuickItem(parent) {
//    auto *timer = new QTimer(nullptr);
//    timer->setInterval(1000);
//    static float ratio = 0;
//    connect(timer, &QTimer::timeout, this, [this] {
//        ratio += 0.05F;
//        this->setProperty("radiusRatio", ratio);
//    });
//    timer->start();
    setFlag(ItemHasContents, true);
}
