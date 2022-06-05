/****************************************************************************
** Meta object code from reading C++ file 'framecontroller.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/player/framecontroller.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'framecontroller.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FrameController_t {
    const uint offsetsAndSize[62];
    char stringdata0[389];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_FrameController_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_FrameController_t qt_meta_stringdata_FrameController = {
    {
QT_MOC_LITERAL(0, 15), // "FrameController"
QT_MOC_LITERAL(16, 21), // "signalDecoderOpenFile"
QT_MOC_LITERAL(38, 0), // ""
QT_MOC_LITERAL(39, 11), // "std::string"
QT_MOC_LITERAL(51, 4), // "path"
QT_MOC_LITERAL(56, 17), // "signalDecoderSeek"
QT_MOC_LITERAL(74, 3), // "pos"
QT_MOC_LITERAL(78, 27), // "signalPositionChangedBySeek"
QT_MOC_LITERAL(106, 14), // "signalSetTrack"
QT_MOC_LITERAL(121, 1), // "i"
QT_MOC_LITERAL(123, 14), // "signalBackward"
QT_MOC_LITERAL(138, 13), // "signalForward"
QT_MOC_LITERAL(152, 31), // "signalAudioOutputDevicesChanged"
QT_MOC_LITERAL(184, 20), // "signalDeviceSwitched"
QT_MOC_LITERAL(205, 14), // "openFileResult"
QT_MOC_LITERAL(220, 30), // "PonyPlayer::OpenFileResultType"
QT_MOC_LITERAL(251, 6), // "result"
QT_MOC_LITERAL(258, 20), // "playbackStateChanged"
QT_MOC_LITERAL(279, 9), // "isPlaying"
QT_MOC_LITERAL(289, 12), // "resourcesEnd"
QT_MOC_LITERAL(302, 10), // "setPicture"
QT_MOC_LITERAL(313, 13), // "VideoFrameRef"
QT_MOC_LITERAL(327, 3), // "pic"
QT_MOC_LITERAL(331, 12), // "initOnThread"
QT_MOC_LITERAL(344, 8), // "openFile"
QT_MOC_LITERAL(353, 5), // "pause"
QT_MOC_LITERAL(359, 4), // "stop"
QT_MOC_LITERAL(364, 5), // "close"
QT_MOC_LITERAL(370, 5), // "start"
QT_MOC_LITERAL(376, 4), // "seek"
QT_MOC_LITERAL(381, 7) // "seekPos"

    },
    "FrameController\0signalDecoderOpenFile\0"
    "\0std::string\0path\0signalDecoderSeek\0"
    "pos\0signalPositionChangedBySeek\0"
    "signalSetTrack\0i\0signalBackward\0"
    "signalForward\0signalAudioOutputDevicesChanged\0"
    "signalDeviceSwitched\0openFileResult\0"
    "PonyPlayer::OpenFileResultType\0result\0"
    "playbackStateChanged\0isPlaying\0"
    "resourcesEnd\0setPicture\0VideoFrameRef\0"
    "pic\0initOnThread\0openFile\0pause\0stop\0"
    "close\0start\0seek\0seekPos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FrameController[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  128,    2, 0x06,    1 /* Public */,
       5,    1,  131,    2, 0x06,    3 /* Public */,
       7,    0,  134,    2, 0x06,    5 /* Public */,
       8,    1,  135,    2, 0x06,    6 /* Public */,
      10,    0,  138,    2, 0x06,    8 /* Public */,
      11,    0,  139,    2, 0x06,    9 /* Public */,
      12,    0,  140,    2, 0x06,   10 /* Public */,
      13,    0,  141,    2, 0x06,   11 /* Public */,
      14,    1,  142,    2, 0x06,   12 /* Public */,
      17,    1,  145,    2, 0x06,   14 /* Public */,
      19,    0,  148,    2, 0x06,   16 /* Public */,
      20,    1,  149,    2, 0x06,   17 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      23,    0,  152,    2, 0x08,   19 /* Private */,
      24,    1,  153,    2, 0x0a,   20 /* Public */,
      25,    0,  156,    2, 0x0a,   22 /* Public */,
      26,    0,  157,    2, 0x0a,   23 /* Public */,
      27,    0,  158,    2, 0x0a,   24 /* Public */,
      28,    0,  159,    2, 0x0a,   25 /* Public */,
      29,    1,  160,    2, 0x0a,   26 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QReal,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,   30,

       0        // eod
};

void FrameController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FrameController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalDecoderOpenFile((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 1: _t->signalDecoderSeek((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 2: _t->signalPositionChangedBySeek(); break;
        case 3: _t->signalSetTrack((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->signalBackward(); break;
        case 5: _t->signalForward(); break;
        case 6: _t->signalAudioOutputDevicesChanged(); break;
        case 7: _t->signalDeviceSwitched(); break;
        case 8: _t->openFileResult((*reinterpret_cast< std::add_pointer_t<PonyPlayer::OpenFileResultType>>(_a[1]))); break;
        case 9: _t->playbackStateChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 10: _t->resourcesEnd(); break;
        case 11: _t->setPicture((*reinterpret_cast< std::add_pointer_t<VideoFrameRef>>(_a[1]))); break;
        case 12: _t->initOnThread(); break;
        case 13: _t->openFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->pause(); break;
        case 15: _t->stop(); break;
        case 16: _t->close(); break;
        case 17: _t->start(); break;
        case 18: _t->seek((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FrameController::*)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::signalDecoderOpenFile)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FrameController::*)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::signalDecoderSeek)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FrameController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::signalPositionChangedBySeek)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FrameController::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::signalSetTrack)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FrameController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::signalBackward)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (FrameController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::signalForward)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (FrameController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::signalAudioOutputDevicesChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (FrameController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::signalDeviceSwitched)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (FrameController::*)(PonyPlayer::OpenFileResultType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::openFileResult)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (FrameController::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::playbackStateChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (FrameController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::resourcesEnd)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (FrameController::*)(VideoFrameRef );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameController::setPicture)) {
                *result = 11;
                return;
            }
        }
    }
}

const QMetaObject FrameController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_FrameController.offsetsAndSize,
    qt_meta_data_FrameController,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_FrameController_t
, QtPrivate::TypeAndForceComplete<FrameController, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::string, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PonyPlayer::OpenFileResultType, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<VideoFrameRef, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>


>,
    nullptr
} };


const QMetaObject *FrameController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FrameController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FrameController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FrameController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void FrameController::signalDecoderOpenFile(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrameController::signalDecoderSeek(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FrameController::signalPositionChangedBySeek()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void FrameController::signalSetTrack(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FrameController::signalBackward()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void FrameController::signalForward()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void FrameController::signalAudioOutputDevicesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void FrameController::signalDeviceSwitched()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void FrameController::openFileResult(PonyPlayer::OpenFileResultType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void FrameController::playbackStateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void FrameController::resourcesEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void FrameController::setPicture(VideoFrameRef _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
