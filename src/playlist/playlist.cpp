#include "playlist.h"

QString PlayListItem::getFileName() {
    return fileName;
}

QString PlayListItem::getDirectory() {
    return dir.path();
}

//PlayListItem::PlayListItem(const PlayListItem &item) : ListItem(item) {
//    fileName = item.fileName;
//    dir = item.dir;
//}

PlayList::PlayList(QString _dbName, QString _tableName, QString _className):pkvList(_dbName, _tableName, _className) {
    qDebug()<<"PlayList init!\n";
}

//PlayList::QQmlListProperty<QObject*> playListItems() {
//
//}
//
//void PlayList::appendPlayList(QObject *qobj) {
//    data.append(qobj);
//}
//
//int PlayList::playListCount() const {
//    return data.count();
//}
//
//QObject* PlayList::listItem(int index) const {
//    return data.at(index);
//}
//
//void PlayList::clearPlayList() {
//    data.clear();
//}
//
//void PlayList::appendPlayList(QQmlListProperty<QObject*>* list,QObject *qobj) {
//    reinterpret_cast<QObject*>(list->data)->appendPlayList(qobj);
//}
//
//int PlayList::playListCount(QQmlListProperty<QObject*>* list) const {
//    return reinterpret_cast<QObject*>(list->data)->playListCount();
//}
//
//QObject* PlayList::listItem(QQmlListProperty<QObject*>* list,int i) const {
//    return reinterpret_cast<QObject*>(list->data)->listItem(i);
//}
//
//void PlayList::clearPlayList(QQmlListProperty<QObject*>* list) {
//    reinterpret_cast<QObject*>(list->data)->clearPlayList();
//}

//PlayList::PlayList(const QString &name) {
//    qRegisterMetaType<PlayListItem *>("PlayListItem");
//
//    PonyKVList<PlayListItem> list(name, QString("playlist"), QString("PlayListItem"));
//    auto *item = new PlayListItem("233", QDir("/usr/bin"));
//
//    list.insert(item);
//
////    qDebug() << data[0]->property("dir");
//}

void PlayList::insert(PlayListItem *item){
    qDebug()<<"PlayList is going to insert a PlayListItem.";
    if(!item)
        qDebug()<<"But this item is NULL which means it was not received correctly.";
    else
        qDebug()<<"And this item was received correctly.";
    pkvList.insert(item);
    qDebug()<<"insert to db done!";
}

void PlayList::remove(QString filepath){
    qDebug()<<"remove";
    pkvList.remove("path",filepath);
    qDebug()<<"remove "<<filepath<<" done!";
}

PlayListItem* PlayList::search(QString key){
    qDebug()<<"search";
    return NULL;
}

void PlayList::getInfo(QString path) {
    qDebug()<<"getInfo";
    PlayListItem* res = pkvList.extractInfo("path",path);
    emit getInfoDone(res);
}

/*
 * 提取数据库信息并处理为 simpleListItem 对象，然后向 Controller 发送处理完毕信号
 */
void PlayList::extractAndProcess() {
    QList<PlayListItem*> rce = pkvList.extract();

    QList<simpleListItem*> res;
    simpleListItem* slItem;
    foreach(PlayListItem* item, rce) {
        slItem = new simpleListItem(item->getFileName(),item->getPath(),"");
        res.append(slItem);
    }
    emit extractDone(res);
}