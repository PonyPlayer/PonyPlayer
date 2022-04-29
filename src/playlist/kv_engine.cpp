#include "kv_engine.h"
#include <QMetaType>
#include <QtSql/QtSql>
#include <string>
#include <sstream>
#include <iomanip>
#include "playlist.h"

/*
 * 在当前目录下新建 data 子目录，其中创建数据库
 * @dnName: 数据库名
 */
PonyKVConnect::PonyKVConnect(const QString &dbName) {
    QDir dataBasePath = QDir::currentPath();
    dataBasePath.mkpath("data");
    dataBasePath.cd("data");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dataBasePath.filePath(dbName));
    if (!db.open()) throw std::runtime_error("Can not open database!");
}

bool PonyKVConnect::hasTable(const QString &tableName) {
    return db.tables().contains(tableName);
}

/*
 * 创建类对象表，列是类属性，主键是对象 id
 * @className: 类名
 * @tableName: 表名
 */
void PonyKVConnect::createTableFrom(const QString &className, const QString &tableName) {
    qInfo() << "Creating new table for class " << className;

    const QMetaObject *metaObj = QMetaType::fromName(className.toUtf8()).metaObject();

    if (!metaObj) throw std::runtime_error("Can not createTableFrom!");

    QString tableDDL = "CREATE TABLE `" + tableName + "` (";
    for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
        if (i != metaObj->propertyOffset()) tableDDL += ", ";
        tableDDL += metaObj->property(i).name();
        tableDDL += " ";
        tableDDL += qTypeToDDL(metaObj->property(i).typeName());
        if (!strcmp(metaObj->property(i).name(), "__uuid__")) tableDDL += "PRIMARY KEY";
    }
    tableDDL += ");";
    qDebug() << tableDDL;
    db.exec(tableDDL);
}

/*
 *
 */
QString PonyKVConnect::qTypeToDDL(const QString &qType) {
    static std::unordered_map<QString, QString> lookUpTable = {{"QString", "text"},
                                                               {"QDir",    "text"}};
    return lookUpTable[qType];
}

/*
 * 向表中插入一个对象
 * @tableName: 表名
 * @object: 被插入对象
 */
void PonyKVConnect::insert(const QString &tableName, const QObject *object) {
    const QMetaObject *metaObj = object->metaObject();
    QString sql = "INSERT INTO `" + tableName + "` VALUES (";
    for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
        if (i != metaObj->propertyOffset()) sql += ",";
        std::stringstream stringstream;
        stringstream << std::quoted(object->property(metaObj->property(i).name()).value<QString>().toStdString(),
                                    '\'', '\\');
        sql += QString::fromStdString(stringstream.str());
    }
    sql += ")";
    qDebug() << sql;
    db.exec(sql);
}

/*
 * 获取表中所有对象
 * @tableName: 表名
 * @className: 类名(主要用户获取各种属性字段，即表的列)
 * @return: 对象指针列表
 */
QList<QObject *> PonyKVConnect::retrieveData(const QString &tableName, const QString &className) {
    QList<QObject *> ret;

    const QMetaObject *metaObj = QMetaType::fromName(className.toUtf8()).metaObject();

    QSqlQuery query("SELECT * FROM `" + tableName + "`", db);

    while (query.next()) {
        QObject *obj = metaObj->newInstance();
        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            obj->setProperty(query.record().fieldName(i - metaObj->propertyOffset()).toUtf8(),
                             query.record().value(i - metaObj->propertyOffset()));
        }
        ret.push_back(obj);
    }

    return ret;
}

template<typename T>
QList<T *> PonyKVConnect::retrieveDataByClass(const QString &tableName, const QString &className) {
    QList<T *> ret;

    const QMetaObject *metaObj = QMetaType::fromName(className.toUtf8()).metaObject();

    QSqlQuery query("SELECT * FROM `" + tableName + "`", db);

    while (query.next()) {
        QObject *obj = metaObj->newInstance();
        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            obj->setProperty(query.record().fieldName(i - metaObj->propertyOffset()).toUtf8(),
                             query.record().value(i - metaObj->propertyOffset()));
        }
        ret.push_back(dynamic_cast<T *>(obj));
    }

    return ret;
}

/*
 * 从表中移除对象
 * @tableName: 表名
 * @object: 被移除对象
 */
void PonyKVConnect::remove(const QString &tableName, const QObject *object) {
    QString sql = "DELETE FROM `" + tableName + "` WHERE _uuid_ = ";
    sql += object->property("_uuid_").value<QString>();
    db.exec(sql);
}

template<typename T>
PonyKVList<T>::PonyKVList(QString _dbName, QString _tableName, QString _className) :engine(_dbName),
                                                                                    dbName(std::move(_dbName)),
                                                                                    tableName(
                                                                                            std::move(_tableName)),
                                                                                    className(std::move(_className)) {
    if (!engine.hasTable(tableName)) {
        engine.createTableFrom(className, tableName);
    }
    data = engine.retrieveDataByClass<T>(tableName, className);
}

template<typename T>
void PonyKVList<T>::insert(T *item) {
    data.push_back(item);
    engine.insert(tableName, item);
}

template<typename T>
void PonyKVList<T>::remove(T *item) {
//    engine.remove(item);
}

template
class PonyKVList<PlayListItem>;