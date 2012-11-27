#include "SyncManager.h"

#include <QCryptographicHash>
#include <QByteArray>
#include <QTime>
#include <QHttpRequestHeader>
#include <QHttp>
#include <QDebug>
#include <QDomElement>
#include <QDomDocument>

SyncManager::SyncManager(QObject *parent) :
	QObject(parent)
{

	connect( &_http, SIGNAL( requestFinished( int, bool ) ), this, SLOT( httpRequestFinished( int, bool ) ) );
	connect( &_http, SIGNAL( responseHeaderReceived ( const QHttpResponseHeader & ) ), this, SLOT( httpHeaderReceived( const QHttpResponseHeader & ) ) );
	connect( &_http, SIGNAL( dataReadProgress ( int, int ) ), this, SLOT( httpDataReadProgress ( int, int ) ) );
	connect( &_http, SIGNAL( dataSendProgress ( int, int ) ), this, SLOT( httpDataSendProgress ( int, int ) ) );


}


const SyncData& SyncManager::dataReceived() const
{
	return _dataReceived;
}

Sync::Result SyncManager::syncResult() const
{
	return _result;
}


void SyncManager::setSyncUrl( const QUrl& url )
{
	_syncUrl = url;
}

QUrl SyncManager::syncUrl() const
{
	return _syncUrl;
}

QString SyncManager::result() const
{
	return _httpResult;
}

/*!
	Creates an SyncData object with the userId and password. It also sets the device,
	using the ID from the current machine.
*/
SyncData SyncManager::initSyncData( const QString &userId, const QString &password )
{
	SyncData sd;

	sd.setDeviceIdFromMachine();
	sd.setUserInfo( "id", userId );
	sd.setUserInfo( "password", password );

    return sd;
}

/*!
	Starts to sync data with the server.

	If the data to sent is not valid it doesn't send
*/
bool SyncManager::sync()
{
	// test if the data to send is valid
	if( !_dataToSend.isValid() )
	{
		return false;
	}

	// Check if the url is set
	if( _syncUrl.isEmpty() )
	{
		return false;
	}

	QString boundary = 	QCryptographicHash::hash( QByteArray( QTime::currentTime().toString().toAscii() ), QCryptographicHash::Md5 ).toBase64();
	QString separator = "--" + boundary;

	//build a muiltipart from data POST

	QByteArray data;
	//email from
	data += separator + Sync::endline;
	data += "Content-Disposition: form-data; name=\"xml\"" + Sync::endline;
//	data += "Content-Type: text/plain;" + Sync::endline;
//	data += "Content-Transfer-Encoding: base64" + Sync::endline;
    data += Sync::endline;  // Don't comment this line
	data += _dataToSend.xmlData() + Sync::endline;
//	data += _dataToSend.xmlData().toUtf8().toBase64() + Sync::endline;
	data += separator + "--";

	//build http header
	QHttpRequestHeader header( "POST",       _syncUrl.path() );
	header.setValue( "Host",                 _syncUrl.host() );
	header.setValue( "Connection",           "Keep-Alive" );
	header.setValue( "Keep-Alive",           "300" );
	header.setValue( "Content-Type",         "multipart/form-data; boundary=" + boundary  );
	header.setValue( "Content-Length",       QString::number( data.length() ) );
	header.setValue( "Accept",               "*/*" );

	//set host
	_http.setHost( _syncUrl.host(), _syncUrl.port(80) );


	//post data
	_syncRequestId = _http.request( header, data );
	qDebug() << "[SyncManager::sync] Request sent id:" << _syncRequestId;

	return true;
}

void SyncManager::stopSync()
{

}

void SyncManager::setDataToSend( const SyncData& data )
{
	_dataToSend = data;
}

void SyncManager::clearData()
{
	_dataReceived.clear();
	_dataToSend.clear();
}


void SyncManager::httpDataReadProgress( int done, int total )
{

}

void SyncManager::httpDataSendProgress( int done, int total )
{

}

void SyncManager::httpHeaderReceived( const QHttpResponseHeader & )
{

}

void SyncManager::httpRequestFinished( int id, bool error )
{
	if( id == _syncRequestId )
	{
		qDebug() << "[SyncManager::httpRequestFinished] Sync request:" << _syncRequestId;
		// Test if there was an error on sync;
		if( error )
		{
			qWarning() << "[SyncManager::httpRequestFinished] Error:" << _http.errorString();
			_result = Sync::UNSUCCESSFULLY;
			emit syncFinished( _result );
			return;
		}

		QByteArray result = _http.readAll();
//        qDebug() << "[SyncManager::httpRequestFinished] Sync result:" << result;

		_httpResult = QString( result );
		_dataReceived.loadData( result );

		_result = Sync::SUCCESSFULLY;
		emit syncFinished( Sync::SUCCESSFULLY );
	}
}
