/****************************************************************************
** Meta object code from reading C++ file 'ViewController.hpp'
**
** Created: Tue Oct 24 16:46:29 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ViewController.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ViewController.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ViewController[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ViewController[] = {
    "ViewController\0\0index\0updateText(QModelIndex)\0"
};

const QMetaObject ViewController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ViewController,
      qt_meta_data_ViewController, 0 }
};

const QMetaObject *ViewController::metaObject() const
{
    return &staticMetaObject;
}

void *ViewController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ViewController))
	return static_cast<void*>(const_cast<ViewController*>(this));
    return QObject::qt_metacast(_clname);
}

int ViewController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateText((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
