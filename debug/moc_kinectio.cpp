/****************************************************************************
** Meta object code from reading C++ file 'kinectio.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../KinectShopApp/kinectio.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kinectio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_kinectio_t {
    QByteArrayData data[7];
    char stringdata[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_kinectio_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_kinectio_t qt_meta_stringdata_kinectio = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 17),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 21),
QT_MOC_LITERAL(4, 50, 3),
QT_MOC_LITERAL(5, 54, 14),
QT_MOC_LITERAL(6, 69, 14)
    },
    "kinectio\0kinectInitialized\0\0"
    "kinectProductDetected\0pid\0setDetectedPid\0"
    "getDetectedPid\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_kinectio[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06,
       3,    1,   35,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    1,   38,    2, 0x0a,
       6,    0,   41,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Int,

       0        // eod
};

void kinectio::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        kinectio *_t = static_cast<kinectio *>(_o);
        switch (_id) {
        case 0: _t->kinectInitialized(); break;
        case 1: _t->kinectProductDetected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setDetectedPid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: { int _r = _t->getDetectedPid();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (kinectio::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&kinectio::kinectInitialized)) {
                *result = 0;
            }
        }
        {
            typedef void (kinectio::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&kinectio::kinectProductDetected)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject kinectio::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_kinectio.data,
      qt_meta_data_kinectio,  qt_static_metacall, 0, 0}
};


const QMetaObject *kinectio::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *kinectio::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_kinectio.stringdata))
        return static_cast<void*>(const_cast< kinectio*>(this));
    return QObject::qt_metacast(_clname);
}

int kinectio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void kinectio::kinectInitialized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void kinectio::kinectProductDetected(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
