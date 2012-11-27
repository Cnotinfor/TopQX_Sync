#ifndef SYNCDATA_H
#define SYNCDATA_H

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include "SyncManagerGlobal.h"
#include "SyncElement.h"
#include "Sync.h"

class SYNC_MANAGER_EXPORT SyncData
{

public:
	SyncData();
	SyncData(const SyncData& data);

	QString userInfo( const QString& key ) const;
	void setUserInfo( const QString& key, const QString &value );
	void removeUserInfo(const QString &key);

    QStringList appInfo() const;
    void setAppInfo(const QString& system, const QString &version , const QString &versionNumber);
    void clearAppInfo();

	QString deviceId() const;
	void setDeviceId( const QString& deviceId );
	void setDeviceIdFromMachine();

	const QList<SyncElement>& elements() const;

	bool addElement( const QString& key, const QDateTime& time, const QByteArray& data );
	bool addElement( const QString& key, const QString& filepath );
	bool addElement( const QString& key, const QString& filepath, const QDateTime& time );

	Sync::Result error() const;
	QString errorMsg() const;

	bool loadData( const QByteArray &xmlData );

	QString xmlData() const;

	void clear();

	bool isValid();
private:
	QList<SyncElement>          _elements;
	Sync::Result                _error;
	QString                     _errorMsg;
	QHash<QString, QString>     _userInfo;
	QString                     _deviceId;
    // App data
    QString                     _appSystem;
    QString                     _appVersion;
    QString                     _appVersionNumber;
};

#endif // SYNCDATA_H
