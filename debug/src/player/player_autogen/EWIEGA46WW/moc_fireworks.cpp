/****************************************************************************
** Meta object code from reading C++ file 'fireworks.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/player/fireworks.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fireworks.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Fireworks_t {
    const uint offsetsAndSize[48];
    char stringdata0[279];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Fireworks_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Fireworks_t qt_meta_stringdata_Fireworks = {
    {
QT_MOC_LITERAL(0, 9), // "Fireworks"
QT_MOC_LITERAL(10, 11), // "QML.Element"
QT_MOC_LITERAL(22, 4), // "auto"
QT_MOC_LITERAL(27, 17), // "brightnessChanged"
QT_MOC_LITERAL(45, 0), // ""
QT_MOC_LITERAL(46, 15), // "contrastChanged"
QT_MOC_LITERAL(62, 17), // "saturationChanged"
QT_MOC_LITERAL(80, 16), // "frameSizeChanged"
QT_MOC_LITERAL(97, 20), // "keepFrameRateChanged"
QT_MOC_LITERAL(118, 13), // "setVideoFrame"
QT_MOC_LITERAL(132, 13), // "VideoFrameRef"
QT_MOC_LITERAL(146, 3), // "pic"
QT_MOC_LITERAL(150, 12), // "setLUTFilter"
QT_MOC_LITERAL(163, 4), // "path"
QT_MOC_LITERAL(168, 13), // "keepFrameRate"
QT_MOC_LITERAL(182, 11), // "frameHeight"
QT_MOC_LITERAL(194, 10), // "frameWidth"
QT_MOC_LITERAL(205, 9), // "frameRate"
QT_MOC_LITERAL(215, 10), // "brightness"
QT_MOC_LITERAL(226, 7), // "GLfloat"
QT_MOC_LITERAL(234, 8), // "contrast"
QT_MOC_LITERAL(243, 10), // "saturation"
QT_MOC_LITERAL(254, 12), // "filterPrefix"
QT_MOC_LITERAL(267, 11) // "filterJsons"

    },
    "Fireworks\0QML.Element\0auto\0brightnessChanged\0"
    "\0contrastChanged\0saturationChanged\0"
    "frameSizeChanged\0keepFrameRateChanged\0"
    "setVideoFrame\0VideoFrameRef\0pic\0"
    "setLUTFilter\0path\0keepFrameRate\0"
    "frameHeight\0frameWidth\0frameRate\0"
    "brightness\0GLfloat\0contrast\0saturation\0"
    "filterPrefix\0filterJsons"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fireworks[] = {

 // content:
      10,       // revision
       0,       // classname
       1,   14, // classinfo
       7,   16, // methods
       9,   69, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   58,    4, 0x06,   10 /* Public */,
       5,    0,   59,    4, 0x06,   11 /* Public */,
       6,    0,   60,    4, 0x06,   12 /* Public */,
       7,    0,   61,    4, 0x06,   13 /* Public */,
       8,    0,   62,    4, 0x06,   14 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    1,   63,    4, 0x0a,   15 /* Public */,
      12,    1,   66,    4, 0x0a,   17 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QString,   13,

 // properties: name, type, flags
      14, QMetaType::Bool, 0x00015103, uint(4), 0,
      15, QMetaType::Int, 0x00015001, uint(3), 0,
      16, QMetaType::Int, 0x00015001, uint(3), 0,
      17, QMetaType::Double, 0x00015001, uint(3), 0,
      18, 0x80000000 | 19, 0x0001510b, uint(0), 0,
      20, 0x80000000 | 19, 0x0001510b, uint(1), 0,
      21, 0x80000000 | 19, 0x0001510b, uint(2), 0,
      22, QMetaType::QString, 0x00015001, uint(-1), 0,
      23, QMetaType::QStringList, 0x00015001, uint(-1), 0,

       0        // eod
};

void Fireworks::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Fireworks *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->brightnessChanged(); break;
        case 1: _t->contrastChanged(); break;
        case 2: _t->saturationChanged(); break;
        case 3: _t->frameSizeChanged(); break;
        case 4: _t->keepFrameRateChanged(); break;
        case 5: _t->setVideoFrame((*reinterpret_cast< std::add_pointer_t<VideoFrameRef>>(_a[1]))); break;
        case 6: _t->setLUTFilter((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Fireworks::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Fireworks::brightnessChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Fireworks::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Fireworks::contrastChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Fireworks::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Fireworks::saturationChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Fireworks::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Fireworks::frameSizeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Fireworks::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Fireworks::keepFrameRateChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Fireworks *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isKeepFrameRate(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->getHeight(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->getWidth(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->getFrameRate(); break;
        case 4: *reinterpret_cast< GLfloat*>(_v) = _t->getBrightness(); break;
        case 5: *reinterpret_cast< GLfloat*>(_v) = _t->getContrast(); break;
        case 6: *reinterpret_cast< GLfloat*>(_v) = _t->getSaturation(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->getFilterPrefix(); break;
        case 8: *reinterpret_cast< QStringList*>(_v) = _t->getFilterJsons(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Fireworks *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setKeepFrameRate(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setBrightness(*reinterpret_cast< GLfloat*>(_v)); break;
        case 5: _t->setContrast(*reinterpret_cast< GLfloat*>(_v)); break;
        case 6: _t->setSaturation(*reinterpret_cast< GLfloat*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Fireworks::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickItem::staticMetaObject>(),
    qt_meta_stringdata_Fireworks.offsetsAndSize,
    qt_meta_data_Fireworks,
    qt_static_metacall,
    nullptr,
qt_metaTypeArray<
bool, int, int, double, GLfloat, GLfloat, GLfloat, QString, QStringList, Fireworks, void, void, void, void, void
, void, const VideoFrameRef &, void, const QString &


>,
    nullptr
} };


const QMetaObject *Fireworks::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fireworks::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Fireworks.stringdata0))
        return static_cast<void*>(this);
    return QQuickItem::qt_metacast(_clname);
}

int Fireworks::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Fireworks::brightnessChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Fireworks::contrastChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Fireworks::saturationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Fireworks::frameSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Fireworks::keepFrameRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
