/****************************************************************************
** Meta object code from reading C++ file 'customgraphicsview.h'
**
** Created: Thu Mar 8 19:40:00 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../customgraphicsview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customgraphicsview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CustomGraphicsView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   19,   19,   19, 0x0a,
      87,   72,   19,   19, 0x0a,
     115,   19,   19,   19, 0x2a,
     149,  139,   19,   19, 0x0a,
     166,   19,   19,   19, 0x0a,
     192,   19,   19,   19, 0x0a,
     210,  205,   19,   19, 0x0a,
     250,  239,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CustomGraphicsView[] = {
    "CustomGraphicsView\0\0items\0"
    "loadImage(QList<QGraphicsItem*>)\0"
    "updateView()\0bSliderPressed\0"
    "setTransformationMode(bool)\0"
    "setTransformationMode()\0nItemSize\0"
    "setItemSize(int)\0onScrollValueChanged(int)\0"
    "updateItem()\0item\0updateItemImage(CustomItem*)\0"
    "strEmfFile\0addItemToScene(QString)\0"
};

const QMetaObject CustomGraphicsView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_CustomGraphicsView,
      qt_meta_data_CustomGraphicsView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomGraphicsView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomGraphicsView))
        return static_cast<void*>(const_cast< CustomGraphicsView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int CustomGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: loadImage((*reinterpret_cast< QList<QGraphicsItem*>(*)>(_a[1]))); break;
        case 1: updateView(); break;
        case 2: setTransformationMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: setTransformationMode(); break;
        case 4: setItemSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: onScrollValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: updateItem(); break;
        case 7: updateItemImage((*reinterpret_cast< CustomItem*(*)>(_a[1]))); break;
        case 8: addItemToScene((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CustomGraphicsView::loadImage(QList<QGraphicsItem*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
