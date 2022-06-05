/****************************************************************************
** Meta object code from reading C++ file 'thumbnail.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/player/thumbnail.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'thumbnail.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Thumbnail_t {
    const uint offsetsAndSize[24];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Thumbnail_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Thumbnail_t qt_meta_stringdata_Thumbnail = {
    {
QT_MOC_LITERAL(0, 9), // "Thumbnail"
QT_MOC_LITERAL(10, 11), // "QML.Element"
QT_MOC_LITERAL(22, 4), // "auto"
QT_MOC_LITERAL(27, 15), // "previewResponse"
QT_MOC_LITERAL(43, 0), // ""
QT_MOC_LITERAL(44, 3), // "pos"
QT_MOC_LITERAL(48, 20), // "signalPreviewRequest"
QT_MOC_LITERAL(69, 14), // "previewRequest"
QT_MOC_LITERAL(84, 19), // "slotPreviewResponse"
QT_MOC_LITERAL(104, 13), // "VideoFrameRef"
QT_MOC_LITERAL(118, 5), // "frame"
QT_MOC_LITERAL(124, 6) // "player"

    },
    "Thumbnail\0QML.Element\0auto\0previewResponse\0"
    "\0pos\0signalPreviewRequest\0previewRequest\0"
    "slotPreviewResponse\0VideoFrameRef\0"
    "frame\0player"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Thumbnail[] = {

 // content:
      10,       // revision
       0,       // classname
       1,   14, // classinfo
       4,   16, // methods
       1,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   40,    4, 0x06,    2 /* Public */,
       6,    1,   43,    4, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    1,   46,    4, 0x0a,    6 /* Public */,
       8,    2,   49,    4, 0x08,    8 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal,    5,
    QMetaType::Void, QMetaType::QReal,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QReal,    5,
    QMetaType::Void, QMetaType::QReal, 0x80000000 | 9,    5,   10,

 // properties: name, type, flags
      11, QMetaType::QString, 0x00015103, uint(-1), 0,

       0        // eod
};

void Thumbnail::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Thumbnail *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->previewResponse((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 1: _t->signalPreviewRequest((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])), QPrivateSignal()); break;
        case 2: _t->previewRequest((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 3: _t->slotPreviewResponse((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<VideoFrameRef>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Thumbnail::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Thumbnail::previewResponse)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Thumbnail::*)(qreal , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Thumbnail::signalPreviewRequest)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Thumbnail *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getPlayer(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Thumbnail *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPlayer(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Thumbnail::staticMetaObject = { {
    QMetaObject::SuperData::link<Fireworks::staticMetaObject>(),
    qt_meta_stringdata_Thumbnail.offsetsAndSize,
    qt_meta_data_Thumbnail,
    qt_static_metacall,
    nullptr,
qt_metaTypeArray<
QString, Thumbnail, void, qreal, void, qreal
, void, qreal, void, qreal, const VideoFrameRef &


>,
    nullptr
} };


const QMetaObject *Thumbnail::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Thumbnail::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Thumbnail.stringdata0))
        return static_cast<void*>(this);
    return Fireworks::qt_metacast(_clname);
}

int Thumbnail::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Fireworks::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
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
void Thumbnail::previewResponse(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Thumbnail::signalPreviewRequest(qreal _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
