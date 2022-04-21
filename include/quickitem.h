//
// Created by ColorsWind on 2022/4/21.
//

#ifndef PONYPLAYER_QUICKITEM_H
#define PONYPLAYER_QUICKITEM_H
#include "hurricane.h"

class HurricaneState : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    /**
     * 视频是否有效, 仅当有效时下面属性才有意义
     */
    bool isValid;

    /**
     * 文件名
     */
    QString filename;

    /**
     * 视频的帧率
     */
    qreal fps;

    /**
     * 文件格式
     */
    QString format;
};
class HurricanePlayer : public Hurricane {
    Q_OBJECT
    QML_ELEMENT
//    Q_PROPERTY(HurricaneState state NOTIFY stateChanged)
public:
    HurricaneState state;
signals:
    /**
     * 状态发生改变
     */
    void stateChanged();

    /**
     * 视频播放完
     */
     void playFinished();

public slots:
    /**
     * 打开视频文件
     * @param path
     */
    Q_INVOKABLE void openFile(const QString &path);

    /**
     * 开始播放视频
     */
    Q_INVOKABLE void start();

    /**
     * 停止播放视频
     */
    Q_INVOKABLE void stop();

    /**
     * 设置视频倍速
     */
    Q_INVOKABLE void setSpeed(qreal speed);


    /**
     * 暂停播放视频
     */
    Q_INVOKABLE void pause();

    /**
     * 将视频播放进度移动到指定位置
     * 注：这个操作不会改变视频的播放状态
     */
    Q_INVOKABLE void seek(qreal sec);
};



#endif //PONYPLAYER_QUICKITEM_H
