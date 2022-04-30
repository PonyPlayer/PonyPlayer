//
// Created by 彭郑威 on 2022/4/29.
//

#ifndef PONYPLAYER_CONTROLLER_H
#define PONYPLAYER_CONTROLLER_H
#include <QObject>
#include <QThread>
#include <QDebug>
#include "playlist.h"

// 对数据库的操作码
enum MLOPCODE {
    INSERTTO,
    REMOVEFROM,
    SEARCHFROM
};

class Controller: public QObject {
    Q_OBJECT
    QThread listOPThread;

public:
    explicit Controller(QObject *parent= nullptr);
    ~Controller();

public slots:
    void getInsertRst(const int resultCode)
    {
        if(resultCode == 0)
            qDebug()<<"Insert to MediaLib Success!\n";
        else
            qDebug()<<"Insert to MediaLib Fail!\n";
    }

    void getRemoveRst(const int resultCode)
    {
        if(resultCode == 0)
            qDebug()<<"Remove from MediaLib Success!\n";
        else
            qDebug()<<"Remove from MediaLib Fail!\n";
    }

    void getSearchRst(const PlayListItem* resultItem)
    {
        if(!resultItem)
            qDebug()<<"Search Find!\n";
        else
            qDebug()<<"Search Fail!\n";
    }

    void getFile(QString filename) {
        qDebug()<<filename;
    }

signals:
    //发送信号触发线程
    void insertItem(PlayListItem *item);
    void removItem(PlayListItem *item);
    void searchItem(QString primaryKey);
};

#endif //PONYPLAYER_CONTROLLER_H
