/****************************************************************************
** Meta object code from reading C++ file 'lyrics.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/wave/include/lyrics.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lyrics.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LyricSentence_t {
    const uint offsetsAndSize[36];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_LyricSentence_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_LyricSentence_t qt_meta_stringdata_LyricSentence = {
    {
QT_MOC_LITERAL(0, 13), // "LyricSentence"
QT_MOC_LITERAL(14, 11), // "QML.Element"
QT_MOC_LITERAL(26, 4), // "auto"
QT_MOC_LITERAL(31, 16), // "startTimeChanged"
QT_MOC_LITERAL(48, 0), // ""
QT_MOC_LITERAL(49, 14), // "endTimeChanged"
QT_MOC_LITERAL(64, 15), // "sentenceChanged"
QT_MOC_LITERAL(80, 12), // "getStartTime"
QT_MOC_LITERAL(93, 10), // "getEndTime"
QT_MOC_LITERAL(104, 11), // "getSentence"
QT_MOC_LITERAL(116, 12), // "setStartTime"
QT_MOC_LITERAL(129, 7), // "newTime"
QT_MOC_LITERAL(137, 10), // "setEndTime"
QT_MOC_LITERAL(148, 11), // "setSentence"
QT_MOC_LITERAL(160, 11), // "newSentence"
QT_MOC_LITERAL(172, 9), // "startTime"
QT_MOC_LITERAL(182, 7), // "endTime"
QT_MOC_LITERAL(190, 8) // "sentence"

    },
    "LyricSentence\0QML.Element\0auto\0"
    "startTimeChanged\0\0endTimeChanged\0"
    "sentenceChanged\0getStartTime\0getEndTime\0"
    "getSentence\0setStartTime\0newTime\0"
    "setEndTime\0setSentence\0newSentence\0"
    "startTime\0endTime\0sentence"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LyricSentence[] = {

 // content:
      10,       // revision
       0,       // classname
       1,   14, // classinfo
       9,   16, // methods
       3,   93, // properties
       0,    0, // enums/sets
       2,  108, // constructors
       0,       // flags
       3,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   70,    4, 0x06,    4 /* Public */,
       5,    0,   71,    4, 0x06,    5 /* Public */,
       6,    0,   72,    4, 0x06,    6 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   73,    4, 0x102,    7 /* Public | MethodIsConst  */,
       8,    0,   74,    4, 0x102,    8 /* Public | MethodIsConst  */,
       9,    0,   75,    4, 0x102,    9 /* Public | MethodIsConst  */,
      10,    1,   76,    4, 0x02,   10 /* Public */,
      12,    1,   79,    4, 0x02,   12 /* Public */,
      13,    1,   82,    4, 0x02,   14 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::QReal,
    QMetaType::QReal,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QReal,   11,
    QMetaType::Void, QMetaType::QReal,   11,
    QMetaType::Void, QMetaType::QString,   14,

 // constructors: parameters
    0x80000000 | 4,
    0x80000000 | 4, QMetaType::QReal, QMetaType::QReal, QMetaType::QString,   15,   16,   17,

 // properties: name, type, flags
      15, QMetaType::QReal, 0x00015103, uint(0), 0,
      16, QMetaType::QReal, 0x00015103, uint(1), 0,
      17, QMetaType::QString, 0x00015103, uint(2), 0,

 // constructors: name, argc, parameters, tag, flags, initial metatype offsets
       0,    0,   85,    4, 0x0e,   16 /* Public */,
       0,    3,   86,    4, 0x0e,   16 /* Public */,

       0        // eod
};

void LyricSentence::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { LyricSentence *_r = new LyricSentence();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 1: { LyricSentence *_r = new LyricSentence((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    } else if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LyricSentence *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->startTimeChanged(); break;
        case 1: _t->endTimeChanged(); break;
        case 2: _t->sentenceChanged(); break;
        case 3: { qreal _r = _t->getStartTime();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 4: { qreal _r = _t->getEndTime();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->getSentence();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->setStartTime((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 7: _t->setEndTime((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 8: _t->setSentence((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LyricSentence::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LyricSentence::startTimeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LyricSentence::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LyricSentence::endTimeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LyricSentence::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LyricSentence::sentenceChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<LyricSentence *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->getStartTime(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = _t->getEndTime(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getSentence(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<LyricSentence *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setStartTime(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setEndTime(*reinterpret_cast< qreal*>(_v)); break;
        case 2: _t->setSentence(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject LyricSentence::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LyricSentence.offsetsAndSize,
    qt_meta_data_LyricSentence,
    qt_static_metacall,
    nullptr,
qt_metaTypeArray<
qreal, qreal, QString, LyricSentence, void, void, void

, qreal, qreal, QString, void, const qreal &, void, const qreal &, void, const QString &
, qreal, qreal, QString
>,
    nullptr
} };


const QMetaObject *LyricSentence::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LyricSentence::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LyricSentence.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LyricSentence::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
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

// SIGNAL 0
void LyricSentence::startTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void LyricSentence::endTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void LyricSentence::sentenceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_LyricsData_t {
    const uint offsetsAndSize[24];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_LyricsData_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_LyricsData_t qt_meta_stringdata_LyricsData = {
    {
QT_MOC_LITERAL(0, 10), // "LyricsData"
QT_MOC_LITERAL(11, 11), // "QML.Element"
QT_MOC_LITERAL(23, 4), // "auto"
QT_MOC_LITERAL(28, 12), // "lyricsChange"
QT_MOC_LITERAL(41, 0), // ""
QT_MOC_LITERAL(42, 18), // "clearLyricSentence"
QT_MOC_LITERAL(61, 17), // "pushLyricSentence"
QT_MOC_LITERAL(79, 14), // "LyricSentence*"
QT_MOC_LITERAL(94, 8), // "sentence"
QT_MOC_LITERAL(103, 26), // "pushLyricSentenceCompleted"
QT_MOC_LITERAL(130, 9), // "sentences"
QT_MOC_LITERAL(140, 31) // "QQmlListProperty<LyricSentence>"

    },
    "LyricsData\0QML.Element\0auto\0lyricsChange\0"
    "\0clearLyricSentence\0pushLyricSentence\0"
    "LyricSentence*\0sentence\0"
    "pushLyricSentenceCompleted\0sentences\0"
    "QQmlListProperty<LyricSentence>"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LyricsData[] = {

 // content:
      10,       // revision
       0,       // classname
       1,   14, // classinfo
       4,   16, // methods
       1,   46, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   40,    4, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   41,    4, 0x0a,    3 /* Public */,
       6,    1,   42,    4, 0x0a,    4 /* Public */,
       9,    0,   45,    4, 0x0a,    6 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

 // properties: name, type, flags
      10, 0x80000000 | 11, 0x00015009, uint(0), 0,

       0        // eod
};

void LyricsData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LyricsData *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->lyricsChange(); break;
        case 1: _t->clearLyricSentence(); break;
        case 2: _t->pushLyricSentence((*reinterpret_cast< std::add_pointer_t<LyricSentence*>>(_a[1]))); break;
        case 3: _t->pushLyricSentenceCompleted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< LyricSentence* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LyricsData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LyricsData::lyricsChange)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<LyricsData *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QQmlListProperty<LyricSentence>*>(_v) = _t->getSentences(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject LyricsData::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LyricsData.offsetsAndSize,
    qt_meta_data_LyricsData,
    qt_static_metacall,
    nullptr,
qt_metaTypeArray<
QQmlListProperty<LyricSentence>, LyricsData, void
, void, void, LyricSentence *, void


>,
    nullptr
} };


const QMetaObject *LyricsData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LyricsData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LyricsData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LyricsData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
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
void LyricsData::lyricsChange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_LyricsReader_t {
    const uint offsetsAndSize[14];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_LyricsReader_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_LyricsReader_t qt_meta_stringdata_LyricsReader = {
    {
QT_MOC_LITERAL(0, 12), // "LyricsReader"
QT_MOC_LITERAL(13, 18), // "lyricReadCompleted"
QT_MOC_LITERAL(32, 0), // ""
QT_MOC_LITERAL(33, 28), // "std::shared_ptr<lrc::Lyrics>"
QT_MOC_LITERAL(62, 8), // "response"
QT_MOC_LITERAL(71, 9), // "readLyric"
QT_MOC_LITERAL(81, 3) // "url"

    },
    "LyricsReader\0lyricReadCompleted\0\0"
    "std::shared_ptr<lrc::Lyrics>\0response\0"
    "readLyric\0url"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LyricsReader[] = {

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
       1,    1,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   29,    2, 0x0a,    3 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void LyricsReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LyricsReader *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->lyricReadCompleted((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<lrc::Lyrics>>>(_a[1]))); break;
        case 1: _t->readLyric((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LyricsReader::*)(std::shared_ptr<lrc::Lyrics> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LyricsReader::lyricReadCompleted)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject LyricsReader::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LyricsReader.offsetsAndSize,
    qt_meta_data_LyricsReader,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_LyricsReader_t
, QtPrivate::TypeAndForceComplete<LyricsReader, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::shared_ptr<lrc::Lyrics>, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>


>,
    nullptr
} };


const QMetaObject *LyricsReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LyricsReader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LyricsReader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LyricsReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void LyricsReader::lyricReadCompleted(std::shared_ptr<lrc::Lyrics> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
