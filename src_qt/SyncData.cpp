#include "SyncData.h"

#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QFileInfo>
#include <QDomDocument>
#include <QBuffer>
#include <QDebug>
#include <QNetworkInterface>

/*!

*/
SyncData::SyncData()
{
}

/*!
	Copy constructor
*/
SyncData::SyncData(const SyncData& data):
	_elements( data._elements ),
	_error( data._error ),
	_errorMsg( data._errorMsg ),
	_userInfo( data._userInfo ),
	_deviceId( data._deviceId)
{

}


/*!

*/
QString SyncData::userInfo( const QString& key ) const
{
	return _userInfo.value( key );
}

/*!

*/
void SyncData::setUserInfo( const QString& key, const QString &value )
{
	_userInfo.insert( key, value );
}

/*!

*/
void SyncData::removeUserInfo( const QString& key )
{
    _userInfo.remove(key);
}

QStringList SyncData::appInfo() const
{
    QStringList appData;
    appData << _appSystem << _appVersion << _appVersionNumber;
    return appData;
}

void SyncData::setAppInfo(const QString &system, const QString &version, const QString &versionNumber)
{
    _appSystem = system;
    _appVersion = version;
    _appVersionNumber = versionNumber;
}

void SyncData::clearAppInfo()
{
    _appSystem.clear();
    _appVersion.clear();
    _appVersionNumber.clear();
}


/*!

*/
QString SyncData::deviceId() const
{
	return _deviceId;
}

/*!

*/
void SyncData::setDeviceId( const QString& deviceId )
{
	_deviceId = deviceId;
}

/*!

*/
void SyncData::setDeviceIdFromMachine()
{
	QString address;
#ifndef _DEBUG

	// Gets the list of all the interfaces present in the system
	QList<QNetworkInterface> networkInterfaces = QNetworkInterface::allInterfaces();
	//
	// Gets first that is not the loopback
	//
	for( int i=0; i < networkInterfaces.size(); i++ )
	{
		//
		// Check if interface is valid and is not loopback and MAC Address is not empty
		//
		if( networkInterfaces[i].isValid() &&
			!networkInterfaces[i].flags().testFlag( QNetworkInterface::IsLoopBack ) && // it is not a local loopback
			!networkInterfaces[i].hardwareAddress().isEmpty() )
		{
#ifdef Q_WS_MAC
			// Check if is Ethernet interface
			if( networkInterfaces[i].name().startsWith( "en" )  )
			{
#endif
				address = networkInterfaces[i].hardwareAddress();
				break;
#ifdef Q_WS_MAC
			}
#endif
		}
	}
#endif
	//
	//  set address to "NIC.NOT.FOUND" if no interface is valid
	//
	if( address == "" )
	{
		address = "NIC.NOT.FOUND";
	}

	_deviceId = address;
}

/*!

*/
const QList<SyncElement>& SyncData::elements() const
{
	return _elements;
}

/*!

*/
bool SyncData::addElement( const QString& key, const QDateTime& time, const QByteArray& data )
{
	SyncElement element( key, time, data );

	_elements.append( element );

	return true;
}

/*!

*/
bool SyncData::addElement( const QString& key, const QString& filepath )
{
	QFileInfo fi( filepath );

	if( !fi.exists() )
	{
		return false;
	}

	QDateTime time = fi.lastModified();

	return addElement( key, filepath, time );
}

/*!
	Opens the file name and put its content in a SyncElement

	The key for the element is the combination of key + filename
*/
bool SyncData::addElement( const QString& key, const QString& filepath, const QDateTime& time )
{
	QFile f( filepath );
	QString filename = filepath.mid( filepath.lastIndexOf("/") + 1 );

	// If there was an error opening the file
	if( !f.open( QIODevice::ReadOnly ) )
	{
		return false;
	}

	QByteArray data = f.readAll().toBase64();
	QString newKey = key + filename;

	SyncElement element( newKey, time, data );

	_elements.append( element );

	return true;
}



/*!
	Returns the result code from the sync
*/
Sync::Result SyncData::error() const
{
	return _error;
}

/*!

*/
QString SyncData::errorMsg() const
{
	return _errorMsg;
}

/*!
	Load the xml data from HTTP response from the server

	XML file must have a status tag
*/
bool SyncData::loadData( const QByteArray &xmlData )
{
	QDomDocument doc;

	if( !doc.setContent( xmlData ) )
	{
		qWarning()<< "[SyncData::loadData] Invalid xml Data";
		return false;
	}

	QDomElement root = doc.documentElement();
	if(root.tagName() != "sync")
	{
		qWarning() << "[SyncData::loadData] Tag sync not found:";
		return false;
	}

	// get the status from the
	QDomNodeList statusNodes = root.elementsByTagName("status");
	if(statusNodes.size() == 0)
	{
		qWarning() << "[SyncData::loadData] Tag status not found.";
		return false;
	}

	QDomElement status = statusNodes.at(0).toElement();
	_error = (Sync::Result) status.attribute( "id", QString::number( Sync::UNSUCCESSFULLY ) ).toInt();

	_errorMsg = status.nodeValue();

	if( _error != Sync::SUCCESSFULLY )
	{
		qWarning() << "[SynData::loadData] Sync status:" << _error;
		return true;
	}

	QDomNodeList nodeList = root.elementsByTagName("data");

	for(int i = 0; i < nodeList.size(); i++ )
	{
		QDomNode node = nodeList.at( i );

		if( node.attributes().contains( "key" ) )
		{
			QString key = node.attributes().namedItem("key").nodeValue();
			QString value = node.toElement().text();
			addElement( key, QDateTime::currentDateTime(), value.toLatin1() );
		}
		else
		{
			qWarning() << "[SyncData::loadData] Invalid data, no key";
		}

	}


	return true;
}


/*!

*/
QString SyncData::xmlData() const
{
	QString xmlData;

	xmlData = "<sync>" + Sync::endline;

	if( !_userInfo.empty() )
	{
		xmlData += "<user";
		// small hack to include password
		//xmlData += " password=\"mozart\"";
		QHashIterator<QString, QString> it( _userInfo );
		while( it.hasNext() )
		{
			it.next();
			xmlData += " " + it.key() + "=\"" + it.value() + "\"";
		}
		xmlData += " />" + Sync::endline;
	}

	if( _deviceId.size() != 0 )
	{
		xmlData += "<device id=\"" + _deviceId + "\" />" + Sync::endline;
	}

	QListIterator<SyncElement> it( _elements );
	while( it.hasNext() )
	{
		SyncElement value = it.next();
		xmlData += value.xmlTag() + Sync::endline;
	}

    xmlData += "<app system=\"" + _appSystem + "\" version=\"" + _appVersion + "\" version_number=\"" + _appVersionNumber + "\" />" + Sync::endline;

	xmlData += "</sync>" + Sync::endline;

	return xmlData;
}


/*!

*/
void SyncData::clear()
{
	_elements.clear();
	_errorMsg = "";
	_error = Sync::UNDEFINED;
	_userInfo.clear();
	_deviceId = "";
}

/*!
	Test if the sync Data has user Info valid and an deviceID
*/
bool SyncData::isValid()
{
	// Test the user info
	if(!_userInfo.empty() && !_userInfo.contains("id") && !_userInfo.contains("password")  )
	{
		qDebug() << "[SyncData::isValid] Invalid user Info";
		return false;
	}

	// User Info and Device id
	// Test if both are empty or set (XOR operation)
	if((_deviceId.isEmpty()) ^ (_userInfo.empty()) )
	{
		qDebug() << "[SyncData::isValid] Device or userInfo are fill in, but not both";
		return false;
	}

	return true;
}
