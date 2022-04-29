/****************************************************************************
** Meta object code from reading C++ file 'hurricane.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../include/hurricane.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hurricane.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HurricaneRenderer_t {
    const uint offsetsAndSize[6];
    char stringdata0[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_HurricaneRenderer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_HurricaneRenderer_t qt_meta_stringdata_HurricaneRenderer = {
    {
QT_MOC_LITERAL(0, 17), // "HurricaneRenderer"
QT_MOC_LITERAL(18, 4), // "init"
QT_MOC_LITERAL(23, 0) // ""

    },
    "HurricaneRenderer\0init\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HurricaneRenderer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x0a,    1 /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void HurricaneRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HurricaneRenderer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->init(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject HurricaneRenderer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_HurricaneRenderer.offsetsAndSize,
    qt_meta_data_HurricaneRenderer,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_HurricaneRenderer_t
, QtPrivate::TypeAndForceComplete<HurricaneRenderer, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *HurricaneRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HurricaneRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HurricaneRenderer.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QSGRenderNode"))
        return static_cast< QSGRenderNode*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions_3_3_Core"))
        return static_cast< QOpenGLFunctions_3_3_Core*>(this);
    return QObject::qt_metacast(_clname);
}

int HurricaneRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_Hurricane_t {
    const uint offsetsAndSize[24];
    char stringdata0[107];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Hurricane_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Hurricane_t qt_meta_stringdata_Hurricane = {
    {
QT_MOC_LITERAL(0, 9), // "Hurricane"
QT_MOC_LITERAL(10, 11), // "QML.Element"
QT_MOC_LITERAL(22, 4), // "auto"
QT_MOC_LITERAL(27, 19), // "handleWindowChanged"
QT_MOC_LITERAL(47, 0), // ""
QT_MOC_LITERAL(48, 13), // "QQuickWindow*"
QT_MOC_LITERAL(62, 3), // "win"
QT_MOC_LITERAL(66, 4), // "sync"
QT_MOC_LITERAL(71, 14), // "cleanupPicture"
QT_MOC_LITERAL(86, 8), // "setImage"
QT_MOC_LITERAL(95, 7), // "Picture"
QT_MOC_LITERAL(103, 3) // "pic"

    },
    "Hurricane\0QML.Element\0auto\0"
    "handleWindowChanged\0\0QQuickWindow*\0"
    "win\0sync\0cleanupPicture\0setImage\0"
    "Picture\0pic"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Hurricane[] = {

 // content:
      10,       // revision
       0,       // classname
       1,   14, // classinfo
       4,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   40,    4, 0x0a,    1 /* Public */,
       7,    0,   43,    4, 0x0a,    3 /* Public */,
       8,    0,   44,    4, 0x0a,    4 /* Public */,
       9,    1,   45,    4, 0x0a,    5 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void Hurricane::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Hurricane *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->handleWindowChanged((*reinterpret_cast< std::add_pointer_t<QQuickWindow*>>(_a[1]))); break;
        case 1: _t->sync(); break;
        case 2: _t->cleanupPicture(); break;
        case 3: _t->setImage((*reinterpret_cast< std::add_pointer_t<Picture>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QQuickWindow* >(); break;
            }
            break;
        }
    }
}

const QMetaObject Hurricane::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickItem::staticMetaObject>(),
    qt_meta_stringdata_Hurricane.offsetsAndSize,
    qt_meta_data_Hurricane,
    qt_static_metacall,
    nullptr,
qt_metaTypeArray<
Hurricane
, void, QQuickWindow *, void, void, void, const Picture &


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
    return QQuickItem::qt_metacast(_clname);
}

int Hurricane::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
