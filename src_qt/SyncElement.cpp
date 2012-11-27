#include "SyncElement.h"

#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QObject>
#include <QFile>
#include <QFileInfo>

SyncElement::SyncElement()
{
}


SyncElement::SyncElement(const QString& key, const QDateTime& time, const QByteArray& data ):
	_key( key ),
	_dateTime( time ),
	_data( data )
{
}

void SyncElement::setData( const QByteArray& data )
{
	_data = data;
}

QByteArray SyncElement::data() const
{
	return _data;
}


void SyncElement::setKey( const QString& key )
{
	_key = key;
}

QString SyncElement::key() const
{
	return _key;
}


void SyncElement::setTime( const QDateTime& dateTime )
{
	_dateTime = dateTime;
}

QDateTime SyncElement::time() const
{
	return _dateTime;
}

/*!
	Build the Xml tag to be used in a Xml file
*/
QString SyncElement::xmlTag() const
{
	QString tag;

	tag = "<data key=\"" + _key + "\" time=\"" + QString::number( _dateTime.toTime_t() ) + "\">";
	tag += _data;
	tag += "</data>";

	return tag;
}


/*!
	Creates a SyncElement from a file.

	Optinally the prefix key can be set
*/
SyncElement* SyncElement::fromFile( const QString& filename, const QString &prefixKey)
{
	QFile f( filename );

	if( !f.exists() ) // file doesn't not exist
	{
		return NULL;
	}

	if( !f.open( QIODevice::ReadOnly ) ) // Can't open file
	{
		return NULL;
	}

	QByteArray data = f.readAll(); // Read file contents
	f.close();

	QFileInfo fi(f); // FileInfo to get the last modified date


	return new SyncElement( prefixKey + f.fileName(), fi.lastModified(), data );
}
