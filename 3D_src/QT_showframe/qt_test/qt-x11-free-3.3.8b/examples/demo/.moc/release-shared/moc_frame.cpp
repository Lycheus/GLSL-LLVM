/****************************************************************************
** Frame meta object code from reading C++ file 'frame.h'
**
** Created: Fri Mar 20 16:22:59 2015
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../frame.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Frame::className() const
{
    return "Frame";
}

QMetaObject *Frame::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Frame( "Frame", &Frame::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Frame::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Frame", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Frame::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Frame", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Frame::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setStyle", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "setStyle(const QString&)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Frame", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Frame.setMetaObject( metaObj );
    return metaObj;
}

void* Frame::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Frame" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool Frame::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setStyle((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Frame::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Frame::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool Frame::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
