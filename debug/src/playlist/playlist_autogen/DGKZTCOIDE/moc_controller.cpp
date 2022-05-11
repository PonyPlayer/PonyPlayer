/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../include/controller.h"
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
    const uint offsetsAndSize[62];
    char stringdata0[414];
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
QT_MOC_LITERAL(169, 12), // "getInsertRst"
QT_MOC_LITERAL(182, 10), // "resultCode"
QT_MOC_LITERAL(193, 12), // "getRemoveRst"
QT_MOC_LITERAL(206, 12), // "getSearchRst"
QT_MOC_LITERAL(219, 10), // "resultItem"
QT_MOC_LITERAL(230, 13), // "getExtractRst"
QT_MOC_LITERAL(244, 22), // "QList<simpleListItem*>"
QT_MOC_LITERAL(267, 3), // "rst"
QT_MOC_LITERAL(271, 10), // "getInfoRst"
QT_MOC_LITERAL(282, 7), // "getFile"
QT_MOC_LITERAL(290, 8), // "filename"
QT_MOC_LITERAL(299, 22), // "sendExtractRequirement"
QT_MOC_LITERAL(322, 21), // "sendRemoveRequirement"
QT_MOC_LITERAL(344, 8), // "filepath"
QT_MOC_LITERAL(353, 22), // "sendGetInfoRequirement"
QT_MOC_LITERAL(376, 21), // "getSimpleListItemList"
QT_MOC_LITERAL(398, 15) // "getListItemInfo"

    },
    "Controller\0insertItem\0\0PlayListItem*\0"
    "item\0removeItem\0searchItem\0primaryKey\0"
    "extractRequirement\0finishExtractItems\0"
    "removeRequirement\0path\0getInfoRequirement\0"
    "finishGetInfo\0getInsertRst\0resultCode\0"
    "getRemoveRst\0getSearchRst\0resultItem\0"
    "getExtractRst\0QList<simpleListItem*>\0"
    "rst\0getInfoRst\0getFile\0filename\0"
    "sendExtractRequirement\0sendRemoveRequirement\0"
    "filepath\0sendGetInfoRequirement\0"
    "getSimpleListItemList\0getListItemInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Controller[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  128,    2, 0x06,    1 /* Public */,
       5,    1,  131,    2, 0x06,    3 /* Public */,
       6,    1,  134,    2, 0x06,    5 /* Public */,
       8,    0,  137,    2, 0x06,    7 /* Public */,
       9,    0,  138,    2, 0x06,    8 /* Public */,
      10,    1,  139,    2, 0x06,    9 /* Public */,
      12,    1,  142,    2, 0x06,   11 /* Public */,
      13,    0,  145,    2, 0x06,   13 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    1,  146,    2, 0x0a,   14 /* Public */,
      16,    1,  149,    2, 0x0a,   16 /* Public */,
      17,    1,  152,    2, 0x0a,   18 /* Public */,
      19,    1,  155,    2, 0x0a,   20 /* Public */,
      22,    1,  158,    2, 0x0a,   22 /* Public */,
      23,    2,  161,    2, 0x0a,   24 /* Public */,
      25,    0,  166,    2, 0x0a,   27 /* Public */,
      26,    1,  167,    2, 0x0a,   28 /* Public */,
      28,    1,  170,    2, 0x0a,   30 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      29,    0,  173,    2, 0x02,   32 /* Public */,
      30,    0,  174,    2, 0x02,   33 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, 0x80000000 | 3,   18,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 3,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   24,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void, QMetaType::QString,   27,

 // methods: parameters
    QMetaType::QVariantList,
    QMetaType::QVariantMap,

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
        case 8: _t->getInsertRst((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->getRemoveRst((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->getSearchRst((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 11: _t->getExtractRst((*reinterpret_cast< std::add_pointer_t<QList<simpleListItem*>>>(_a[1]))); break;
        case 12: _t->getInfoRst((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 13: _t->getFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->sendExtractRequirement(); break;
        case 15: _t->sendRemoveRequirement((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->sendGetInfoRequirement((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: { QVariantList _r = _t->getSimpleListItemList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 18: { QVariantMap _r = _t->getListItemInfo();
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
        case 10:
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
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<simpleListItem*> >(); break;
            }
            break;
        case 12:
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
    }
}

const QMetaObject Controller::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Controller.offsetsAndSize,
    qt_meta_data_Controller,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Controller_t
, QtPrivate::TypeAndForceComplete<Controller, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QList<simpleListItem*>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>, QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>

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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
