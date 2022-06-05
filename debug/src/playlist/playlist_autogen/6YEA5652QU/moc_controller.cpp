/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/playlist/include/controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Controller_t {
    const uint offsetsAndSize[74];
    char stringdata0[495];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Controller_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Controller_t qt_meta_stringdata_Controller = {
    {
QT_MOC_LITERAL(0, 10), // "Controller"
QT_MOC_LITERAL(11, 10), // "insertItem"
QT_MOC_LITERAL(22, 0), // ""
QT_MOC_LITERAL(23, 13), // "PlayListItem*"
QT_MOC_LITERAL(37, 4), // "item"
QT_MOC_LITERAL(42, 10), // "removeItem"
QT_MOC_LITERAL(53, 10), // "searchItem"
QT_MOC_LITERAL(64, 10), // "primaryKey"
QT_MOC_LITERAL(75, 18), // "extractRequirement"
QT_MOC_LITERAL(94, 18), // "finishExtractItems"
QT_MOC_LITERAL(113, 17), // "removeRequirement"
QT_MOC_LITERAL(131, 4), // "path"
QT_MOC_LITERAL(136, 18), // "getInfoRequirement"
QT_MOC_LITERAL(155, 13), // "finishGetInfo"
QT_MOC_LITERAL(169, 18), // "recentFilesChanged"
QT_MOC_LITERAL(188, 12), // "getInsertRst"
QT_MOC_LITERAL(201, 10), // "resultCode"
QT_MOC_LITERAL(212, 12), // "getRemoveRst"
QT_MOC_LITERAL(225, 12), // "getSearchRst"
QT_MOC_LITERAL(238, 10), // "resultItem"
QT_MOC_LITERAL(249, 13), // "getExtractRst"
QT_MOC_LITERAL(263, 22), // "QList<simpleListItem*>"
QT_MOC_LITERAL(286, 3), // "rst"
QT_MOC_LITERAL(290, 10), // "getInfoRst"
QT_MOC_LITERAL(301, 7), // "getFile"
QT_MOC_LITERAL(309, 8), // "filename"
QT_MOC_LITERAL(318, 22), // "sendExtractRequirement"
QT_MOC_LITERAL(341, 21), // "sendRemoveRequirement"
QT_MOC_LITERAL(363, 8), // "filepath"
QT_MOC_LITERAL(372, 8), // "iconPath"
QT_MOC_LITERAL(381, 22), // "sendGetInfoRequirement"
QT_MOC_LITERAL(404, 21), // "getSimpleListItemList"
QT_MOC_LITERAL(426, 14), // "getRecentFiles"
QT_MOC_LITERAL(441, 16), // "updateRecentFile"
QT_MOC_LITERAL(458, 8), // "filePath"
QT_MOC_LITERAL(467, 15), // "getListItemInfo"
QT_MOC_LITERAL(483, 11) // "recentFiles"

    },
    "Controller\0insertItem\0\0PlayListItem*\0"
    "item\0removeItem\0searchItem\0primaryKey\0"
    "extractRequirement\0finishExtractItems\0"
    "removeRequirement\0path\0getInfoRequirement\0"
    "finishGetInfo\0recentFilesChanged\0"
    "getInsertRst\0resultCode\0getRemoveRst\0"
    "getSearchRst\0resultItem\0getExtractRst\0"
    "QList<simpleListItem*>\0rst\0getInfoRst\0"
    "getFile\0filename\0sendExtractRequirement\0"
    "sendRemoveRequirement\0filepath\0iconPath\0"
    "sendGetInfoRequirement\0getSimpleListItemList\0"
    "getRecentFiles\0updateRecentFile\0"
    "filePath\0getListItemInfo\0recentFiles"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Controller[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       1,  200, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  146,    2, 0x06,    2 /* Public */,
       5,    1,  149,    2, 0x06,    4 /* Public */,
       6,    1,  152,    2, 0x06,    6 /* Public */,
       8,    0,  155,    2, 0x06,    8 /* Public */,
       9,    0,  156,    2, 0x06,    9 /* Public */,
      10,    1,  157,    2, 0x06,   10 /* Public */,
      12,    1,  160,    2, 0x06,   12 /* Public */,
      13,    0,  163,    2, 0x06,   14 /* Public */,
      14,    0,  164,    2, 0x06,   15 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    1,  165,    2, 0x0a,   16 /* Public */,
      17,    1,  168,    2, 0x0a,   18 /* Public */,
      18,    1,  171,    2, 0x0a,   20 /* Public */,
      20,    1,  174,    2, 0x0a,   22 /* Public */,
      23,    1,  177,    2, 0x0a,   24 /* Public */,
      24,    2,  180,    2, 0x0a,   26 /* Public */,
      26,    0,  185,    2, 0x0a,   29 /* Public */,
      27,    2,  186,    2, 0x0a,   30 /* Public */,
      30,    1,  191,    2, 0x0a,   33 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      31,    0,  194,    2, 0x02,   35 /* Public */,
      32,    0,  195,    2, 0x02,   36 /* Public */,
      33,    1,  196,    2, 0x02,   37 /* Public */,
      35,    0,  199,    2, 0x02,   39 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, 0x80000000 | 3,   19,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 3,   22,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,   25,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   28,   29,
    QMetaType::Void, QMetaType::QString,   28,

 // methods: parameters
    QMetaType::QVariantList,
    QMetaType::QVariantList,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::QVariantMap,

 // properties: name, type, flags
      36, QMetaType::QVariantList, 0x00015001, uint(8), 0,

       0        // eod
};

void Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Controller *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->insertItem((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 1: _t->removeItem((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 2: _t->searchItem((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->extractRequirement(); break;
        case 4: _t->finishExtractItems(); break;
        case 5: _t->removeRequirement((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->getInfoRequirement((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->finishGetInfo(); break;
        case 8: _t->recentFilesChanged(); break;
        case 9: _t->getInsertRst((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->getRemoveRst((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->getSearchRst((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 12: _t->getExtractRst((*reinterpret_cast< std::add_pointer_t<QList<simpleListItem*>>>(_a[1]))); break;
        case 13: _t->getInfoRst((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 14: { QString _r = _t->getFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->sendExtractRequirement(); break;
        case 16: _t->sendRemoveRequirement((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 17: _t->sendGetInfoRequirement((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: { QVariantList _r = _t->getSimpleListItemList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 19: { QVariantList _r = _t->getRecentFiles();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->updateRecentFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: { QVariantMap _r = _t->getListItemInfo();
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PlayListItem* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PlayListItem* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PlayListItem* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<simpleListItem*> >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PlayListItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Controller::*)(PlayListItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::insertItem)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Controller::*)(PlayListItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::removeItem)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Controller::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::searchItem)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Controller::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::extractRequirement)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Controller::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::finishExtractItems)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Controller::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::removeRequirement)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Controller::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::getInfoRequirement)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Controller::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::finishGetInfo)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Controller::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::recentFilesChanged)) {
                *result = 8;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Controller *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVariantList*>(_v) = _t->getRecentFiles(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Controller::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Controller.offsetsAndSize,
    qt_meta_data_Controller,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Controller_t
, QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>, QtPrivate::TypeAndForceComplete<Controller, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QList<simpleListItem*>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>, QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>

>,
    nullptr
} };


const QMetaObject *Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Controller.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Controller::insertItem(PlayListItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Controller::removeItem(PlayListItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Controller::searchItem(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Controller::extractRequirement()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Controller::finishExtractItems()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Controller::removeRequirement(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Controller::getInfoRequirement(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Controller::finishGetInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Controller::recentFilesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
