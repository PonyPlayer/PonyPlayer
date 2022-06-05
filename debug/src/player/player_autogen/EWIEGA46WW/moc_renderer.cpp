/****************************************************************************
** Meta object code from reading C++ file 'renderer.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/player/renderer.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renderer.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FireworksRenderer_t {
    const uint offsetsAndSize[18];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_FireworksRenderer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_FireworksRenderer_t qt_meta_stringdata_FireworksRenderer = {
    {
QT_MOC_LITERAL(0, 17), // "FireworksRenderer"
QT_MOC_LITERAL(18, 4), // "init"
QT_MOC_LITERAL(23, 0), // ""
QT_MOC_LITERAL(24, 4), // "sync"
QT_MOC_LITERAL(29, 10), // "brightness"
QT_MOC_LITERAL(40, 7), // "GLfloat"
QT_MOC_LITERAL(48, 8), // "contrast"
QT_MOC_LITERAL(57, 10), // "saturation"
QT_MOC_LITERAL(68, 13) // "keepFrameRate"

    },
    "FireworksRenderer\0init\0\0sync\0brightness\0"
    "GLfloat\0contrast\0saturation\0keepFrameRate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FireworksRenderer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       4,   28, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x0a,    5 /* Public */,
       3,    0,   27,    2, 0x0a,    6 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       4, 0x80000000 | 5, 0x0001510b, uint(-1), 0,
       6, 0x80000000 | 5, 0x0001510b, uint(-1), 0,
       7, 0x80000000 | 5, 0x0001510b, uint(-1), 0,
       8, QMetaType::Bool, 0x00015103, uint(-1), 0,

       0        // eod
};

void FireworksRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FireworksRenderer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->sync(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<FireworksRenderer *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< GLfloat*>(_v) = _t->getBrightness(); break;
        case 1: *reinterpret_cast< GLfloat*>(_v) = _t->getContrast(); break;
        case 2: *reinterpret_cast< GLfloat*>(_v) = _t->getSaturation(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isKeepFrameRate(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<FireworksRenderer *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBrightness(*reinterpret_cast< GLfloat*>(_v)); break;
        case 1: _t->setContrast(*reinterpret_cast< GLfloat*>(_v)); break;
        case 2: _t->setSaturation(*reinterpret_cast< GLfloat*>(_v)); break;
        case 3: _t->setKeepFrameRate(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
    (void)_a;
}

const QMetaObject FireworksRenderer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_FireworksRenderer.offsetsAndSize,
    qt_meta_data_FireworksRenderer,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_FireworksRenderer_t
, QtPrivate::TypeAndForceComplete<GLfloat, std::true_type>, QtPrivate::TypeAndForceComplete<GLfloat, std::true_type>, QtPrivate::TypeAndForceComplete<GLfloat, std::true_type>, QtPrivate::TypeAndForceComplete<bool, std::true_type>, QtPrivate::TypeAndForceComplete<FireworksRenderer, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *FireworksRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FireworksRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FireworksRenderer.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QSGRenderNode"))
        return static_cast< QSGRenderNode*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions_3_3_Core"))
        return static_cast< QOpenGLFunctions_3_3_Core*>(this);
    return QObject::qt_metacast(_clname);
}

int FireworksRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
