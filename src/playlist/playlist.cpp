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
    qDebug()<<"insert";
}

void PlayList::remove(PlayListItem *item){
    qDebug()<<"remove";
}

PlayListItem* PlayList::search(QString key){
    qDebug()<<"search";
}