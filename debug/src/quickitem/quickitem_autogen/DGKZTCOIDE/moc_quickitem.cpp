/****************************************************************************
** Meta object code from reading C++ file 'quickitem.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../include/quickitem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'quickitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoPlayWorker_t {
    const uint offsetsAndSize[46];
    char stringdata0[273];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_VideoPlayWorker_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_VideoPlayWorker_t qt_meta_stringdata_VideoPlayWorker = {
    {
QT_MOC_LITERAL(0, 15), // "VideoPlayWorker"
QT_MOC_LITERAL(16, 18), // "signalImageChanged"
QT_MOC_LITERAL(35, 0), // ""
QT_MOC_LITERAL(36, 7), // "Picture"
QT_MOC_LITERAL(44, 3), // "pic"
QT_MOC_LITERAL(48, 18), // "signalStateChanged"
QT_MOC_LITERAL(67, 14), // "HurricaneState"
QT_MOC_LITERAL(82, 5), // "state"
QT_MOC_LITERAL(88, 27), // "signalPositionChangedBySeek"
QT_MOC_LITERAL(116, 23), // "signalVolumeChangedFail"
QT_MOC_LITERAL(140, 1), // "d"
QT_MOC_LITERAL(142, 12), // "slotOpenFile"
QT_MOC_LITERAL(155, 4), // "path"
QT_MOC_LITERAL(160, 19), // "onAudioStateChanged"
QT_MOC_LITERAL(180, 13), // "QAudio::State"
QT_MOC_LITERAL(194, 14), // "slotThreadInit"
QT_MOC_LITERAL(209, 10), // "slotOnWork"
QT_MOC_LITERAL(220, 9), // "slotClose"
QT_MOC_LITERAL(230, 17), // "slotVolumeChanged"
QT_MOC_LITERAL(248, 1), // "v"
QT_MOC_LITERAL(250, 9), // "slotPause"
QT_MOC_LITERAL(260, 8), // "slotSeek"
QT_MOC_LITERAL(269, 3) // "pos"

    },
    "VideoPlayWorker\0signalImageChanged\0\0"
    "Picture\0pic\0signalStateChanged\0"
    "HurricaneState\0state\0signalPositionChangedBySeek\0"
    "signalVolumeChangedFail\0d\0slotOpenFile\0"
    "path\0onAudioStateChanged\0QAudio::State\0"
    "slotThreadInit\0slotOnWork\0slotClose\0"
    "slotVolumeChanged\0v\0slotPause\0slotSeek\0"
    "pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoPlayWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   86,    2, 0x06,    1 /* Public */,
       5,    1,   89,    2, 0x06,    3 /* Public */,
       8,    0,   92,    2, 0x06,    5 /* Public */,
       9,    1,   93,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    1,   96,    2, 0x0a,    8 /* Public */,
      13,    1,   99,    2, 0x0a,   10 /* Public */,
      15,    0,  102,    2, 0x0a,   12 /* Public */,
      16,    0,  103,    2, 0x0a,   13 /* Public */,
      17,    0,  104,    2, 0x0a,   14 /* Public */,
      18,    1,  105,    2, 0x0a,   15 /* Public */,
      20,    0,  108,    2, 0x0a,   17 /* Public */,
      21,    1,  109,    2, 0x0a,   18 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, 0x80000000 | 14,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,   22,

       0        // eod
};

void VideoPlayWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoPlayWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalImageChanged((*reinterpret_cast< std::add_pointer_t<Picture>>(_a[1]))); break;
        case 1: _t->signalStateChanged((*reinterpret_cast< std::add_pointer_t<HurricaneState>>(_a[1]))); break;
        case 2: _t->signalPositionChangedBySeek(); break;
        case 3: _t->signalVolumeChangedFail((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 4: _t->slotOpenFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->onAudioStateChanged((*reinterpret_cast< std::add_pointer_t<QAudio::State>>(_a[1]))); break;
        case 6: _t->slotThreadInit(); break;
        case 7: _t->slotOnWork(); break;
        case 8: _t->slotClose(); break;
        case 9: _t->slotVolumeChanged((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 10: _t->slotPause(); break;
        case 11: _t->slotSeek((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoPlayWorker::*)(Picture );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoPlayWorker::signalImageChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoPlayWorker::*)(HurricaneState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoPlayWorker::signalStateChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoPlayWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoPlayWorker::signalPositionChangedBySeek)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoPlayWorker::*)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoPlayWorker::signalVolumeChangedFail)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject VideoPlayWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VideoPlayWorker.offsetsAndSize,
    qt_meta_data_VideoPlayWorker,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_VideoPlayWorker_t
, QtPrivate::TypeAndForceComplete<VideoPlayWorker, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Picture, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<HurricaneState, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAudio::State, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>


>,
    nullptr
} };


const QMetaObject *VideoPlayWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoPlayWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoPlayWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoPlayWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void VideoPlayWorker::signalImageChanged(Picture _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoPlayWorker::signalStateChanged(HurricaneState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoPlayWorker::signalPositionChangedBySeek()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VideoPlayWorker::signalVolumeChangedFail(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_HurricanePlayer_t {
    const uint offsetsAndSize[72];
    char stringdata0[414];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_HurricanePlayer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_HurricanePlayer_t qt_meta_stringdata_HurricanePlayer = {
    {
QT_MOC_LITERAL(0, 15), // "HurricanePlayer"
QT_MOC_LITERAL(16, 11), // "QML.Element"
QT_MOC_LITERAL(28, 4), // "auto"
QT_MOC_LITERAL(33, 12), // "stateChanged"
QT_MOC_LITERAL(46, 0), // ""
QT_MOC_LITERAL(47, 13), // "volumeChanged"
QT_MOC_LITERAL(61, 21), // "positionChangedBySeek"
QT_MOC_LITERAL(83, 17), // "volumeChangedFail"
QT_MOC_LITERAL(101, 7), // "current"
QT_MOC_LITERAL(109, 24), // "signalPlayerInitializing"
QT_MOC_LITERAL(134, 11), // "signalPause"
QT_MOC_LITERAL(146, 12), // "signalResume"
QT_MOC_LITERAL(159, 11), // "signalClose"
QT_MOC_LITERAL(171, 20), // "signalVolumeChanging"
QT_MOC_LITERAL(192, 1), // "v"
QT_MOC_LITERAL(194, 14), // "signalOpenFile"
QT_MOC_LITERAL(209, 3), // "url"
QT_MOC_LITERAL(213, 10), // "signalSeek"
QT_MOC_LITERAL(224, 3), // "pos"
QT_MOC_LITERAL(228, 8), // "openFile"
QT_MOC_LITERAL(237, 4), // "path"
QT_MOC_LITERAL(242, 9), // "autoClose"
QT_MOC_LITERAL(252, 5), // "start"
QT_MOC_LITERAL(258, 5), // "pause"
QT_MOC_LITERAL(264, 5), // "close"
QT_MOC_LITERAL(270, 9), // "setVolume"
QT_MOC_LITERAL(280, 16), // "getAudioDuration"
QT_MOC_LITERAL(297, 16), // "getVideoDuration"
QT_MOC_LITERAL(314, 6), // "getPTS"
QT_MOC_LITERAL(321, 4), // "seek"
QT_MOC_LITERAL(326, 16), // "slotStateChanged"
QT_MOC_LITERAL(343, 14), // "HurricaneState"
QT_MOC_LITERAL(358, 1), // "s"
QT_MOC_LITERAL(360, 25), // "slotPositionChangedBySeek"
QT_MOC_LITERAL(386, 21), // "slotVolumeChangedFail"
QT_MOC_LITERAL(408, 5) // "state"

    },
    "HurricanePlayer\0QML.Element\0auto\0"
    "stateChanged\0\0volumeChanged\0"
    "positionChangedBySeek\0volumeChangedFail\0"
    "current\0signalPlayerInitializing\0"
    "signalPause\0signalResume\0signalClose\0"
    "signalVolumeChanging\0v\0signalOpenFile\0"
    "url\0signalSeek\0pos\0openFile\0path\0"
    "autoClose\0start\0pause\0close\0setVolume\0"
    "getAudioDuration\0getVideoDuration\0"
    "getPTS\0seek\0slotStateChanged\0"
    "HurricaneState\0s\0slotPositionChangedBySeek\0"
    "slotVolumeChangedFail\0state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HurricanePlayer[] = {

 // content:
      10,       // revision
       0,       // classname
       1,   14, // classinfo
      24,   16, // methods
       1,  206, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,  160,    4, 0x06,    2 /* Public */,
       5,    0,  161,    4, 0x06,    3 /* Public */,
       6,    0,  162,    4, 0x06,    4 /* Public */,
       7,    1,  163,    4, 0x06,    5 /* Public */,
       9,    0,  166,    4, 0x06,    7 /* Public */,
      10,    0,  167,    4, 0x06,    8 /* Public */,
      11,    0,  168,    4, 0x06,    9 /* Public */,
      12,    0,  169,    4, 0x06,   10 /* Public */,
      13,    1,  170,    4, 0x06,   11 /* Public */,
      15,    1,  173,    4, 0x06,   13 /* Public */,
      17,    1,  176,    4, 0x06,   15 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      19,    2,  179,    4, 0x0a,   17 /* Public */,
      19,    1,  184,    4, 0x2a,   20 /* Public | MethodCloned */,
      22,    0,  187,    4, 0x0a,   22 /* Public */,
      23,    0,  188,    4, 0x0a,   23 /* Public */,
      24,    0,  189,    4, 0x0a,   24 /* Public */,
      25,    1,  190,    4, 0x0a,   25 /* Public */,
      26,    0,  193,    4, 0x0a,   27 /* Public */,
      27,    0,  194,    4, 0x0a,   28 /* Public */,
      28,    0,  195,    4, 0x0a,   29 /* Public */,
      29,    1,  196,    4, 0x0a,   30 /* Public */,
      30,    1,  199,    4, 0x08,   32 /* Private */,
      33,    0,  202,    4, 0x08,   34 /* Private */,
      34,    1,  203,    4, 0x08,   35 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QReal,   18,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   20,   21,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,   14,
    QMetaType::QReal,
    QMetaType::QReal,
    QMetaType::QReal,
    QMetaType::Void, QMetaType::QReal,   18,
    QMetaType::Void, 0x80000000 | 31,   32,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,    8,

 // properties: name, type, flags
      35, 0x80000000 | 31, 0x00015809, uint(0), 0,

 // enums: name, alias, flags, count, data

 // enum data: key, value

       0        // eod
};

void HurricanePlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HurricanePlayer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stateChanged(); break;
        case 1: _t->volumeChanged(); break;
        case 2: _t->positionChangedBySeek(); break;
        case 3: _t->volumeChangedFail((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 4: _t->signalPlayerInitializing(QPrivateSignal()); break;
        case 5: _t->signalPause(QPrivateSignal()); break;
        case 6: _t->signalResume(QPrivateSignal()); break;
        case 7: _t->signalClose(QPrivateSignal()); break;
        case 8: _t->signalVolumeChanging((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 9: _t->signalOpenFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])), QPrivateSignal()); break;
        case 10: _t->signalSeek((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 11: _t->openFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 12: _t->openFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->start(); break;
        case 14: _t->pause(); break;
        case 15: _t->close(); break;
        case 16: _t->setVolume((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 17: { qreal _r = _t->getAudioDuration();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 18: { qreal _r = _t->getVideoDuration();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 19: { qreal _r = _t->getPTS();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->seek((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 21: _t->slotStateChanged((*reinterpret_cast< std::add_pointer_t<HurricaneState>>(_a[1]))); break;
        case 22: _t->slotPositionChangedBySeek(); break;
        case 23: _t->slotVolumeChangedFail((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HurricanePlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::volumeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::positionChangedBySeek)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::volumeChangedFail)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::signalPlayerInitializing)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::signalPause)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::signalResume)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::signalClose)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::signalVolumeChanging)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)(const QString & , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::signalOpenFile)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (HurricanePlayer::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HurricanePlayer::signalSeek)) {
                *result = 10;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<HurricanePlayer *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< HurricaneState*>(_v) = _t->getState(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject HurricanePlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<Hurricane::staticMetaObject>(),
    qt_meta_stringdata_HurricanePlayer.offsetsAndSize,
    qt_meta_data_HurricanePlayer,
    qt_static_metacall,
    nullptr,
qt_metaTypeArray<
HurricaneState, HurricanePlayer, void, void, void, void, qreal, void, void, void, void, void, qreal, void, const QString &, void, qreal
, void, const QString &, bool, void, const QString &, void, void, void, void, qreal, qreal, qreal, qreal, void, qreal, void, HurricaneState, void, void, qreal


>,
    nullptr
} };


const QMetaObject *HurricanePlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HurricanePlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HurricanePlayer.stringdata0))
        return static_cast<void*>(this);
    return Hurricane::qt_metacast(_clname);
}

int HurricanePlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Hurricane::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
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
void HurricanePlayer::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HurricanePlayer::volumeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HurricanePlayer::positionChangedBySeek()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void HurricanePlayer::volumeChangedFail(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void HurricanePlayer::signalPlayerInitializing(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void HurricanePlayer::signalPause(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void HurricanePlayer::signalResume(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void HurricanePlayer::signalClose(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void HurricanePlayer::signalVolumeChanging(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void HurricanePlayer::signalOpenFile(const QString & _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void HurricanePlayer::signalSeek(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
struct qt_meta_stringdata_H_t {
    const uint offsetsAndSize[18];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_H_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_H_t qt_meta_stringdata_H = {
    {
QT_MOC_LITERAL(0, 1), // "H"
QT_MOC_LITERAL(2, 14), // "HurricaneState"
QT_MOC_LITERAL(17, 7), // "CLOSING"
QT_MOC_LITERAL(25, 7), // "LOADING"
QT_MOC_LITERAL(33, 7), // "INVALID"
QT_MOC_LITERAL(41, 8), // "PRE_PLAY"
QT_MOC_LITERAL(50, 7), // "PLAYING"
QT_MOC_LITERAL(58, 9), // "PRE_PAUSE"
QT_MOC_LITERAL(68, 6) // "PAUSED"

    },
    "H\0HurricaneState\0CLOSING\0LOADING\0"
    "INVALID\0PRE_PLAY\0PLAYING\0PRE_PAUSE\0"
    "PAUSED"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_H[] = {

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
       1,    1, 0x0,    7,   19,

 // enum data: key, value
       2, uint(H::CLOSING),
       3, uint(H::LOADING),
       4, uint(H::INVALID),
       5, uint(H::PRE_PLAY),
       6, uint(H::PLAYING),
       7, uint(H::PRE_PAUSE),
       8, uint(H::PAUSED),

       0        // eod
};

const QMetaObject H::staticMetaObject = { {
    nullptr,
    qt_meta_stringdata_H.offsetsAndSize,
    qt_meta_data_H,
    nullptr,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_H_t
, QtPrivate::TypeAndForceComplete<void, std::true_type>



>,
    nullptr
} };

QT_WARNING_POP
QT_END_MOC_NAMESPACE
