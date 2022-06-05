/****************************************************************************
** Meta object code from reading C++ file 'wave.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/wave/wave.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wave.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WaveView_t {
    const uint offsetsAndSize[42];
    char stringdata0[309];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_WaveView_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_WaveView_t qt_meta_stringdata_WaveView = {
    {
QT_MOC_LITERAL(0, 8), // "WaveView"
QT_MOC_LITERAL(9, 11), // "QML.Element"
QT_MOC_LITERAL(21, 4), // "auto"
QT_MOC_LITERAL(26, 23), // "signalPushLyricSentence"
QT_MOC_LITERAL(50, 0), // ""
QT_MOC_LITERAL(51, 14), // "LyricSentence*"
QT_MOC_LITERAL(66, 13), // "lyricSentence"
QT_MOC_LITERAL(80, 24), // "signalClearLyricSentence"
QT_MOC_LITERAL(105, 17), // "notifyRadiusRatio"
QT_MOC_LITERAL(123, 32), // "signalPushLyricSentenceCompleted"
QT_MOC_LITERAL(156, 14), // "setRadiusRatio"
QT_MOC_LITERAL(171, 11), // "radiusRatio"
QT_MOC_LITERAL(183, 18), // "readLyricsResponse"
QT_MOC_LITERAL(202, 28), // "std::shared_ptr<lrc::Lyrics>"
QT_MOC_LITERAL(231, 6), // "lyrics"
QT_MOC_LITERAL(238, 10), // "readLyrics"
QT_MOC_LITERAL(249, 8), // "filePath"
QT_MOC_LITERAL(258, 13), // "tryLoadLyrics"
QT_MOC_LITERAL(272, 14), // "getRadiusRatio"
QT_MOC_LITERAL(287, 6), // "player"
QT_MOC_LITERAL(294, 14) // "lyricsDataName"

    },
    "WaveView\0QML.Element\0auto\0"
    "signalPushLyricSentence\0\0LyricSentence*\0"
    "lyricSentence\0signalClearLyricSentence\0"
    "notifyRadiusRatio\0signalPushLyricSentenceCompleted\0"
    "setRadiusRatio\0radiusRatio\0"
    "readLyricsResponse\0std::shared_ptr<lrc::Lyrics>\0"
    "lyrics\0readLyrics\0filePath\0tryLoadLyrics\0"
    "getRadiusRatio\0player\0lyricsDataName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WaveView[] = {

 // content:
      10,       // revision
       0,       // classname
       1,   14, // classinfo
       9,   16, // methods
       3,   89, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   70,    4, 0x06,    4 /* Public */,
       7,    0,   73,    4, 0x06,    6 /* Public */,
       8,    0,   74,    4, 0x06,    7 /* Public */,
       9,    0,   75,    4, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      10,    1,   76,    4, 0x0a,    9 /* Public */,
      12,    1,   79,    4, 0x0a,   11 /* Public */,
      15,    1,   82,    4, 0x0a,   13 /* Public */,
      17,    1,   85,    4, 0x0a,   15 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      18,    0,   88,    4, 0x102,   17 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Bool, QMetaType::QString,   16,
    QMetaType::Bool, QMetaType::QString,   16,

 // methods: parameters
    QMetaType::Float,

 // properties: name, type, flags
      11, QMetaType::Float, 0x00015103, uint(2), 0,
      19, QMetaType::QString, 0x00015103, uint(-1), 0,
      20, QMetaType::QString, 0x00015103, uint(-1), 0,

       0        // eod
};

void WaveView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WaveView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalPushLyricSentence((*reinterpret_cast< std::add_pointer_t<LyricSentence*>>(_a[1]))); break;
        case 1: _t->signalClearLyricSentence(); break;
        case 2: _t->notifyRadiusRatio(); break;
        case 3: _t->signalPushLyricSentenceCompleted(); break;
        case 4: _t->setRadiusRatio((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 5: _t->readLyricsResponse((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<lrc::Lyrics>>>(_a[1]))); break;
        case 6: { bool _r = _t->readLyrics((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->tryLoadLyrics((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { float _r = _t->getRadiusRatio();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
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
            using _t = void (WaveView::*)(LyricSentence * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WaveView::signalPushLyricSentence)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WaveView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WaveView::signalClearLyricSentence)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WaveView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WaveView::notifyRadiusRatio)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WaveView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WaveView::signalPushLyricSentenceCompleted)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<WaveView *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = _t->getRadiusRatio(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getPlayer(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getLyricsDataName(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<WaveView *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setRadiusRatio(*reinterpret_cast< float*>(_v)); break;
        case 1: _t->setPlayer(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setLyricsDataName(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject WaveView::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickItem::staticMetaObject>(),
    qt_meta_stringdata_WaveView.offsetsAndSize,
    qt_meta_data_WaveView,
    qt_static_metacall,
    nullptr,
qt_metaTypeArray<
float, QString, QString, WaveView, void, LyricSentence *, void, void, void
, void, float, void, std::shared_ptr<lrc::Lyrics>, bool, const QString &, bool, const QString &
, float

>,
    nullptr
} };


const QMetaObject *WaveView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WaveView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WaveView.stringdata0))
        return static_cast<void*>(this);
    return QQuickItem::qt_metacast(_clname);
}

int WaveView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
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
void WaveView::signalPushLyricSentence(LyricSentence * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WaveView::signalClearLyricSentence()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void WaveView::notifyRadiusRatio()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void WaveView::signalPushLyricSentenceCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
