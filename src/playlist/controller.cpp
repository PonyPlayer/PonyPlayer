//
// Created by 彭郑威 on 2022/4/29.
//

#include "controller.h"


Controller::Controller(QObject *parent) : QObject(parent)
{
    PlayList *listOPer = new PlayList("MediaLib","MediaInfo","PlayListItem");
    // 调用 moveToThread 将该任务交给 listOPThread
    listOPer->moveToThread(&listOPThread);

    // Controller 的信号和 PonyKVList 处理者进行绑定
    connect(this, SIGNAL(insertItem(PlayListItem*)), listOPer, SLOT(insert(PlayListItem*)));
    connect(this, SIGNAL(removeItem(PlayListItem*)), listOPer, SLOT(remove(PlayListItem*)));
    connect(this, SIGNAL(searchItem(QString)), listOPer, SLOT(search(QString)));
    connect(this, SIGNAL(extractRequirement()), listOPer, SLOT(extractAndProcess()));
    connect(this, SIGNAL(removeRequirement(QString)), listOPer, SLOT(remove(QString)));
    connect(this, SIGNAL(getInfoRequirement(QString)), listOPer, SLOT(getInfo(QString)));

    // 该线程结束时销毁
    connect(&listOPThread, &QThread::finished, listOPer, &QObject::deleteLater);

    // 线程结束后发送信号，对结果进行处理
    connect(listOPer, SIGNAL(insertDone(int)), this, SLOT(getInsertRst(int)));
    connect(listOPer, SIGNAL(removeDone(int)), this, SLOT(getRemoveRst(int)));
    connect(listOPer, SIGNAL(searchDone(PlayListItem*)), this, SLOT(getSearchRst(PlayListItem*)));
    connect(listOPer, SIGNAL(extractDone(QList<simpleListItem*>)), this, SLOT(getExtractRst(QList<simpleListItem*>)));
    connect(listOPer, SIGNAL(getInfoDone(PlayListItem*)), this, SLOT(getInfoRst(PlayListItem*)));
    //启动线程
    listOPThread.start();
    //发射信号，开始执行
    qDebug()<<"-------------- MediaLib Thread Start! ID:"<<QThread::currentThreadId()<<"--------------\n";
}

Controller::~Controller()
{
    listOPThread.quit();
    listOPThread.wait();
    qDebug()<<"-------------- MediaLib Thread Finish! --------------\n";
}

QVariantList Controller::getSimpleListItemList() {
    QVariantList res;
    for(int i=0;i<result.size();i++){
        res.append(QVariant::fromValue(result[i]));
    }

    return res;
}

/*
 * 获取最近打开的文件
 */
QVariantList Controller::getRecentFiles() {
    QVariantList res;
    QString source = QDir::currentPath() + "/recentOpenFiles.txt";
    QFile file(source);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<< "Can't open the file!";
    }

    QTextStream in(&file);
    while (true) {
        QString line = in.readLine();
        if (line.isNull())
            break;
        QStringList parts = line.split(",");
        QVariantList temp;
        for(int i=0;i<parts.size();i++)
            temp.append(QVariant::fromValue<QString>(parts[i]));
        res.append(QVariant::fromValue<QVariantList>(temp));
    }
    file.close();
    return res;
}

/*
 * 写入最近打开的文件
 */
void Controller::updateRecentFile(QString filePath) {
    QUrl url(filePath);
    QString fileName = QFileInfo(url.path()).fileName();  // 文件名
    QString source = QDir::currentPath() + "/recentOpenFiles.txt";
    QFile file(source);
    QStringList readyWrite;  // 将要写的内容
    readyWrite.append(fileName+","+filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<< "Can't open the file!";
    }

    QTextStream in(&file);
    int count=0;
    while (true) {
        QString line = in.readLine();
        if (line.isNull())
            break;
        if(line==readyWrite[0])
            continue;
        else {
            if(count<9) {
                readyWrite.append(line);
                count += 1;
            }
            else
                break;
        }
    }
    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
        qDebug()<< "Can't open the file!";
    }
    QTextStream out(&file);
    for(int i=0;i<readyWrite.size();i++) {
        out<<readyWrite[i]<<"\n";
    }
    file.close();
}


QVariantMap Controller::getListItemInfo() {
    QVariantMap res;
    qDebug()<<playListItemResult->getVideoWidth()+"*"+playListItemResult->getVideoHeight();
    res["文件名"] = QVariant::fromValue(playListItemResult->getFileName());
    res["文件路径"] = QVariant::fromValue(playListItemResult->getPath());
    res["时长"] = QVariant::fromValue(playListItemResult->getDuration());
    res["封装格式"] = QVariant::fromValue(playListItemResult->getFormat());
    res["视频帧率"] = QVariant::fromValue(QString::number(playListItemResult->getFrameRate()));
    res["视频码率"] = QVariant::fromValue(QString::number(playListItemResult->getBitRate()));
    res["视频流大小"] = QVariant::fromValue(QString::number(playListItemResult->getVideoSize()));
    res["视频尺寸"] = QVariant::fromValue(QString::number(playListItemResult->getVideoWidth())+"*"+QString::number(playListItemResult->getVideoHeight()));
    res["视频编码格式"] = QVariant::fromValue(playListItemResult->getVideoFormat());
    res["音频编码格式"] = QVariant::fromValue(playListItemResult->getAudioFormat());
    res["音频平均码率"] = QVariant::fromValue(QString::number(playListItemResult->getAudioAverageBitRate()));
    res["音频通道数"] = QVariant::fromValue(QString::number(playListItemResult->getChannelNumbers()));
    res["音频采样率"] = QVariant::fromValue(QString::number(playListItemResult->getSampleRate()));
    res["音频流大小"] = QVariant::fromValue(QString::number(playListItemResult->getAudioSize()));
    return res;
}