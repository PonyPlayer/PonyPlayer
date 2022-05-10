//
// Created by 彭郑威 on 2022/4/29.
//

#ifndef PONYPLAYER_CONTROLLER_H
#define PONYPLAYER_CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "playlist.h"
#include "info_accessor.h"

class Controller : public QObject {
    Q_OBJECT
            QThread listOPThread;

private:
    QList<simpleListItem*> result;
    PlayListItem* playListItemResult;

public:
    explicit Controller(QObject *parent = nullptr);

    ~Controller();

    Q_INVOKABLE QVariantList getSimpleListItemList();
    Q_INVOKABLE QVariantMap getListItemInfo();

public slots:

    void getInsertRst(int resultCode) {
        if (resultCode == 0)
            qDebug() << "Insert to MediaLib Success!\n";
        else
            qDebug() << "Insert to MediaLib Fail!\n";
    }

    void getRemoveRst(int resultCode) {
        if (resultCode == 0)
            qDebug() << "Remove from MediaLib Success!\n";
        else
            qDebug() << "Remove from MediaLib Fail!\n";
    }

    void getSearchRst(PlayListItem *resultItem) {
        if (!resultItem)
            qDebug() << "Search Find!\n";
        else
            qDebug() << "Search Fail!\n";
    }

    void getExtractRst(QList<simpleListItem*> rst) {
        result.clear();
        for(int i=0;i<rst.size();i++) {
            result.append(rst[i]);
        }
        emit finishExtractItems();
    }

    void getInfoRst(PlayListItem* rst) {
        playListItemResult = rst;
        emit finishGetInfo();
    }

    void getFile(QString filename, QString path) {
        PlayListItem *info = new PlayListItem;
        info->setFileName(filename);
        info->setPath(path);
        infoAccessor::getInfo(path, *info);
        qDebug() << "文件:" << info->getFileName();
        qDebug() << "路径:" << info->getPath();
        qDebug() << "帧率:" << info->getFrameRate();
        qDebug() << "比特率:" << info->getBitRate();
        qDebug() << "视频流大小:" << info->getVideoSize();
        qDebug() << "画面尺寸:" << info->getVideoWidth() << "*" << info->getVideoHeight();
        qDebug() << "视频格式:" << info->getVideoFormat();
        qDebug() << "音频格式:" << info->getAudioFormat();
        qDebug() << "音频平均比特率:" << info->getAudioAverageBitRate();
        qDebug() << "音频通道数:" << info->getChannelNumbers();
        qDebug() << "音频采样率:" << info->getSampleRate();
        qDebug() << "音频流大小:" << info->getAudioSize();
        qDebug() << "流数量:" << info->getStreamNumbers();
        emit insertItem(info);
    }

    void sendExtractRequirement() { emit extractRequirement(); }
    void sendRemoveRequirement(QString filepath) { emit removeRequirement(filepath); }
    void sendGetInfoRequirement(QString filepath) { emit getInfoRequirement(filepath); }

signals:
    //发送信号触发线程
    void insertItem(PlayListItem *item);

    void removeItem(PlayListItem *item);

    void searchItem(QString primaryKey);

    void extractRequirement();   // 向 PlayList 发送的提取请求

    void finishExtractItems();   // 向 qml 发送的提取完毕的信号

    void removeRequirement(QString path);   // 向 PlayList 发送的删除请求

    void getInfoRequirement(QString path);  // 向 PLayList 发送获取信息请求

    void finishGetInfo();  // 向 qml 发送查找完毕的信号
};

#endif //PONYPLAYER_CONTROLLER_H