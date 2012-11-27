#ifndef SYNCELEMENT_H
#define SYNCELEMENT_H

#include <QString>
#include <QByteArray>
#include <QDateTime>

#include "SyncManagerGlobal.h"

class SYNC_MANAGER_EXPORT SyncElement
{

public:
	SyncElement();
	SyncElement(const QString& key, const QDateTime& time, const QByteArray& data );

	void setData( const QByteArray& data );
	QByteArray data() const;

	void setKey( const QString& key );
	QString key() const;

	void setTime( const QDateTime& dateTime );
	QDateTime time() const;

	QString xmlTag() const;

	static SyncElement* fromFile( const QString& filename, const QString &prefixKey = "");

private:
	QByteArray     _data;
	QString        _key;
	QDateTime      _dateTime;
};

#endif // SYNCELEMENT_H
