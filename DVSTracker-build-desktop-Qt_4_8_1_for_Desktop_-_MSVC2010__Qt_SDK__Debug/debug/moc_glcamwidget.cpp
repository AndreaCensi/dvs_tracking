/****************************************************************************
** Meta object code from reading C++ file 'glcamwidget.h'
**
** Created: Fri 25. Jan 15:20:04 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DVSTracker/glcamwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glcamwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GLCamWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   13,   12,   12, 0x0a,
      45,   35,   12,   12, 0x0a,
      80,   66,   12,   12, 0x0a,
     113,  105,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GLCamWidget[] = {
    "GLCamWidget\0\0e\0updateImage(Event*)\0"
    "from,size\0updateImage(int,int)\0"
    "x,y,greyValue\0updateImage(int,int,int)\0"
    "x,y,w,i\0updateImage(int,int,int,int)\0"
};

void GLCamWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GLCamWidget *_t = static_cast<GLCamWidget *>(_o);
        switch (_id) {
        case 0: _t->updateImage((*reinterpret_cast< Event*(*)>(_a[1]))); break;
        case 1: _t->updateImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->updateImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->updateImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GLCamWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GLCamWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLCamWidget,
      qt_meta_data_GLCamWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GLCamWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GLCamWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GLCamWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLCamWidget))
        return static_cast<void*>(const_cast< GLCamWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GLCamWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
