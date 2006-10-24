/****************************************************************************
** Meta object code from reading C++ file 'ExController.h'
**
** Created: Tue Oct 24 19:29:32 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ExController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ExController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ExController[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   34,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ExController[] = {
    "ExController\0\0myUsefulSignal(int)\0index\0updateList(QModelIndex)\0"
};

const QMetaObject ExController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ExController,
      qt_meta_data_ExController, 0 }
};

const QMetaObject *ExController::metaObject() const
{
    return &staticMetaObject;
}

void *ExController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExController))
	return static_cast<void*>(const_cast<ExController*>(this));
    return QObject::qt_metacast(_clname);
}

int ExController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: myUsefulSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: updateList((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ExController::myUsefulSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
