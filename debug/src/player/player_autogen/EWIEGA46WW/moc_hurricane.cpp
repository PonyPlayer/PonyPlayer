/****************************************************************************
** Meta object code from reading C++ file 'hurricane.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/player/hurricane.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hurricane.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Hurricane_t {
    const uint offsetsAndSize[142];
    char stringdata0[846];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Hurricane_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Hurricane_t qt_meta_stringdata_Hurricane = {
    {
QT_MOC_LITERAL(0, 9), // "Hurricane"
QT_MOC_LITERAL(10, 18), // "QML.AddedInVersion"
QT_MOC_LITERAL(29, 3), // "256"
QT_MOC_LITERAL(33, 11), // "QML.Element"
QT_MOC_LITERAL(45, 4), // "auto"
QT_MOC_LITERAL(50, 12), // "stateChanged"
QT_MOC_LITERAL(63, 0), // ""
QT_MOC_LITERAL(64, 14), // "openFileResult"
QT_MOC_LITERAL(79, 30), // "PonyPlayer::OpenFileResultType"
QT_MOC_LITERAL(110, 6), // "result"
QT_MOC_LITERAL(117, 21), // "positionChangedBySeek"
QT_MOC_LITERAL(139, 24), // "audioOutputDeviceChanged"
QT_MOC_LITERAL(164, 21), // "backwardStatusChanged"
QT_MOC_LITERAL(186, 12), // "trackChanged"
QT_MOC_LITERAL(199, 26), // "currentOutputDeviceChanged"
QT_MOC_LITERAL(226, 12), // "pitchChanged"
QT_MOC_LITERAL(239, 12), // "speedChanged"
QT_MOC_LITERAL(252, 12), // "resourcesEnd"
QT_MOC_LITERAL(265, 24), // "signalPlayerInitializing"
QT_MOC_LITERAL(290, 11), // "signalStart"
QT_MOC_LITERAL(302, 11), // "signalPause"
QT_MOC_LITERAL(314, 11), // "signalClose"
QT_MOC_LITERAL(326, 14), // "signalOpenFile"
QT_MOC_LITERAL(341, 4), // "path"
QT_MOC_LITERAL(346, 10), // "signalSeek"
QT_MOC_LITERAL(357, 3), // "pos"
QT_MOC_LITERAL(361, 8), // "openFile"
QT_MOC_LITERAL(370, 3), // "url"
QT_MOC_LITERAL(374, 9), // "autoClose"
QT_MOC_LITERAL(384, 5), // "start"
QT_MOC_LITERAL(390, 5), // "pause"
QT_MOC_LITERAL(396, 5), // "close"
QT_MOC_LITERAL(402, 9), // "setVolume"
QT_MOC_LITERAL(412, 6), // "volume"
QT_MOC_LITERAL(419, 8), // "setPitch"
QT_MOC_LITERAL(428, 5), // "pitch"
QT_MOC_LITERAL(434, 8), // "setSpeed"
QT_MOC_LITERAL(443, 5), // "speed"
QT_MOC_LITERAL(449, 22), // "setCurrentOutputDevice"
QT_MOC_LITERAL(472, 10), // "deviceName"
QT_MOC_LITERAL(483, 22), // "getCurrentOutputDevice"
QT_MOC_LITERAL(506, 16), // "getAudioDuration"
QT_MOC_LITERAL(523, 16), // "getVideoDuration"
QT_MOC_LITERAL(540, 6), // "getPTS"
QT_MOC_LITERAL(547, 4), // "seek"
QT_MOC_LITERAL(552, 9), // "getTracks"
QT_MOC_LITERAL(562, 8), // "setTrack"
QT_MOC_LITERAL(571, 1), // "i"
QT_MOC_LITERAL(573, 7), // "forward"
QT_MOC_LITERAL(581, 8), // "backward"
QT_MOC_LITERAL(590, 14), // "toggleBackward"
QT_MOC_LITERAL(605, 8), // "hasVideo"
QT_MOC_LITERAL(614, 8), // "getPitch"
QT_MOC_LITERAL(623, 25), // "slotPositionChangedBySeek"
QT_MOC_LITERAL(649, 24), // "slotPlaybackStateChanged"
QT_MOC_LITERAL(674, 9), // "isPlaying"
QT_MOC_LITERAL(684, 18), // "slotOpenFileResult"
QT_MOC_LITERAL(703, 5), // "state"
QT_MOC_LITERAL(709, 14), // "HurricaneState"
QT_MOC_LITERAL(724, 15), // "audioDeviceList"
QT_MOC_LITERAL(740, 6), // "tracks"
QT_MOC_LITERAL(747, 14), // "backwardStatus"
QT_MOC_LITERAL(762, 5), // "track"
QT_MOC_LITERAL(768, 19), // "currentOutputDevice"
QT_MOC_LITERAL(788, 7), // "CLOSING"
QT_MOC_LITERAL(796, 7), // "LOADING"
QT_MOC_LITERAL(804, 7), // "INVALID"
QT_MOC_LITERAL(812, 8), // "PRE_PLAY"
QT_MOC_LITERAL(821, 7), // "PLAYING"
QT_MOC_LITERAL(829, 9), // "PRE_PAUSE"
QT_MOC_LITERAL(839, 6) // "PAUSED"

    },
    "Hurricane\0QML.AddedInVersion\0""256\0"
    "QML.Element\0auto\0stateChanged\0\0"
    "openFileResult\0PonyPlayer::OpenFileResultType\0"
    "result\0positionChangedBySeek\0"
    "audioOutputDeviceChanged\0backwardStatusChanged\0"
    "trackChanged\0currentOutputDeviceChanged\0"
    "pitchChanged\0speedChanged\0resourcesEnd\0"
    "signalPlayerInitializing\0signalStart\0"
    "signalPause\0signalClose\0signalOpenFile\0"
    "path\0signalSeek\0pos\0openFile\0url\0"
    "autoClose\0start\0pause\0close\0setVolume\0"
    "volume\0setPitch\0pitch\0setSpeed\0speed\0"
    "setCurrentOutputDevice\0deviceName\0"
    "getCurrentOutputDevice\0getAudioDuration\0"
    "getVideoDuration\0getPTS\0seek\0getTracks\0"
    "setTrack\0i\0forward\0backward\0toggleBackward\0"
    "hasVideo\0getPitch\0slotPositionChangedBySeek\0"
    "slotPlaybackStateChanged\0isPlaying\0"
    "slotOpenFileResult\0state\0HurricaneState\0"
    "audioDeviceList\0tracks\0backwardStatus\0"
    "track\0currentOutputDevice\0CLOSING\0"
    "LOADING\0INVALID\0PRE_PLAY\0PLAYING\0"
    "PRE_PAUSE\0PAUSED"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Hurricane[] = {

 // content:
      10,       // revision
       0,       // classname
       3,   14, // classinfo
      40,   20, // methods
       8,  328, // properties
       1,  368, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,
       3,    4,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,  260,    6, 0x06,    9 /* Public */,
       7,    1,  261,    6, 0x06,   10 /* Public */,
      10,    0,  264,    6, 0x06,   12 /* Public */,
      11,    0,  265,    6, 0x06,   13 /* Public */,
      12,    0,  266,    6, 0x06,   14 /* Public */,
      13,    0,  267,    6, 0x06,   15 /* Public */,
      14,    0,  268,    6, 0x06,   16 /* Public */,
      15,    0,  269,    6, 0x06,   17 /* Public */,
      16,    0,  270,    6, 0x06,   18 /* Public */,
      17,    0,  271,    6, 0x06,   19 /* Public */,
      18,    0,  272,    6, 0x06,   20 /* Public */,
      19,    0,  273,    6, 0x06,   21 /* Public */,
      20,    0,  274,    6, 0x06,   22 /* Public */,
      21,    0,  275,    6, 0x06,   23 /* Public */,
      22,    1,  276,    6, 0x06,   24 /* Public */,
      24,    1,  279,    6, 0x06,   26 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      26,    2,  282,    6, 0x0a,   28 /* Public */,
      26,    1,  287,    6, 0x2a,   31 /* Public | MethodCloned */,
      29,    0,  290,    6, 0x0a,   33 /* Public */,
      30,    0,  291,    6, 0x0a,   34 /* Public */,
      31,    0,  292,    6, 0x0a,   35 /* Public */,
      32,    1,  293,    6, 0x0a,   36 /* Public */,
      34,    1,  296,    6, 0x0a,   38 /* Public */,
      36,    1,  299,    6, 0x0a,   40 /* Public */,
      38,    1,  302,    6, 0x0a,   42 /* Public */,
      40,    0,  305,    6, 0x0a,   44 /* Public */,
      41,    0,  306,    6, 0x0a,   45 /* Public */,
      42,    0,  307,    6, 0x0a,   46 /* Public */,
      43,    0,  308,    6, 0x0a,   47 /* Public */,
      44,    1,  309,    6, 0x0a,   48 /* Public */,
      45,    0,  312,    6, 0x0a,   50 /* Public */,
      46,    1,  313,    6, 0x0a,   51 /* Public */,
      48,    0,  316,    6, 0x0a,   53 /* Public */,
      49,    0,  317,    6, 0x0a,   54 /* Public */,
      50,    0,  318,    6, 0x0a,   55 /* Public */,
      51,    0,  319,    6, 0x0a,   56 /* Public */,
      52,    0,  320,    6, 0x0a,   57 /* Public */,
      53,    0,  321,    6, 0x08,   58 /* Private */,
      54,    1,  322,    6, 0x08,   59 /* Private */,
      56,    1,  325,    6, 0x08,   61 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void, QMetaType::QReal,   25,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   27,   28,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,   33,
    QMetaType::Void, QMetaType::QReal,   35,
    QMetaType::Void, QMetaType::QReal,   37,
    QMetaType::Void, QMetaType::QString,   39,
    QMetaType::QString,
    QMetaType::QReal,
    QMetaType::QReal,
    QMetaType::QReal,
    QMetaType::Void, QMetaType::QReal,   25,
    QMetaType::QStringList,
    QMetaType::Void, QMetaType::Int,   47,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::QReal,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   55,
    QMetaType::Void, 0x80000000 | 8,    9,

 // properties: name, type, flags
      35, QMetaType::QReal, 0x00015103, uint(7), 0,
      57, 0x80000000 | 58, 0x00015809, uint(0), 0,
      59, QMetaType::QStringList, 0x00015001, uint(3), 0,
      60, QMetaType::QStringList, 0x00015001, uint(1), 0,
      61, QMetaType::Bool, 0x00015001, uint(4), 0,
      62, QMetaType::Int, 0x00015103, uint(5), 0,
      63, QMetaType::QString, 0x00015103, uint(6), 0,
      37, QMetaType::Double, 0x00015103, uint(8), 0,

 // enums: name, alias, flags, count, data
      58,   58, 0x0,    7,  373,

 // enum data: key, value
      64, uint(Hurricane::CLOSING),
      65, uint(Hurricane::LOADING),
      66, uint(Hurricane::INVALID),
      67, uint(Hurricane::PRE_PLAY),
      68, uint(Hurricane::PLAYING),
      69, uint(Hurricane::PRE_PAUSE),
      70, uint(Hurricane::PAUSED),

       0        // eod
};

void Hurricane::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Hurricane *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stateChanged(); break;
        case 1: _t->openFileResult((*reinterpret_cast< std::add_pointer_t<PonyPlayer::OpenFileResultType>>(_a[1])), QPrivateSignal()); break;
        case 2: _t->positionChangedBySeek(); break;
        case 3: _t->audioOutputDeviceChanged(); break;
        case 4: _t->backwardStatusChanged(); break;
        case 5: _t->trackChanged(); break;
        case 6: _t->currentOutputDeviceChanged(); break;
        case 7: _t->pitchChanged(); break;
        case 8: _t->speedChanged(); break;
        case 9: _t->resourcesEnd(); break;
        case 10: _t->signalPlayerInitializing(QPrivateSignal()); break;
        case 11: _t->signalStart(QPrivateSignal()); break;
        case 12: _t->signalPause(QPrivateSignal()); break;
        case 13: _t->signalClose(QPrivateSignal()); break;
        case 14: _t->signalOpenFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])), QPrivateSignal()); break;
        case 15: _t->signalSeek((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 16: _t->openFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 17: _t->openFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->start(); break;
        case 19: _t->pause(); break;
        case 20: _t->close(); break;
        case 21: _t->setVolume((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 22: _t->setPitch((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 23: _t->setSpeed((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 24: _t->setCurrentOutputDevice((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: { QString _r = _t->getCurrentOutputDevice();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 26: { qreal _r = _t->getAudioDuration();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 27: { qreal _r = _t->getVideoDuration();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 28: { qreal _r = _t->getPTS();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 29: _t->seek((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 30: { QStringList _r = _t->getTracks();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 31: _t->setTrack((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 32: _t->forward(); break;
        case 33: _t->backward(); break;
        case 34: _t->toggleBackward(); break;
        case 35: { bool _r = _t->hasVideo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 36: { qreal _r = _t->getPitch();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 37: _t->slotPositionChangedBySeek(); break;
        case 38: _t->slotPlaybackStateChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 39: _t->slotOpenFileResult((*reinterpret_cast< std::add_pointer_t<PonyPlayer::OpenFileResultType>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)(PonyPlayer::OpenFileResultType , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::openFileResult)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::positionChangedBySeek)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::audioOutputDeviceChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::backwardStatusChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::trackChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::currentOutputDeviceChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::pitchChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::speedChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::resourcesEnd)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::signalPlayerInitializing)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::signalStart)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::signalPause)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)(QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::signalClose)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)(const QString & , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::signalOpenFile)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (Hurricane::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Hurricane::signalSeek)) {
                *result = 15;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Hurricane *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->getPitch(); break;
        case 1: *reinterpret_cast< HurricaneState*>(_v) = _t->getState(); break;
        case 2: *reinterpret_cast< QStringList*>(_v) = _t->getAudioDeviceList(); break;
        case 3: *reinterpret_cast< QStringList*>(_v) = _t->getTracks(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->isBackward(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->getTrack(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getCurrentOutputDevice(); break;
        case 7: *reinterpret_cast< double*>(_v) = _t->getSpeed(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Hurricane *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPitch(*reinterpret_cast< qreal*>(_v)); break;
        case 5: _t->setTrack(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setCurrentOutputDevice(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setSpeed(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Hurricane::staticMetaObject = { {
    QMetaObject::SuperData::link<Fireworks::staticMetaObject>(),
    qt_meta_stringdata_Hurricane.offsetsAndSize,
    qt_meta_data_Hurricane,
    qt_static_metacall,
    nullptr,
qt_metaTypeArray<
qreal, HurricaneState, QStringList, QStringList, bool, int, QString, double, Hurricane, void, void, PonyPlayer::OpenFileResultType, void, void, void, void, void, void, void, void, void, void, void, void, void, const QString &, void, qreal
, void, const QString &, bool, void, const QString &, void, void, void, void, qreal, void, qreal, void, qreal, void, QString, QString, qreal, qreal, qreal, void, qreal, QStringList, void, int, void, void, void, bool, qreal, void, void, bool, void, PonyPlayer::OpenFileResultType


>,
    nullptr
} };


const QMetaObject *Hurricane::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Hurricane::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Hurricane.stringdata0))
        return static_cast<void*>(this);
    return Fireworks::qt_metacast(_clname);
}

int Hurricane::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Fireworks::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 40;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Hurricane::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Hurricane::openFileResult(PonyPlayer::OpenFileResultType _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Hurricane::positionChangedBySeek()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Hurricane::audioOutputDeviceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Hurricane::backwardStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Hurricane::trackChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Hurricane::currentOutputDeviceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Hurricane::pitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Hurricane::speedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void Hurricane::resourcesEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void Hurricane::signalPlayerInitializing(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Hurricane::signalStart(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Hurricane::signalPause(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Hurricane::signalClose(QPrivateSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void Hurricane::signalOpenFile(const QString & _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void Hurricane::signalSeek(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
