#pragma once

#include <QtCore>
#include <utility>
#include "kv_engine.h"
#include <QThread>

class PlayListItem : public ListItem {
Q_OBJECT
    Q_PROPERTY(QString _uuid_ READ getUUID WRITE setUUID)
//    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName)    // 文件名
    Q_PROPERTY(QString dir READ getDirectory WRITE setDirectoryByStr)
//    Q_PROPERTY(QString duration READ getDuration WRITE setDuration)  // 视频时长
//    Q_PROPERTY(QString frameRate READ getFrameRate WRITE setFrameRate)  // 视频帧率
//    Q_PROPERTY(QString bitRate READ getBitRate WRITE setBitRate)  // 视频码率
//    Q_PROPERTY(QString videoSize READ getVideoSize WRITE setVideoSize)  // 视频尺寸(宽度*高度)
//    Q_PROPERTY(QString videoSize READ getVideoSize WRITE setVideoSize)  // 视频大小
//    Q_PROPERTY(QString videoFormat READ getVideoFormat WRITE setVideoFormat)  // 视频编码格式
//    Q_PROPERTY(QString audioAverageBitRate READ getAudioAverageBitRate WRITE setAudioAverageBitRate)  // 音频平均码率
//    Q_PROPERTY(QString channelNumbers READ getChannelNumbers WRITE setChannelNumbers)  // 音频通道数
//    Q_PROPERTY(QString sampleRate READ getSampleRate WRITE setSampleRate)  // 音频采样率
//    Q_PROPERTY(QString audioSize READ getAudioSize WRITE setAudioSize)  // 音频大小

protected:
    QString fileName;
    QDir dir;
public:
    Q_INVOKABLE PlayListItem(QString _fileName, const QDir &_dir)
            : ListItem(), fileName(std::move(_fileName)), dir(_dir) {
    };

    Q_INVOKABLE PlayListItem() = default;

    Q_INVOKABLE QString getFileName();

    Q_INVOKABLE void setFileName(QString _fileName) { fileName = _fileName; };

    Q_INVOKABLE QString getDirectory();

    Q_INVOKABLE void setDirectory(QDir _dir) { dir = _dir; };

    Q_INVOKABLE void setDirectoryByStr(QString _dir) { dir = _dir; };

    Q_INVOKABLE ~PlayListItem() override = default;

//    Q_INVOKABLE PlayListItem(const PlayListItem &listItem);
};


class PlayList : public QObject {
Q_OBJECT

private:
    QList<QObject *> data;
    PonyKVList<PlayListItem> pkvList;
public:
//    QList<ListItem> getData();
    PlayList(QString _dbName, QString _tableName, QString _className);

    void insert(ListItem item);

    explicit PlayList(const QString &name);

public slots:
    void insert(PlayListItem *item);
    void remove(PlayListItem *item);
    PlayListItem* search(QString key);

signals:
    void insertDone(int resultcode);
    void removeDone(int resultcode);
    void searchDone(PlayListItem *item);
};

Q_DECLARE_METATYPE(PlayListItem*)

