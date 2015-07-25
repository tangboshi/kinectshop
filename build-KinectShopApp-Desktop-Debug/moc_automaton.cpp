/****************************************************************************
** Meta object code from reading C++ file 'automaton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../KinectShopApp/automaton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'automaton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_automaton_t {
    QByteArrayData data[18];
    char stringdata[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_automaton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_automaton_t qt_meta_stringdata_automaton = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 12),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 5),
QT_MOC_LITERAL(4, 30, 9),
QT_MOC_LITERAL(5, 40, 2),
QT_MOC_LITERAL(6, 43, 13),
QT_MOC_LITERAL(7, 57, 6),
QT_MOC_LITERAL(8, 64, 13),
QT_MOC_LITERAL(9, 78, 5),
QT_MOC_LITERAL(10, 84, 4),
QT_MOC_LITERAL(11, 89, 19),
QT_MOC_LITERAL(12, 109, 11),
QT_MOC_LITERAL(13, 121, 12),
QT_MOC_LITERAL(14, 134, 11),
QT_MOC_LITERAL(15, 146, 18),
QT_MOC_LITERAL(16, 165, 14),
QT_MOC_LITERAL(17, 180, 18)
    },
    "automaton\0stateChanged\0\0input\0idChanged\0"
    "id\0amountChanged\0amount\0inputReceived\0"
    "state\0from\0transitionToExecute\0"
    "transition*\0toBeExecuted\0transitions\0"
    "updateStatusViewer\0updateIdViewer\0"
    "updateAmountViewer\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_automaton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06,
       4,    1,   62,    2, 0x06,
       6,    1,   65,    2, 0x06,
       8,    2,   68,    2, 0x06,
      11,    1,   73,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
      14,    1,   76,    2, 0x0a,
      15,    0,   79,    2, 0x0a,
      16,    0,   80,    2, 0x0a,
      17,    0,   81,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 9,    3,   10,
    QMetaType::Void, 0x80000000 | 12,   13,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,

       0        // eod
};

void automaton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        automaton *_t = static_cast<automaton *>(_o);
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->idChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->amountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->inputReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< state(*)>(_a[2]))); break;
        case 4: _t->transitionToExecute((*reinterpret_cast< transition*(*)>(_a[1]))); break;
        case 5: _t->transitions((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: { QString _r = _t->updateStatusViewer();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->updateIdViewer();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { QString _r = _t->updateAmountViewer();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (automaton::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&automaton::stateChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (automaton::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&automaton::idChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (automaton::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&automaton::amountChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (automaton::*_t)(int , state );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&automaton::inputReceived)) {
                *result = 3;
            }
        }
        {
            typedef void (automaton::*_t)(transition * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&automaton::transitionToExecute)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject automaton::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_automaton.data,
      qt_meta_data_automaton,  qt_static_metacall, 0, 0}
};


const QMetaObject *automaton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *automaton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_automaton.stringdata))
        return static_cast<void*>(const_cast< automaton*>(this));
    return QObject::qt_metacast(_clname);
}

int automaton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void automaton::stateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void automaton::idChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void automaton::amountChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void automaton::inputReceived(int _t1, state _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void automaton::transitionToExecute(transition * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
