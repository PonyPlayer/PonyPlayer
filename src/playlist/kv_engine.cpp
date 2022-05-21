#include "kv_engine.h"
#include <QMetaType>
#include <QtSql/QtSql>
#include <string>
#include <sstream>
#include <iomanip>
#include "playlist.h"
#include <QtGlobal>
#include <stdlib.h>
#ifdef Q_OS_MAC
#define PLATFORM "MAC"
#endif
#ifdef Q_OS_LINUX
#define PLATFORM "LINUX"
#endif
#ifdef Q_OS_WIN32
#define PLATFORM "WINDOWS"
#endif

/*
 * 在当前目录下新建 data 子目录，其中创建数据库
 * @dnName: 数据库名
 */
PonyKVConnect::PonyKVConnect(const QString &dbName) {
    QString home;
    if(PLATFORM=="MAC") {
        home = qEnvironmentVariable("HOME");
        home += "/Library/Containers";
    }
    if(PLATFORM=="WINDOWS") {
        home = qEnvironmentVariable("HOME");
        home += "/AppData/Local";
    }
    if(PLATFORM=="LINUX") {
        home = qEnvironmentVariable("HOME");
    }



//    QDir dataBasePath = QDir::currentPath();

    home += "/PonyPlayer";
    bool done = qputenv("PONYPATH", home.toUtf8());
    if(done)
        qDebug()<<"环境变量设置成功!"<<home;
    else
        qDebug()<<"环境变量设置失败!"<<home;

    QDir dir_check;
    if(!dir_check.exists(home))
        dir_check.mkpath(home);
    QDir dataBasePath(home);
    dataBasePath.mkpath("data");
    dataBasePath.mkpath("preview");

    QString currentOpenFile = dataBasePath.absoluteFilePath("recentOpenFiles.txt");
    QFile newfile(currentOpenFile);
    if (!newfile.exists()) {
        newfile.open(QIODevice::WriteOnly);
        newfile.close();
    }

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
                                                               {"QDir",    "text"},
                                                               {"float",   "float"},
                                                               {"int",     "int"}};
    return lookUpTable[qType];
}

/*
 * 向表中插入一个对象
 * @tableName: 表名
 * @object: 被插入对象
 */
void PonyKVConnect::insert(const QString &tableName, const QObject *object) {

    const QMetaObject *metaObj = object->metaObject();

    if (!metaObj)
        qDebug() << "metaObj is NULL";
    else
        qDebug() << "metaObj is not NULL";

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

void PonyKVConnect::removeByKV(const QString &tableName, const QString &key, const QString &value) {
    PlayListItem* pli = search<PlayListItem>(tableName,"PlayListItem",key, value);
    QUrl url(pli->getIconPath());
    QString iconPath = url.toLocalFile();
    QFileInfo FileInfo(iconPath);
    if (FileInfo.isFile()) {
        QFile::remove(iconPath);
        qDebug()<<"delete iconPath:"<<iconPath;
    }
    QSqlQuery query(db);
    query.prepare("DELETE FROM `" + tableName + "` WHERE " + key + "= :value");
    query.bindValue(0, value);
    query.exec();
}

/*
 * 向数据库查询
 * @tableName: 表名
 * @key: 键
 * @value: 值
 */
template<typename T>
T* PonyKVConnect::search(const QString &tableName, const QString &className, const QString &key, const QString &value) {
    const QMetaObject *metaObj = QMetaType::fromName(className.toUtf8()).metaObject();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM `" + tableName + "` WHERE "+ key + "= :value");
    query.bindValue(0,value);

    query.exec();
    QObject *obj = metaObj->newInstance();
    while(query.next()) {
        qDebug()<<"have";
        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            obj->setProperty(query.record().fieldName(i - metaObj->propertyOffset()).toUtf8(),
                             query.record().value(i - metaObj->propertyOffset()));
        }
        break;
    }
    return dynamic_cast<T *>(obj);
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
void PonyKVList<T>::remove(QString key,QString value) {
    engine.removeByKV(tableName,key,value);
}

template<typename T>
QList<T*> PonyKVList<T>::extract() {
    // data = engine.retrieveData<T>(tableName,className);
    return data;
}

template<typename T>
T* PonyKVList<T>::extractInfo(QString key,QString value) {
    return engine.search<T>(tableName,className,key,value);
}

template
class PonyKVList<PlayListItem>;