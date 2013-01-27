/****************************************************************************
** Meta object code from reading C++ file 'camwidget.h'
**
** Created: Fri 25. Jan 15:20:03 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DVSTracker/camwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CamWidget[] = {

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
      13,   11,   10,   10, 0x0a,
      47,   33,   10,   10, 0x0a,
      80,   72,   10,   10, 0x0a,
     113,  109,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CamWidget[] = {
    "CamWidget\0\0e\0updateImage(Event*)\0"
    "x,y,greyValue\0updateImage(int,int,int)\0"
    "x,y,w,i\0updateImage(int,int,int,int)\0"
    "i,p\0updateMaxWeightParticle(int,Particle*)\0"
};

void CamWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CamWidget *_t = static_cast<CamWidget *>(_o);
        switch (_id) {
        case 0: _t->updateImage((*reinterpret_cast< Event*(*)>(_a[1]))); break;
        case 1: _t->updateImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->updateImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->updateMaxWeightParticle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Particle*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CamWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CamWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CamWidget,
      qt_meta_data_CamWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CamWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CamWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CamWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CamWidget))
        return static_cast<void*>(const_cast< CamWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CamWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
