/****************************************************************************
** Meta object code from reading C++ file 'demuxer.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/decoder/demuxer.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'demuxer.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Demuxer_t {
    const uint offsetsAndSize[34];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Demuxer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Demuxer_t qt_meta_stringdata_Demuxer = {
    {
QT_MOC_LITERAL(0, 7), // "Demuxer"
QT_MOC_LITERAL(8, 14), // "openFileResult"
QT_MOC_LITERAL(23, 0), // ""
QT_MOC_LITERAL(24, 30), // "PonyPlayer::OpenFileResultType"
QT_MOC_LITERAL(55, 6), // "result"
QT_MOC_LITERAL(62, 4), // "seek"
QT_MOC_LITERAL(67, 4), // "secs"
QT_MOC_LITERAL(72, 13), // "setAudioIndex"
QT_MOC_LITERAL(86, 11), // "StreamIndex"
QT_MOC_LITERAL(98, 5), // "index"
QT_MOC_LITERAL(104, 8), // "openFile"
QT_MOC_LITERAL(113, 11), // "std::string"
QT_MOC_LITERAL(125, 2), // "fn"
QT_MOC_LITERAL(128, 8), // "backward"
QT_MOC_LITERAL(137, 7), // "forward"
QT_MOC_LITERAL(145, 5), // "close"
QT_MOC_LITERAL(151, 11) // "test_onWork"

    },
    "Demuxer\0openFileResult\0\0"
    "PonyPlayer::OpenFileResultType\0result\0"
    "seek\0secs\0setAudioIndex\0StreamIndex\0"
    "index\0openFile\0std::string\0fn\0backward\0"
    "forward\0close\0test_onWork"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Demuxer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   65,    2, 0x0a,    3 /* Public */,
       7,    1,   68,    2, 0x0a,    5 /* Public */,
      10,    1,   71,    2, 0x0a,    7 /* Public */,
      13,    0,   74,    2, 0x0a,    9 /* Public */,
      14,    0,   75,    2, 0x0a,   10 /* Public */,
      15,    0,   76,    2, 0x0a,   11 /* Public */,
      16,    0,   77,    2, 0x0a,   12 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QReal,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Demuxer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Demuxer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->openFileResult((*reinterpret_cast< std::add_pointer_t<PonyPlayer::OpenFileResultType>>(_a[1])), QPrivateSignal()); break;
        case 1: _t->seek((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 2: _t->setAudioIndex((*reinterpret_cast< std::add_pointer_t<StreamIndex>>(_a[1]))); break;
        case 3: _t->openFile((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 4: _t->backward(); break;
        case 5: _t->forward(); break;
        case 6: _t->close(); break;
        case 7: _t->test_onWork(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Demuxer::*)(PonyPlayer::OpenFileResultType , QPrivateSignal);
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Demuxer::openFileResult)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Demuxer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Demuxer.offsetsAndSize,
    qt_meta_data_Demuxer,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Demuxer_t
, QtPrivate::TypeAndForceComplete<Demuxer, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<PonyPlayer::OpenFileResultType, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qreal, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<StreamIndex, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const std::string &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Demuxer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Demuxer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Demuxer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Demuxer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Demuxer::openFileResult(PonyPlayer::OpenFileResultType _t1, QPrivateSignal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
