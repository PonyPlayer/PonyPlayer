//
// Created by 彭郑威 on 2022/4/29.
//

#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    PlayList *listOPer = new PlayList("MediaLib","MeidaInfo","PlayListItem");
    // 调用 moveToThread 将该任务交给 listOPThread
    listOPer->moveToThread(&listOPThread);

    // Controller 的信号和 PonyKVList 处理者进行绑定
    connect(this, SIGNAL(insertItem(PlayListItem*)), listOPer, SLOT(insert(PlayListItem*)));
    connect(this, SIGNAL(removeItem(PlayListItem*)), listOPer, SLOT(remove(PlayListItem*)));
    connect(this, SIGNAL(searchItem(QString)), listOPer, SLOT(search(QString)));

    // 该线程结束时销毁
    connect(&listOPThread, &QThread::finished, listOPer, &QObject::deleteLater);

    // 线程结束后发送信号，对结果进行处理
    connect(listOPer, SIGNAL(insertDone(int)), this, SLOT(getInsertRst(int)));
    connect(listOPer, SIGNAL(removeDone(int)), this, SLOT(getRemoveRst(int)));
    connect(listOPer, SIGNAL(searchDone(PlayListItem*)), this, SLOT(getSearchRst(PlayListItem*)));

    //启动线程
    listOPThread.start();
    //发射信号，开始执行
    qDebug()<<"emit the signal to execute!---------------------------------";
    qDebug()<<"     current thread ID:"<<QThread::currentThreadId()<<'\n';
}

Controller::~Controller()
{
    listOPThread.quit();
    listOPThread.wait();
}