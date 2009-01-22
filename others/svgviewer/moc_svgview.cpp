/****************************************************************************
** Meta object code from reading C++ file 'svgview.h'
**
** Created: Thu Jan 22 15:47:30 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "svgview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'svgview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SvgRasterView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_SvgRasterView[] = {
    "SvgRasterView\0\0poluteImage()\0"
};

const QMetaObject SvgRasterView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SvgRasterView,
      qt_meta_data_SvgRasterView, 0 }
};

const QMetaObject *SvgRasterView::metaObject() const
{
    return &staticMetaObject;
}

void *SvgRasterView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SvgRasterView))
        return static_cast<void*>(const_cast< SvgRasterView*>(this));
    return QWidget::qt_metacast(_clname);
}

int SvgRasterView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: poluteImage(); break;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_SvgNativeView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_SvgNativeView[] = {
    "SvgNativeView\0"
};

const QMetaObject SvgNativeView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SvgNativeView,
      qt_meta_data_SvgNativeView, 0 }
};

const QMetaObject *SvgNativeView::metaObject() const
{
    return &staticMetaObject;
}

void *SvgNativeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SvgNativeView))
        return static_cast<void*>(const_cast< SvgNativeView*>(this));
    return QWidget::qt_metacast(_clname);
}

int SvgNativeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SvgGLView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_SvgGLView[] = {
    "SvgGLView\0"
};

const QMetaObject SvgGLView::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_SvgGLView,
      qt_meta_data_SvgGLView, 0 }
};

const QMetaObject *SvgGLView::metaObject() const
{
    return &staticMetaObject;
}

void *SvgGLView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SvgGLView))
        return static_cast<void*>(const_cast< SvgGLView*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int SvgGLView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
