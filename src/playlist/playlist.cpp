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


PlayList::PlayList(const QString &name) {
    qRegisterMetaType<PlayListItem *>("PlayListItem");

    PonyKVList<PlayListItem> list(name, QString("playlist"), QString("PlayListItem"));
    auto *item = new PlayListItem("233", QDir("/usr/bin"));

    list.insert(item);

//    qDebug() << data[0]->property("dir");
}
