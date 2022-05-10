#pragma once

#include <QtCore>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QDir>
#include <unordered_map>
#include <QUuid>


class PonyKVConnect {
private:
    QSqlDatabase db;
public:
    explicit PonyKVConnect(const QString &dbName);

    bool hasTable(const QString &tableName);

    void createTableFrom(const QString &className, const QString &tableName);

    static QString qTypeToDDL(const QString &qType);

    void insert(const QString &tableName, const QObject *object);

    void remove(const QString &tableName, const QObject *object);

    void removeByKV(const QString &tableName, const QString &key, const QString &value);

    template<typename T>
    T* search(const QString &tableName, const QString &className, const QString &key, const QString &value);

    QList<QObject *> retrieveData(const QString &tableName, const QString &className);

    template<typename T>
    QList<T *> retrieveDataByClass(const QString &tableName, const QString &className);
};


class ListItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString _uuid_ READ getUUID WRITE setUUID)
protected:
    QString _uuid_;
public:
    Q_INVOKABLE ListItem() { _uuid_ = QUuid::createUuid().toString(); };

    Q_INVOKABLE void setUUID(QString uuid) { _uuid_ = std::move(uuid); };

    Q_INVOKABLE QString getUUID() { return _uuid_; };

    Q_INVOKABLE ~ListItem() override = default;

//    Q_INVOKABLE ListItem(const ListItem &listItem) : QObject() { _uuid_ = listItem._uuid_; };
};

template<typename T>
class PonyKVList {
private:
    PonyKVConnect engine;
    QString dbName;
    QString tableName;
    QString className;
    QList<T *> data;
    T* infoData;
public:
    PonyKVList(QString _dbName, QString _tableName, QString _className);

    QString getTableName() { return tableName; }

    void insert(T *item);

    void remove(QString key,QString value);

    T* extractInfo(QString key,QString value);

    QList<T*> extract();

};