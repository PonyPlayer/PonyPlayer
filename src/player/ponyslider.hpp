//#pragma once
//
//#include <QStylePainter>
//#include <QStyleOptionSlider>
//#include <QStyleOptionComplex>
//
//#include <QColor>
//#include <cmath>
//#include <QQuickItem>
//#include <QtQuick>
//
//class PonySlider : public QQuickItem {
//public:
//    explicit PonySlider(Qt::Orientation orientation, QQuickItem *parent = nullptr) : QSlider(orientation, parent) {};
//
//    explicit PonySlider(QWidget *parent = nullptr) : QSlider(parent) {
//        this->setStyleSheet("\
//                            QSlider::groove:horizontal {\
//                                height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */ \
//                                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\
//                                margin: 2px 0;\
//                            }\
//                            \
//                            QSlider::handle:horizontal {\
//                                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);\
//                                border: 1px solid #5c5c5c;\
//                                width: 18px;\
//                                margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */ \
//                                border-radius: 3px;\
//                            }\
//                        ");
//    };
//protected:
//    virtual void paintEvent(QPaintEvent *ev) {
//        QStylePainter p(this);
//        QStyleOptionSlider opt;
//        initStyleOption(&opt);
//
//        QRect handle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
//
//        // draw tick marks
//        // do this manually because they are very badly behaved with style sheets
//        int interval = tickInterval();
//        if (interval == 0) {
//            interval = pageStep();
//        }
//
//        if (tickPosition() != NoTicks) {
//            for (int i = minimum(); i <= maximum(); i += interval) {
//                int x = std::round((double) (
//                        (double) ((double) (i - this->minimum()) / (double) (this->maximum() - this->minimum())) *
//                        (double) (this->width() - handle.width()) + (double) (handle.width() / 2.0))) - 1;
//                int h = 4;
//                p.setPen(QColor("#a5a294"));
//                if (tickPosition() == TicksBothSides || tickPosition() == TicksAbove) {
//                    int y = this->rect().top();
//                    p.drawLine(x, y, x, y + h);
//                }
//                if (tickPosition() == TicksBothSides || tickPosition() == TicksBelow) {
//                    int y = this->rect().bottom();
//                    p.drawLine(x, y, x, y - h);
//                }
//            }
//        }
//
//        QSlider::paintEvent(ev);
//    }
//};