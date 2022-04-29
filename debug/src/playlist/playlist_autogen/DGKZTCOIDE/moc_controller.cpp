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
    const uint offsetsAndSize[32];
    char stringdata0[172];
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
QT_MOC_LITERAL(42, 9), // "removItem"
QT_MOC_LITERAL(52, 10), // "searchItem"
QT_MOC_LITERAL(63, 10), // "primaryKey"
QT_MOC_LITERAL(74, 12), // "getInsertRst"
QT_MOC_LITERAL(87, 10), // "resultCode"
QT_MOC_LITERAL(98, 12), // "getRemoveRst"
QT_MOC_LITERAL(111, 12), // "getSearchRst"
QT_MOC_LITERAL(124, 19), // "const PlayListItem*"
QT_MOC_LITERAL(144, 10), // "resultItem"
QT_MOC_LITERAL(155, 7), // "getFile"
QT_MOC_LITERAL(163, 8) // "filename"

    },
    "Controller\0insertItem\0\0PlayListItem*\0"
    "item\0removItem\0searchItem\0primaryKey\0"
    "getInsertRst\0resultCode\0getRemoveRst\0"
    "getSearchRst\0const PlayListItem*\0"
    "resultItem\0getFile\0filename"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Controller[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,
       5,    1,   59,    2, 0x06,    3 /* Public */,
       6,    1,   62,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    1,   65,    2, 0x0a,    7 /* Public */,
      10,    1,   68,    2, 0x0a,    9 /* Public */,
      11,    1,   71,    2, 0x0a,   11 /* Public */,
      14,    1,   74,    2, 0x0a,   13 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::QString,   15,

       0        // eod
};

void Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Controller *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->insertItem((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 1: _t->removItem((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 2: _t->searchItem((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->getInsertRst((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->getRemoveRst((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->getSearchRst((*reinterpret_cast< std::add_pointer_t<const PlayListItem*>>(_a[1]))); break;
        case 6: _t->getFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
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
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::removItem)) {
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
    }
}

const QMetaObject Controller::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Controller.offsetsAndSize,
    qt_meta_data_Controller,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Controller_t
, QtPrivate::TypeAndForceComplete<Controller, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>


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
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
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
void Controller::removItem(PlayListItem * _t1)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
