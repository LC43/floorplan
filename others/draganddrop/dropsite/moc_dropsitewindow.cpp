/****************************************************************************
** Meta object code from reading C++ file 'dropsitewindow.h'
**
** Created: Mon Jan 19 10:27:46 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dropsitewindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dropsitewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DropSiteWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      25,   16,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DropSiteWindow[] = {
    "DropSiteWindow\0\0mimeData\0"
    "updateFormatsTable(const QMimeData*)\0"
};

const QMetaObject DropSiteWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DropSiteWindow,
      qt_meta_data_DropSiteWindow, 0 }
};

const QMetaObject *DropSiteWindow::metaObject() const
{
    return &staticMetaObject;
}

void *DropSiteWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DropSiteWindow))
        return static_cast<void*>(const_cast< DropSiteWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int DropSiteWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateFormatsTable((*reinterpret_cast< const QMimeData*(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
