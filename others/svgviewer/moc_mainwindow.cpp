/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu Jan 22 15:47:28 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x0a,
      35,   11,   11,   11, 0x2a,
      53,   46,   11,   11, 0x0a,
      78,   75,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0path\0openFile(QString)\0"
    "openFile()\0action\0setRenderer(QAction*)\0"
    "hq\0setHighQualityAntialiasing(bool)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: openFile(); break;
        case 2: setRenderer((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 3: setHighQualityAntialiasing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
