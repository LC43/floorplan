/****************************************************************************
** Meta object code from reading C++ file 'pieceslist.h'
**
** Created: Mon Jan 19 11:23:52 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "pieceslist.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pieceslist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PiecesList[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_PiecesList[] = {
    "PiecesList\0"
};

const QMetaObject PiecesList::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_PiecesList,
      qt_meta_data_PiecesList, 0 }
};

const QMetaObject *PiecesList::metaObject() const
{
    return &staticMetaObject;
}

void *PiecesList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PiecesList))
        return static_cast<void*>(const_cast< PiecesList*>(this));
    return QListWidget::qt_metacast(_clname);
}

int PiecesList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
