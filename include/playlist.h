#pragma once

#include <QtCore>
#include <utility>
#include "kv_engine.h"
#include <QThread>
#include <QMetaType>

class PlayListItem : public ListItem {
    Q_OBJECT
    Q_PROPERTY(QString _uuid_ READ getUUID WRITE setUUID)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName)    // 文件名
    Q_PROPERTY(QString dir READ getDirectory WRITE setDirectoryByStr)
    Q_PROPERTY(QString duration READ getDuration WRITE setDuration)  // 视频时长
    Q_PROPERTY(int frameRate READ getFrameRate WRITE setFrameRate)  // 视频帧率
    Q_PROPERTY(int bitRate READ getBitRate WRITE setBitRate)  // 视频码率
    Q_PROPERTY(float videoSize READ getVideoSize WRITE setVideoSize)  // 视频流大小
    Q_PROPERTY(int videoWidth READ getVideoWidth WRITE setVideoWidth)  // 视频宽度
    Q_PROPERTY(int videoHeight READ getVideoHeight WRITE setVideoHeight)  // 视频高度
    Q_PROPERTY(QString videoFormat READ getVideoFormat WRITE setVideoFormat)  // 视频编码格式
    Q_PROPERTY(QString audioFormat READ getAudioFormat WRITE setAudioFormat)  // 音频编码格式
    Q_PROPERTY(int audioAverageBitRate READ getAudioAverageBitRate WRITE setAudioAverageBitRate)  // 音频平均码率
    Q_PROPERTY(int channelNumbers READ getChannelNumbers WRITE setChannelNumbers)  // 音频通道数
    Q_PROPERTY(int sampleRate READ getSampleRate WRITE setSampleRate)  // 音频采样率
    Q_PROPERTY(float audioSize READ getAudioSize WRITE setAudioSize)  // 音频大小
    Q_PROPERTY(QString format READ getFormat WRITE setFormat)  // 封装格式
    Q_PROPERTY(QString path READ getPath WRITE setPath)  // 路径


protected:
    QString fileName;
    QDir dir;
    QString path;
    QString duration;
    int frameRate;
    int bitRate;
    float videoSize;
    int videoWidth;
    int videoHeight;
    QString videoFormat;
    QString audioFormat;
    int audioAverageBitRate;
    int channelNumbers;
    int sampleRate;
    float audioSize;
    int streamNumbers;
    QString format;

public:
    Q_INVOKABLE PlayListItem(QString _fileName, const QDir &_dir)
            : ListItem(), fileName(std::move(_fileName)), dir(_dir) {
    };

    Q_INVOKABLE PlayListItem() = default;

    Q_INVOKABLE QString getFileName();
    Q_INVOKABLE void setFileName(QString _fileName) { fileName = _fileName; }

    Q_INVOKABLE QString getPath() { return path; }
    Q_INVOKABLE void setPath(QString _path) { path = _path; }

    Q_INVOKABLE QString getDirectory();
    Q_INVOKABLE void setDirectory(QDir _dir) { dir = _dir; }
    Q_INVOKABLE void setDirectoryByStr(QString _dir) { dir = _dir; }

    Q_INVOKABLE QString getDuration() { return duration; }
    Q_INVOKABLE void setDuration(QString _duration) { duration = _duration; }

    Q_INVOKABLE int getFrameRate() { return frameRate; }
    Q_INVOKABLE void setFrameRate(int _frameRate) { frameRate = _frameRate; }

    Q_INVOKABLE int getBitRate() { return bitRate; }
    Q_INVOKABLE void setBitRate(int _bitRate) { bitRate = _bitRate; }

    Q_INVOKABLE float getVideoSize() { return videoSize; }
    Q_INVOKABLE void setVideoSize(float _videoSize) { videoSize = _videoSize; }

    Q_INVOKABLE int getVideoWidth() { return videoWidth; }
    Q_INVOKABLE void setVideoWidth(int _videoWidth) { videoWidth = _videoWidth; }

    Q_INVOKABLE int getVideoHeight() { return videoHeight; }
    Q_INVOKABLE void setVideoHeight(int _videoHeight) { videoHeight = _videoHeight; }

    Q_INVOKABLE QString getVideoFormat() { return videoFormat; }
    Q_INVOKABLE void setVideoFormat(QString _videoFormat) { videoFormat = _videoFormat; }

    Q_INVOKABLE QString getAudioFormat() { return audioFormat; }
    Q_INVOKABLE void setAudioFormat(QString _audioFormat) { audioFormat = _audioFormat; }

    Q_INVOKABLE int getAudioAverageBitRate() { return audioAverageBitRate; }
    Q_INVOKABLE void setAudioAverageBitRate(int _audioAverageBitRate) { audioAverageBitRate = _audioAverageBitRate; }

    Q_INVOKABLE int getChannelNumbers() { return channelNumbers; }
    Q_INVOKABLE void setChannelNumbers(int _channelNumbers) { channelNumbers = _channelNumbers; }

    Q_INVOKABLE int getSampleRate() { return sampleRate; }
    Q_INVOKABLE void setSampleRate(int _sampleRate) { sampleRate = _sampleRate; }

    Q_INVOKABLE QString getFormat() { return format; }
    Q_INVOKABLE void setFormat(QString _format) { format = _format; }

    Q_INVOKABLE float getAudioSize() { return audioSize; }
    Q_INVOKABLE void setAudioSize(float _audioSize) { audioSize = _audioSize; }

    Q_INVOKABLE int getStreamNumbers() { return streamNumbers; }
    Q_INVOKABLE void setStreamNumbers(int _streamNumbers) { streamNumbers = _streamNumbers; }

    Q_INVOKABLE ~PlayListItem() override = default;

//    Q_INVOKABLE PlayListItem(const PlayListItem &listItem);
};

/*
 * 用于播放列表左侧条目简单信息
 */
class simpleListItem: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName)
    Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath)
    Q_PROPERTY(QString iconPath READ getIconPath WRITE setIconPath)

public:
    Q_INVOKABLE simpleListItem() = default;

    Q_INVOKABLE simpleListItem(QString _fileName, QString _filePath, QString _iconPath)
            : fileName(_fileName), filePath(_filePath), iconPath(_iconPath) { };

    Q_INVOKABLE QString getFileName() { return fileName; }
    Q_INVOKABLE void setFileName(QString _fileName) { fileName = _fileName; }

    Q_INVOKABLE QString getFilePath() { return filePath; }
    Q_INVOKABLE void setFilePath(QString _filePath) { filePath = _filePath; }

    Q_INVOKABLE QString getIconPath() { return iconPath; }
    Q_INVOKABLE void setIconPath(QString _iconPath) { iconPath = _iconPath; }

    Q_INVOKABLE ~simpleListItem() = default;

    Q_INVOKABLE simpleListItem(const simpleListItem& other) {
        fileName = other.fileName;
        filePath = other.filePath;
        iconPath = other.iconPath;
    }



private:
    QString fileName;
    QString filePath;
    QString iconPath;
};
Q_DECLARE_METATYPE(simpleListItem);

class PlayList : public QObject {
    Q_OBJECT
//    Q_PROPERTY(QQmlListProperty<QObject*> playListItems READ playListItems)

private:
    QList<QObject *> data;
    PonyKVList<PlayListItem> pkvList;

//    static void appendPlayList(QQmlListProperty<QObject*>* list,QObject *qobj);
//    static int playListCount(QQmlListProperty<QObject*>*) const;
//    static QObject* listItem(QQmlListProperty<QObject*>*,int) const;
//    static void clearPlayList(QQmlListProperty<QObject*>*);
public:
//    QList<ListItem> getData();
    PlayList(QString _dbName, QString _tableName, QString _className);
//    QQmlListProperty<QObject*> playListItems();

//    void appendPlayList(QObject *qobj);
//    int playListCount() const;
//    QObject* listItem(int) const;
//    void clearPlayList();

//    void insert(ListItem item);

    explicit PlayList(const QString &name);

public slots:
    void insert(PlayListItem *item);
    void remove(QString filepath);
    PlayListItem* search(QString key);
    void extractAndProcess();
    void getInfo(QString path);

signals:
    void insertDone(int resultcode);
    void removeDone(int resultcode);
    void searchDone(PlayListItem *item);
    void extractDone(QList<simpleListItem*> res);
    void getInfoDone(PlayListItem *item);
};

Q_DECLARE_METATYPE(PlayListItem*)