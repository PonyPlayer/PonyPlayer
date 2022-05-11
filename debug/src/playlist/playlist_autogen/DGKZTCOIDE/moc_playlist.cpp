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
#include <QtCore/QList>
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
    const uint offsetsAndSize[144];
    char stringdata0[877];
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
QT_MOC_LITERAL(48, 7), // "getPath"
QT_MOC_LITERAL(56, 7), // "setPath"
QT_MOC_LITERAL(64, 5), // "_path"
QT_MOC_LITERAL(70, 12), // "getDirectory"
QT_MOC_LITERAL(83, 12), // "setDirectory"
QT_MOC_LITERAL(96, 4), // "QDir"
QT_MOC_LITERAL(101, 4), // "_dir"
QT_MOC_LITERAL(106, 17), // "setDirectoryByStr"
QT_MOC_LITERAL(124, 11), // "getDuration"
QT_MOC_LITERAL(136, 11), // "setDuration"
QT_MOC_LITERAL(148, 9), // "_duration"
QT_MOC_LITERAL(158, 12), // "getFrameRate"
QT_MOC_LITERAL(171, 12), // "setFrameRate"
QT_MOC_LITERAL(184, 10), // "_frameRate"
QT_MOC_LITERAL(195, 10), // "getBitRate"
QT_MOC_LITERAL(206, 10), // "setBitRate"
QT_MOC_LITERAL(217, 8), // "_bitRate"
QT_MOC_LITERAL(226, 12), // "getVideoSize"
QT_MOC_LITERAL(239, 12), // "setVideoSize"
QT_MOC_LITERAL(252, 10), // "_videoSize"
QT_MOC_LITERAL(263, 13), // "getVideoWidth"
QT_MOC_LITERAL(277, 13), // "setVideoWidth"
QT_MOC_LITERAL(291, 11), // "_videoWidth"
QT_MOC_LITERAL(303, 14), // "getVideoHeight"
QT_MOC_LITERAL(318, 14), // "setVideoHeight"
QT_MOC_LITERAL(333, 12), // "_videoHeight"
QT_MOC_LITERAL(346, 14), // "getVideoFormat"
QT_MOC_LITERAL(361, 14), // "setVideoFormat"
QT_MOC_LITERAL(376, 12), // "_videoFormat"
QT_MOC_LITERAL(389, 14), // "getAudioFormat"
QT_MOC_LITERAL(404, 14), // "setAudioFormat"
QT_MOC_LITERAL(419, 12), // "_audioFormat"
QT_MOC_LITERAL(432, 22), // "getAudioAverageBitRate"
QT_MOC_LITERAL(455, 22), // "setAudioAverageBitRate"
QT_MOC_LITERAL(478, 20), // "_audioAverageBitRate"
QT_MOC_LITERAL(499, 17), // "getChannelNumbers"
QT_MOC_LITERAL(517, 17), // "setChannelNumbers"
QT_MOC_LITERAL(535, 15), // "_channelNumbers"
QT_MOC_LITERAL(551, 13), // "getSampleRate"
QT_MOC_LITERAL(565, 13), // "setSampleRate"
QT_MOC_LITERAL(579, 11), // "_sampleRate"
QT_MOC_LITERAL(591, 9), // "getFormat"
QT_MOC_LITERAL(601, 9), // "setFormat"
QT_MOC_LITERAL(611, 7), // "_format"
QT_MOC_LITERAL(619, 12), // "getAudioSize"
QT_MOC_LITERAL(632, 12), // "setAudioSize"
QT_MOC_LITERAL(645, 10), // "_audioSize"
QT_MOC_LITERAL(656, 16), // "getStreamNumbers"
QT_MOC_LITERAL(673, 16), // "setStreamNumbers"
QT_MOC_LITERAL(690, 14), // "_streamNumbers"
QT_MOC_LITERAL(705, 6), // "_uuid_"
QT_MOC_LITERAL(712, 8), // "fileName"
QT_MOC_LITERAL(721, 3), // "dir"
QT_MOC_LITERAL(725, 8), // "duration"
QT_MOC_LITERAL(734, 9), // "frameRate"
QT_MOC_LITERAL(744, 7), // "bitRate"
QT_MOC_LITERAL(752, 9), // "videoSize"
QT_MOC_LITERAL(762, 10), // "videoWidth"
QT_MOC_LITERAL(773, 11), // "videoHeight"
QT_MOC_LITERAL(785, 11), // "videoFormat"
QT_MOC_LITERAL(797, 11), // "audioFormat"
QT_MOC_LITERAL(809, 19), // "audioAverageBitRate"
QT_MOC_LITERAL(829, 14), // "channelNumbers"
QT_MOC_LITERAL(844, 10), // "sampleRate"
QT_MOC_LITERAL(855, 9), // "audioSize"
QT_MOC_LITERAL(865, 6), // "format"
QT_MOC_LITERAL(872, 4) // "path"

    },
    "PlayListItem\0getFileName\0\0setFileName\0"
    "_fileName\0getPath\0setPath\0_path\0"
    "getDirectory\0setDirectory\0QDir\0_dir\0"
    "setDirectoryByStr\0getDuration\0setDuration\0"
    "_duration\0getFrameRate\0setFrameRate\0"
    "_frameRate\0getBitRate\0setBitRate\0"
    "_bitRate\0getVideoSize\0setVideoSize\0"
    "_videoSize\0getVideoWidth\0setVideoWidth\0"
    "_videoWidth\0getVideoHeight\0setVideoHeight\0"
    "_videoHeight\0getVideoFormat\0setVideoFormat\0"
    "_videoFormat\0getAudioFormat\0setAudioFormat\0"
    "_audioFormat\0getAudioAverageBitRate\0"
    "setAudioAverageBitRate\0_audioAverageBitRate\0"
    "getChannelNumbers\0setChannelNumbers\0"
    "_channelNumbers\0getSampleRate\0"
    "setSampleRate\0_sampleRate\0getFormat\0"
    "setFormat\0_format\0getAudioSize\0"
    "setAudioSize\0_audioSize\0getStreamNumbers\0"
    "setStreamNumbers\0_streamNumbers\0_uuid_\0"
    "fileName\0dir\0duration\0frameRate\0bitRate\0"
    "videoSize\0videoWidth\0videoHeight\0"
    "videoFormat\0audioFormat\0audioAverageBitRate\0"
    "channelNumbers\0sampleRate\0audioSize\0"
    "format\0path"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlayListItem[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
      17,  301, // properties
       0,    0, // enums/sets
       2,  386, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  224,    2, 0x02,   18 /* Public */,
       3,    1,  225,    2, 0x02,   19 /* Public */,
       5,    0,  228,    2, 0x02,   21 /* Public */,
       6,    1,  229,    2, 0x02,   22 /* Public */,
       8,    0,  232,    2, 0x02,   24 /* Public */,
       9,    1,  233,    2, 0x02,   25 /* Public */,
      12,    1,  236,    2, 0x02,   27 /* Public */,
      13,    0,  239,    2, 0x02,   29 /* Public */,
      14,    1,  240,    2, 0x02,   30 /* Public */,
      16,    0,  243,    2, 0x02,   32 /* Public */,
      17,    1,  244,    2, 0x02,   33 /* Public */,
      19,    0,  247,    2, 0x02,   35 /* Public */,
      20,    1,  248,    2, 0x02,   36 /* Public */,
      22,    0,  251,    2, 0x02,   38 /* Public */,
      23,    1,  252,    2, 0x02,   39 /* Public */,
      25,    0,  255,    2, 0x02,   41 /* Public */,
      26,    1,  256,    2, 0x02,   42 /* Public */,
      28,    0,  259,    2, 0x02,   44 /* Public */,
      29,    1,  260,    2, 0x02,   45 /* Public */,
      31,    0,  263,    2, 0x02,   47 /* Public */,
      32,    1,  264,    2, 0x02,   48 /* Public */,
      34,    0,  267,    2, 0x02,   50 /* Public */,
      35,    1,  268,    2, 0x02,   51 /* Public */,
      37,    0,  271,    2, 0x02,   53 /* Public */,
      38,    1,  272,    2, 0x02,   54 /* Public */,
      40,    0,  275,    2, 0x02,   56 /* Public */,
      41,    1,  276,    2, 0x02,   57 /* Public */,
      43,    0,  279,    2, 0x02,   59 /* Public */,
      44,    1,  280,    2, 0x02,   60 /* Public */,
      46,    0,  283,    2, 0x02,   62 /* Public */,
      47,    1,  284,    2, 0x02,   63 /* Public */,
      49,    0,  287,    2, 0x02,   65 /* Public */,
      50,    1,  288,    2, 0x02,   66 /* Public */,
      52,    0,  291,    2, 0x02,   68 /* Public */,
      53,    1,  292,    2, 0x02,   69 /* Public */,

 // methods: parameters
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::QString,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Float,
    QMetaType::Void, QMetaType::Float,   24,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   30,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,   36,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   39,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   45,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,   48,
    QMetaType::Float,
    QMetaType::Void, QMetaType::Float,   51,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   54,

 // constructors: parameters
    0x80000000 | 2, QMetaType::QString, 0x80000000 | 10,    4,   11,
    0x80000000 | 2,

 // properties: name, type, flags
      55, QMetaType::QString, 0x00015003, uint(-1), 0,
      56, QMetaType::QString, 0x00015103, uint(-1), 0,
      57, QMetaType::QString, 0x00015003, uint(-1), 0,
      58, QMetaType::QString, 0x00015103, uint(-1), 0,
      59, QMetaType::Int, 0x00015103, uint(-1), 0,
      60, QMetaType::Int, 0x00015103, uint(-1), 0,
      61, QMetaType::Float, 0x00015103, uint(-1), 0,
      62, QMetaType::Int, 0x00015103, uint(-1), 0,
      63, QMetaType::Int, 0x00015103, uint(-1), 0,
      64, QMetaType::QString, 0x00015103, uint(-1), 0,
      65, QMetaType::QString, 0x00015103, uint(-1), 0,
      66, QMetaType::Int, 0x00015103, uint(-1), 0,
      67, QMetaType::Int, 0x00015103, uint(-1), 0,
      68, QMetaType::Int, 0x00015103, uint(-1), 0,
      69, QMetaType::Float, 0x00015103, uint(-1), 0,
      70, QMetaType::QString, 0x00015103, uint(-1), 0,
      71, QMetaType::QString, 0x00015103, uint(-1), 0,

 // constructors: name, argc, parameters, tag, flags, initial metatype offsets
       0,    2,  295,    2, 0x0e,   71 /* Public */,
       0,    0,  300,    2, 0x0e,   73 /* Public */,

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
        case 2: { QString _r = _t->getPath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->setPath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: { QString _r = _t->getDirectory();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->setDirectory((*reinterpret_cast< std::add_pointer_t<QDir>>(_a[1]))); break;
        case 6: _t->setDirectoryByStr((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: { QString _r = _t->getDuration();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->setDuration((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: { int _r = _t->getFrameRate();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->setFrameRate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: { int _r = _t->getBitRate();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->setBitRate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 13: { float _r = _t->getVideoSize();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->setVideoSize((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 15: { int _r = _t->getVideoWidth();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->setVideoWidth((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: { int _r = _t->getVideoHeight();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 18: _t->setVideoHeight((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 19: { QString _r = _t->getVideoFormat();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->setVideoFormat((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: { QString _r = _t->getAudioFormat();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 22: _t->setAudioFormat((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: { int _r = _t->getAudioAverageBitRate();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->setAudioAverageBitRate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: { int _r = _t->getChannelNumbers();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 26: _t->setChannelNumbers((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 27: { int _r = _t->getSampleRate();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 28: _t->setSampleRate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 29: { QString _r = _t->getFormat();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 30: _t->setFormat((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 31: { float _r = _t->getAudioSize();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = std::move(_r); }  break;
        case 32: _t->setAudioSize((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 33: { int _r = _t->getStreamNumbers();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 34: _t->setStreamNumbers((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
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
        case 1: *reinterpret_cast< QString*>(_v) = _t->getFileName(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getDirectory(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getDuration(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->getFrameRate(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->getBitRate(); break;
        case 6: *reinterpret_cast< float*>(_v) = _t->getVideoSize(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->getVideoWidth(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->getVideoHeight(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->getVideoFormat(); break;
        case 10: *reinterpret_cast< QString*>(_v) = _t->getAudioFormat(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->getAudioAverageBitRate(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->getChannelNumbers(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->getSampleRate(); break;
        case 14: *reinterpret_cast< float*>(_v) = _t->getAudioSize(); break;
        case 15: *reinterpret_cast< QString*>(_v) = _t->getFormat(); break;
        case 16: *reinterpret_cast< QString*>(_v) = _t->getPath(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<PlayListItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setUUID(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setFileName(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setDirectoryByStr(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setDuration(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setFrameRate(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setBitRate(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setVideoSize(*reinterpret_cast< float*>(_v)); break;
        case 7: _t->setVideoWidth(*reinterpret_cast< int*>(_v)); break;
        case 8: _t->setVideoHeight(*reinterpret_cast< int*>(_v)); break;
        case 9: _t->setVideoFormat(*reinterpret_cast< QString*>(_v)); break;
        case 10: _t->setAudioFormat(*reinterpret_cast< QString*>(_v)); break;
        case 11: _t->setAudioAverageBitRate(*reinterpret_cast< int*>(_v)); break;
        case 12: _t->setChannelNumbers(*reinterpret_cast< int*>(_v)); break;
        case 13: _t->setSampleRate(*reinterpret_cast< int*>(_v)); break;
        case 14: _t->setAudioSize(*reinterpret_cast< float*>(_v)); break;
        case 15: _t->setFormat(*reinterpret_cast< QString*>(_v)); break;
        case 16: _t->setPath(*reinterpret_cast< QString*>(_v)); break;
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
, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<float, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<float, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<PlayListItem, std::true_type>

, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QDir, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>
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
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 35;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
struct qt_meta_stringdata_simpleListItem_t {
    const uint offsetsAndSize[30];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_simpleListItem_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_simpleListItem_t qt_meta_stringdata_simpleListItem = {
    {
QT_MOC_LITERAL(0, 14), // "simpleListItem"
QT_MOC_LITERAL(15, 11), // "getFileName"
QT_MOC_LITERAL(27, 0), // ""
QT_MOC_LITERAL(28, 11), // "setFileName"
QT_MOC_LITERAL(40, 9), // "_fileName"
QT_MOC_LITERAL(50, 11), // "getFilePath"
QT_MOC_LITERAL(62, 11), // "setFilePath"
QT_MOC_LITERAL(74, 9), // "_filePath"
QT_MOC_LITERAL(84, 11), // "getIconPath"
QT_MOC_LITERAL(96, 11), // "setIconPath"
QT_MOC_LITERAL(108, 9), // "_iconPath"
QT_MOC_LITERAL(118, 5), // "other"
QT_MOC_LITERAL(124, 8), // "fileName"
QT_MOC_LITERAL(133, 8), // "filePath"
QT_MOC_LITERAL(142, 8) // "iconPath"

    },
    "simpleListItem\0getFileName\0\0setFileName\0"
    "_fileName\0getFilePath\0setFilePath\0"
    "_filePath\0getIconPath\0setIconPath\0"
    "_iconPath\0other\0fileName\0filePath\0"
    "iconPath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_simpleListItem[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       3,   73, // properties
       0,    0, // enums/sets
       3,   88, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x02,    4 /* Public */,
       3,    1,   51,    2, 0x02,    5 /* Public */,
       5,    0,   54,    2, 0x02,    7 /* Public */,
       6,    1,   55,    2, 0x02,    8 /* Public */,
       8,    0,   58,    2, 0x02,   10 /* Public */,
       9,    1,   59,    2, 0x02,   11 /* Public */,

 // methods: parameters
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,   10,

 // constructors: parameters
    0x80000000 | 2,
    0x80000000 | 2, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    7,   10,
    0x80000000 | 2, 0x80000000 | 0,   11,

 // properties: name, type, flags
      12, QMetaType::QString, 0x00015103, uint(-1), 0,
      13, QMetaType::QString, 0x00015103, uint(-1), 0,
      14, QMetaType::QString, 0x00015103, uint(-1), 0,

 // constructors: name, argc, parameters, tag, flags, initial metatype offsets
       0,    0,   62,    2, 0x0e,   13 /* Public */,
       0,    3,   63,    2, 0x0e,   13 /* Public */,
       0,    1,   70,    2, 0x0e,   16 /* Public */,

       0        // eod
};

void simpleListItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { simpleListItem *_r = new simpleListItem();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 1: { simpleListItem *_r = new simpleListItem((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 2: { simpleListItem *_r = new simpleListItem((*reinterpret_cast< std::add_pointer_t<simpleListItem>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    } else if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<simpleListItem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { QString _r = _t->getFileName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->setFileName((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: { QString _r = _t->getFilePath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->setFilePath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: { QString _r = _t->getIconPath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->setIconPath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<simpleListItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getFileName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getFilePath(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getIconPath(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<simpleListItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setFileName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setFilePath(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setIconPath(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject simpleListItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_simpleListItem.offsetsAndSize,
    qt_meta_data_simpleListItem,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_simpleListItem_t
, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<simpleListItem, std::true_type>

, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<const simpleListItem &, std::false_type>
>,
    nullptr
} };


const QMetaObject *simpleListItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *simpleListItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_simpleListItem.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int simpleListItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
struct qt_meta_stringdata_PlayList_t {
    const uint offsetsAndSize[40];
    char stringdata0[189];
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
QT_MOC_LITERAL(73, 11), // "extractDone"
QT_MOC_LITERAL(85, 22), // "QList<simpleListItem*>"
QT_MOC_LITERAL(108, 3), // "res"
QT_MOC_LITERAL(112, 11), // "getInfoDone"
QT_MOC_LITERAL(124, 6), // "insert"
QT_MOC_LITERAL(131, 6), // "remove"
QT_MOC_LITERAL(138, 8), // "filepath"
QT_MOC_LITERAL(147, 6), // "search"
QT_MOC_LITERAL(154, 3), // "key"
QT_MOC_LITERAL(158, 17), // "extractAndProcess"
QT_MOC_LITERAL(176, 7), // "getInfo"
QT_MOC_LITERAL(184, 4) // "path"

    },
    "PlayList\0insertDone\0\0resultcode\0"
    "removeDone\0searchDone\0PlayListItem*\0"
    "item\0extractDone\0QList<simpleListItem*>\0"
    "res\0getInfoDone\0insert\0remove\0filepath\0"
    "search\0key\0extractAndProcess\0getInfo\0"
    "path"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlayList[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,
       4,    1,   77,    2, 0x06,    3 /* Public */,
       5,    1,   80,    2, 0x06,    5 /* Public */,
       8,    1,   83,    2, 0x06,    7 /* Public */,
      11,    1,   86,    2, 0x06,    9 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    1,   89,    2, 0x0a,   11 /* Public */,
      13,    1,   92,    2, 0x0a,   13 /* Public */,
      15,    1,   95,    2, 0x0a,   15 /* Public */,
      17,    0,   98,    2, 0x0a,   17 /* Public */,
      18,    1,   99,    2, 0x0a,   18 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QString,   14,
    0x80000000 | 6, QMetaType::QString,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   19,

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
        case 3: _t->extractDone((*reinterpret_cast< std::add_pointer_t<QList<simpleListItem*>>>(_a[1]))); break;
        case 4: _t->getInfoDone((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 5: _t->insert((*reinterpret_cast< std::add_pointer_t<PlayListItem*>>(_a[1]))); break;
        case 6: _t->remove((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: { PlayListItem* _r = _t->search((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< PlayListItem**>(_a[0]) = std::move(_r); }  break;
        case 8: _t->extractAndProcess(); break;
        case 9: _t->getInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
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
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<simpleListItem*> >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PlayListItem* >(); break;
            }
            break;
        case 5:
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
        {
            using _t = void (PlayList::*)(QList<simpleListItem*> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayList::extractDone)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (PlayList::*)(PlayListItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayList::getInfoDone)) {
                *result = 4;
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
, QtPrivate::TypeAndForceComplete<PlayList, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QList<simpleListItem*>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<PlayListItem *, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>


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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
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

// SIGNAL 3
void PlayList::extractDone(QList<simpleListItem*> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void PlayList::getInfoDone(PlayListItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
