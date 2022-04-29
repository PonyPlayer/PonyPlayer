/****************************************************************************
** Meta object code from reading C++ file 'playlist.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../include/playlist.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playlist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlayListItem_t {
    const uint offsetsAndSize[24];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_PlayListItem_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_PlayListItem_t qt_meta_stringdata_PlayListItem = {
    {
QT_MOC_LITERAL(0, 12), // "PlayListItem"
QT_MOC_LITERAL(13, 11), // "getFileName"
QT_MOC_LITERAL(25, 0), // ""
QT_MOC_LITERAL(26, 11), // "setFileName"
QT_MOC_LITERAL(38, 9), // "_fileName"
QT_MOC_LITERAL(48, 12), // "getDirectory"
QT_MOC_LITERAL(61, 12), // "setDirectory"
QT_MOC_LITERAL(74, 4), // "QDir"
QT_MOC_LITERAL(79, 4), // "_dir"
QT_MOC_LITERAL(84, 17), // "setDirectoryByStr"
QT_MOC_LITERAL(102, 6), // "_uuid_"
QT_MOC_LITERAL(109, 3) // "dir"

    },
    "PlayListItem\0getFileName\0\0setFileName\0"
    "_fileName\0getDirectory\0setDirectory\0"
    "QDir\0_dir\0setDirectoryByStr\0_uuid_\0"
    "dir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlayListItem[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       2,   61, // properties
       0,    0, // enums/sets
       2,   71, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x02,    3 /* Public */,
       3,    1,   45,    2, 0x02,    4 /* Public */,
       5,    0,   48,    2, 0x02,    6 /* Public */,
       6,    1,   49,    2, 0x02,    7 /* Public */,
       9,    1,   52,    2, 0x02,    9 /* Public */,

 // methods: parameters
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::QString,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString,    8,

 // constructors: parameters
    0x80000000 | 2, QMetaType::QString, 0x80000000 | 7,    4,    8,
    0x80000000 | 2,

 // properties: name, type, flags
      10, QMetaType::QString, 0x00015003, uint(-1), 0,
      11, QMetaType::QString, 0x00015003, uint(-1), 0,

 // constructors: name, argc, parameters, tag, flags, initial metatype offsets
       0,    2,   55,    2, 0x0e,   11 /* Public */,
       0,    0,   60,    2, 0x0e,   13 /* Public */,

       0        // eod
};

void PlayListItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { PlayListItem *_r = new PlayListItem((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDir>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 1: { PlayListItem *_r = new PlayListItem();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    } else if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PlayListItem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { QString _r = _t->getFileName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->setFileName((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: { QString _r = _t->getDirectory();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->setDirectory((*reinterpret_cast< std::add_pointer_t<QDir>>(_a[1]))); break;
        case 4: _t->setDirectoryByStr((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<PlayListItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getUUID(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getDirectory(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<PlayListItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setUUID(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setDirectoryByStr(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject PlayListItem::staticMetaObject = { {
    QMetaObject::SuperData::link<ListItem::staticMetaObject>(),
    qt_meta_stringdata_PlayListItem.offsetsAndSize,
    qt_meta_data_PlayListItem,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_PlayListItem_t
, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<PlayListItem, std::true_type>

, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QDir, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<const QDir &, std::false_type>
>,
    nullptr
} };


const QMetaObject *PlayListItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlayListItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlayListItem.stringdata0))
        return static_cast<void*>(this);
    return ListItem::qt_metacast(_clname);
}

int PlayListItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ListItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
struct qt_meta_stringdata_PlayList_t {
    const uint offsetsAndSize[24];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_PlayList_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_PlayList_t qt_meta_stringdata_PlayList = {
    {
QT_MOC_LITERAL(0, 8), // "PlayList"
QT_MOC_LITERAL(9, 10), // "insertDone"
QT_MOC_LITERAL(20, 0), // ""
QT_MOC_LITERAL(21, 10), // "resultcode"
QT_MOC_LITERAL(32, 10), // "removeDone"
QT_MOC_LITERAL(43, 10), // "searchDone"
QT_MOC_LITERAL(54, 13), // "PlayListItem*"
QT_MOC_LITERAL(68, 4), // "item"
QT_MOC_LITERAL(73, 6), // "insert"
QT_MOC_LITERAL(80, 6), // "remove"
QT_MOC_LITERAL(87, 6), // "search"
QT_MOC_LITERAL(94, 3) // "key"

    },
    "PlayList\0insertDone\0\0resultcode\0"
    "removeDone\0searchDone\0PlayListItem*\0"
    "item\0insert\0remove\0search\0key"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlayList[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       4,    1,   53,    2, 0x06,    3 /* Public */,
       5,    1,   56,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    1,   59,    2, 0x0a,    7 /* Public */,
       9,    1,   62,    2, 0x0a,    9 /* Public */,
      10,    1,   65,    2, 0x0a,   11 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    0x80000000 | 6, QMetaType::QString,   11,

       0        // eod
};

void PlayList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PlayList *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->insertDone((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->removeDone((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->searchDone((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 3: _t->insert((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 4: _t->remove((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 5: { PlayListItem* _r = _t->search((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< PlayListItem**>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PlayListItem* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PlayListItem* >(); break;
            }
            break;
        case 4:
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
            using _t = void (PlayList::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayList::insertDone)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PlayList::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayList::removeDone)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PlayList::*)(PlayListItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayList::searchDone)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject PlayList::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PlayList.offsetsAndSize,
    qt_meta_data_PlayList,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_PlayList_t
, QtPrivate::TypeAndForceComplete<PlayList, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>


>,
    nullptr
} };


const QMetaObject *PlayList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlayList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlayList.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PlayList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void PlayList::insertDone(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PlayList::removeDone(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PlayList::searchDone(PlayListItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
