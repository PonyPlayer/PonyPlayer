#pragma once

#include <QtCore>
#include <utility>
#include "kv_engine.h"


class PlayListItem : public ListItem {
Q_OBJECT
    Q_PROPERTY(QString _uuid_ READ getUUID WRITE setUUID)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName)
    Q_PROPERTY(QString dir READ getDirectory WRITE setDirectoryByStr)
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
public:
//    QList<ListItem> getData();

    void insert(ListItem item);

    explicit PlayList(const QString &name);
};

Q_DECLARE_METATYPE(PlayListItem*)

