/****************************************************************************
** Meta object code from reading C++ file 'playback.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/player/playback.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playback.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Playback_t {
    const uint offsetsAndSize[56];
    char stringdata0[397];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Playback_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Playback_t qt_meta_stringdata_Playback = {
    {
QT_MOC_LITERAL(0, 8), // "Playback"
QT_MOC_LITERAL(9, 9), // "startWork"
QT_MOC_LITERAL(19, 0), // ""
QT_MOC_LITERAL(20, 8), // "stopWork"
QT_MOC_LITERAL(29, 15), // "clearRingBuffer"
QT_MOC_LITERAL(45, 18), // "setAudioStartPoint"
QT_MOC_LITERAL(64, 10), // "startPoint"
QT_MOC_LITERAL(75, 14), // "setAudioVolume"
QT_MOC_LITERAL(90, 6), // "volume"
QT_MOC_LITERAL(97, 13), // "setAudioPitch"
QT_MOC_LITERAL(111, 5), // "pitch"
QT_MOC_LITERAL(117, 13), // "setAudioSpeed"
QT_MOC_LITERAL(131, 5), // "speed"
QT_MOC_LITERAL(137, 34), // "signalSetSelectedAudioOutputD..."
QT_MOC_LITERAL(172, 20), // "signalDeviceSwitched"
QT_MOC_LITERAL(193, 19), // "showFirstVideoFrame"
QT_MOC_LITERAL(213, 20), // "clearCacheVideoFrame"
QT_MOC_LITERAL(234, 10), // "setPicture"
QT_MOC_LITERAL(245, 13), // "VideoFrameRef"
QT_MOC_LITERAL(259, 3), // "pic"
QT_MOC_LITERAL(263, 12), // "stateChanged"
QT_MOC_LITERAL(276, 9), // "isPlaying"
QT_MOC_LITERAL(286, 12), // "resourcesEnd"
QT_MOC_LITERAL(299, 35), // "signalAudioOutputDevicesListC..."
QT_MOC_LITERAL(335, 24), // "requestResynchronization"
QT_MOC_LITERAL(360, 11), // "enableAudio"
QT_MOC_LITERAL(372, 17), // "updateAudioFormat"
QT_MOC_LITERAL(390, 6) // "onWork"

    },
    "Playback\0startWork\0\0stopWork\0"
    "clearRingBuffer\0setAudioStartPoint\0"
    "startPoint\0setAudioVolume\0volume\0"
    "setAudioPitch\0pitch\0setAudioSpeed\0"
    "speed\0signalSetSelectedAudioOutputDevice\0"
    "signalDeviceSwitched\0showFirstVideoFrame\0"
    "clearCacheVideoFrame\0setPicture\0"
    "VideoFrameRef\0pic\0stateChanged\0isPlaying\0"
    "resourcesEnd\0signalAudioOutputDevicesListChanged\0"
    "requestResynchronization\0enableAudio\0"
    "updateAudioFormat\0onWork"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Playback[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  116,    2, 0x06,    1 /* Public */,
       3,    0,  117,    2, 0x06,    2 /* Public */,
       4,    0,  118,    2, 0x06,    3 /* Public */,
       5,    1,  119,    2, 0x06,    4 /* Public */,
       7,    1,  122,    2, 0x06,    6 /* Public */,
       9,    1,  125,    2, 0x06,    8 /* Public */,
      11,    1,  128,    2, 0x06,   10 /* Public */,
      13,    1,  131,    2, 0x06,   12 /* Public */,
      14,    0,  134,    2, 0x06,   14 /* Public */,
      15,    0,  135,    2, 0x06,   15 /* Public */,
      16,    0,  136,    2, 0x06,   16 /* Public */,
      17,    1,  137,    2, 0x06,   17 /* Public */,
      20,    1,  140,    2, 0x06,   19 /* Public */,
      22,    0,  143,    2, 0x06,   21 /* Public */,
      23,    0,  144,    2, 0x06,   22 /* Public */,
      24,    2,  145,    2, 0x06,   23 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      27,    0,  150,    2, 0x08,   26 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,    6,
    QMetaType::Void, QMetaType::QReal,    8,
    QMetaType::Void, QMetaType::QReal,   10,
    QMetaType::Void, QMetaType::QReal,   12,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, QMetaType::Bool,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   25,   26,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Playback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Playback *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->startWork(QPrivateSignal()); break;
        case 1: _t->stopWork(QPrivateSignal()); break;
        case 2: _t->clearRingBuffer(QPrivateSignal()); break;
        case 3: _t->setAudioStartPoint((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 4: _t->setAudioVolume((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 5: _t->setAudioPitch((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 6: _t->setAudioSpeed((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 7: _t->signalSetSelectedAudioOutputDevice((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->signalDeviceSwitched(); break;
        case 9: _t->showFirstVideoFrame(QPrivateSignal()); break;
        case 10: _t->clearCacheVideoFrame(QPrivateSignal()); break;
        case 11: _t->setPicture((*reinterpret_cast< std::add_pointer_t<VideoFrameRef>>(_a[1]))); break;
        case 12: _t->stateChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->resourcesEnd(); break;
        case 14: _t->signalAudioOutputDevicesListChanged(); break;
        case 15: _t->requestResynchronization((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 16: _t->onWork(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Playback::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::startWork)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Playback::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::stopWork)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Playback::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::clearRingBuffer)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Playback::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::setAudioStartPoint)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Playback::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::setAudioVolume)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Playback::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::setAudioPitch)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Playback::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::setAudioSpeed)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Playback::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::signalSetSelectedAudioOutputDevice)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Playback::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::signalDeviceSwitched)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (Playback::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::showFirstVideoFrame)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (Playback::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::clearCacheVideoFrame)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (Playback::*)(VideoFrameRef );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::setPicture)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (Playback::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::stateChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (Playback::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::resourcesEnd)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (Playback::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::signalAudioOutputDevicesListChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (Playback::*)(bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Playback::requestResynchronization)) {
                *result = 15;
                return;
            }
        }
    }
}

const QMetaObject Playback::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Playback.offsetsAndSize,
    qt_meta_data_Playback,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Playback_t
, QtPrivate::TypeAndForceComplete<Playback, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<VideoFrameRef, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Playback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Playback::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Playback.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Playback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void Playback::startWork(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Playback::stopWork(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Playback::clearRingBuffer(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Playback::setAudioStartPoint(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Playback::setAudioVolume(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Playback::setAudioPitch(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Playback::setAudioSpeed(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Playback::signalSetSelectedAudioOutputDevice(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Playback::signalDeviceSwitched()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void Playback::showFirstVideoFrame(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Playback::clearCacheVideoFrame(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Playback::setPicture(VideoFrameRef _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Playback::stateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Playback::resourcesEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void Playback::signalAudioOutputDevicesListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void Playback::requestResynchronization(bool _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
