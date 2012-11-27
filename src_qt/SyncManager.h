#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <QObject>
#include <QUrl>
#include <QHttp>
#include <QHttpResponseHeader>

#include "SyncManagerGlobal.h"
#include "Sync.h"
#include "SyncData.h"

/*!
	\class SyncManager

	SyncManager is responsible for sinchronizing information between the server
	and client
*/
class SYNC_MANAGER_EXPORT SyncManager : public QObject
{
	Q_OBJECT

public:
	SyncManager(QObject *parent = 0);

	const SyncData& dataReceived() const;
	Sync::Result syncResult() const;

	void setSyncUrl( const QUrl& url );
	QUrl syncUrl() const;

	QString result() const; // testing only

    static SyncData initSyncData( const QString &userId, const QString &password );

signals:
	void syncFinished( int );
	void syncStarted();

public slots:
	bool sync();
	void stopSync();
	void setDataToSend( const SyncData& data );
	void clearData();

private slots:
	void httpDataReadProgress( int done, int total );
	void httpDataSendProgress( int done, int total );
	void httpHeaderReceived( const QHttpResponseHeader & );
	void httpRequestFinished( int id, bool error );

private:
	SyncData       _dataToSend;
	SyncData       _dataReceived;
	Sync::Result   _result;
	QUrl           _syncUrl;
	QHttp          _http;
	int            _syncRequestId;

	QString        _httpResult; // testing only
};

#endif // SYNCMANAGER_H
