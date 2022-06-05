/****************************************************************************
** Meta object code from reading C++ file 'audiosink.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/audiosink/audiosink.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audiosink.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PonyAudioSink_t {
    const uint offsetsAndSize[22];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_PonyAudioSink_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_PonyAudioSink_t qt_meta_stringdata_PonyAudioSink = {
    {
QT_MOC_LITERAL(0, 13), // "PonyAudioSink"
QT_MOC_LITERAL(14, 12), // "stateChanged"
QT_MOC_LITERAL(27, 0), // ""
QT_MOC_LITERAL(28, 20), // "resourceInsufficient"
QT_MOC_LITERAL(49, 34), // "signalAudioOutputDeviceListCh..."
QT_MOC_LITERAL(84, 20), // "signalDeviceSwitched"
QT_MOC_LITERAL(105, 27), // "onAudioOutputDevicesChanged"
QT_MOC_LITERAL(133, 19), // "requestDeviceSwitch"
QT_MOC_LITERAL(153, 6), // "device"
QT_MOC_LITERAL(160, 22), // "getCurrentDeviceFormat"
QT_MOC_LITERAL(183, 15) // "PonyAudioFormat"

    },
    "PonyAudioSink\0stateChanged\0\0"
    "resourceInsufficient\0"
    "signalAudioOutputDeviceListChanged\0"
    "signalDeviceSwitched\0onAudioOutputDevicesChanged\0"
    "requestDeviceSwitch\0device\0"
    "getCurrentDeviceFormat\0PonyAudioFormat"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PonyAudioSink[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    1 /* Public */,
       3,    0,   57,    2, 0x06,    2 /* Public */,
       4,    0,   58,    2, 0x06,    3 /* Public */,
       5,    0,   59,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   60,    2, 0x0a,    5 /* Public */,
       7,    1,   61,    2, 0x0a,    6 /* Public */,
       9,    0,   64,    2, 0x0a,    8 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    0x80000000 | 10,

       0        // eod
};

void PonyAudioSink::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PonyAudioSink *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stateChanged(); break;
        case 1: _t->resourceInsufficient(); break;
        case 2: _t->signalAudioOutputDeviceListChanged(); break;
        case 3: _t->signalDeviceSwitched(); break;
        case 4: _t->onAudioOutputDevicesChanged(); break;
        case 5: _t->requestDeviceSwitch((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: { PonyAudioFormat _r = _t->getCurrentDeviceFormat();
            if (_a[0]) *reinterpret_cast< PonyAudioFormat*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PonyAudioSink::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PonyAudioSink::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PonyAudioSink::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PonyAudioSink::resourceInsufficient)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PonyAudioSink::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PonyAudioSink::signalAudioOutputDeviceListChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PonyAudioSink::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PonyAudioSink::signalDeviceSwitched)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject PonyAudioSink::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PonyAudioSink.offsetsAndSize,
    qt_meta_data_PonyAudioSink,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_PonyAudioSink_t
, QtPrivate::TypeAndForceComplete<PonyAudioSink, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<PonyAudioFormat, std::false_type>


>,
    nullptr
} };


const QMetaObject *PonyAudioSink::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PonyAudioSink::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PonyAudioSink.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PonyAudioSink::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void PonyAudioSink::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void PonyAudioSink::resourceInsufficient()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PonyAudioSink::signalAudioOutputDeviceListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PonyAudioSink::signalDeviceSwitched()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
