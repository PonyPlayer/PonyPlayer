/****************************************************************************
** Meta object code from reading C++ file 'dispatcher.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/decoder/private/dispatcher.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dispatcher.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DemuxDispatcherBase_t {
    const uint offsetsAndSize[2];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_DemuxDispatcherBase_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_DemuxDispatcherBase_t qt_meta_stringdata_DemuxDispatcherBase = {
    {
QT_MOC_LITERAL(0, 19) // "DemuxDispatcherBase"

    },
    "DemuxDispatcherBase"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DemuxDispatcherBase[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void DemuxDispatcherBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject DemuxDispatcherBase::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_DemuxDispatcherBase.offsetsAndSize,
    qt_meta_data_DemuxDispatcherBase,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_DemuxDispatcherBase_t
, QtPrivate::TypeAndForceComplete<DemuxDispatcherBase, std::true_type>



>,
    nullptr
} };


const QMetaObject *DemuxDispatcherBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DemuxDispatcherBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DemuxDispatcherBase.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DemuxDispatcherBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_DecodeDispatcher_t {
    const uint offsetsAndSize[8];
    char stringdata0[43];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_DecodeDispatcher_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_DecodeDispatcher_t qt_meta_stringdata_DecodeDispatcher = {
    {
QT_MOC_LITERAL(0, 16), // "DecodeDispatcher"
QT_MOC_LITERAL(17, 17), // "signalStartWorker"
QT_MOC_LITERAL(35, 0), // ""
QT_MOC_LITERAL(36, 6) // "onWork"

    },
    "DecodeDispatcher\0signalStartWorker\0\0"
    "onWork"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DecodeDispatcher[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   27,    2, 0x08,    2 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void DecodeDispatcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DecodeDispatcher *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalStartWorker(QPrivateSignal()); break;
        case 1: _t->onWork(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DecodeDispatcher::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DecodeDispatcher::signalStartWorker)) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject DecodeDispatcher::staticMetaObject = { {
    QMetaObject::SuperData::link<DemuxDispatcherBase::staticMetaObject>(),
    qt_meta_stringdata_DecodeDispatcher.offsetsAndSize,
    qt_meta_data_DecodeDispatcher,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_DecodeDispatcher_t
, QtPrivate::TypeAndForceComplete<DecodeDispatcher, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *DecodeDispatcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DecodeDispatcher::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DecodeDispatcher.stringdata0))
        return static_cast<void*>(this);
    return DemuxDispatcherBase::qt_metacast(_clname);
}

int DecodeDispatcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DemuxDispatcherBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DecodeDispatcher::signalStartWorker(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_ReverseDecodeDispatcher_t {
    const uint offsetsAndSize[8];
    char stringdata0[50];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ReverseDecodeDispatcher_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ReverseDecodeDispatcher_t qt_meta_stringdata_ReverseDecodeDispatcher = {
    {
QT_MOC_LITERAL(0, 23), // "ReverseDecodeDispatcher"
QT_MOC_LITERAL(24, 17), // "signalStartWorker"
QT_MOC_LITERAL(42, 0), // ""
QT_MOC_LITERAL(43, 6) // "onWork"

    },
    "ReverseDecodeDispatcher\0signalStartWorker\0"
    "\0onWork"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReverseDecodeDispatcher[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   27,    2, 0x08,    2 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ReverseDecodeDispatcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ReverseDecodeDispatcher *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalStartWorker(QPrivateSignal()); break;
        case 1: _t->onWork(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ReverseDecodeDispatcher::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReverseDecodeDispatcher::signalStartWorker)) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject ReverseDecodeDispatcher::staticMetaObject = { {
    QMetaObject::SuperData::link<DemuxDispatcherBase::staticMetaObject>(),
    qt_meta_stringdata_ReverseDecodeDispatcher.offsetsAndSize,
    qt_meta_data_ReverseDecodeDispatcher,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ReverseDecodeDispatcher_t
, QtPrivate::TypeAndForceComplete<ReverseDecodeDispatcher, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *ReverseDecodeDispatcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReverseDecodeDispatcher::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ReverseDecodeDispatcher.stringdata0))
        return static_cast<void*>(this);
    return DemuxDispatcherBase::qt_metacast(_clname);
}

int ReverseDecodeDispatcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DemuxDispatcherBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ReverseDecodeDispatcher::signalStartWorker(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_PonyPlayer_t {
    const uint offsetsAndSize[10];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_PonyPlayer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_PonyPlayer_t qt_meta_stringdata_PonyPlayer = {
    {
QT_MOC_LITERAL(0, 10), // "PonyPlayer"
QT_MOC_LITERAL(11, 18), // "OpenFileResultType"
QT_MOC_LITERAL(30, 6), // "FAILED"
QT_MOC_LITERAL(37, 5), // "VIDEO"
QT_MOC_LITERAL(43, 5) // "AUDIO"

    },
    "PonyPlayer\0OpenFileResultType\0FAILED\0"
    "VIDEO\0AUDIO"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PonyPlayer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       4,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    3,   19,

 // enum data: key, value
       2, uint(PonyPlayer::FAILED),
       3, uint(PonyPlayer::VIDEO),
       4, uint(PonyPlayer::AUDIO),

       0        // eod
};

const QMetaObject PonyPlayer::staticMetaObject = { {
    nullptr,
    qt_meta_stringdata_PonyPlayer.offsetsAndSize,
    qt_meta_data_PonyPlayer,
    nullptr,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_PonyPlayer_t
, QtPrivate::TypeAndForceComplete<void, std::true_type>



>,
    nullptr
} };

QT_WARNING_POP
QT_END_MOC_NAMESPACE
