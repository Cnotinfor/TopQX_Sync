#include "XmlSync.h"

XmlSync::XmlSync()
{
}

QDomDocument XmlSync::received()
{
	return _xml;
}

void XmlSync::setXmlToSend(const QDomDocument xml)
{
	_xml = xml;
}

/**
 * @brief XmlSync::dataToString
 *
 * Retunrs the xml in a string format
 *
 * @return QString
 */

QString XmlSync::dataToString()
{
	QString xmlStr;



//	QDomNodeList

//	QDomElement root = _xml.documentElement();

//	xmlStr.append("<");
//	xmlStr.append(root.tagName());


//	if(root.tagName() != _mainElement)
//	{
//		qWarning() << "[SyncData::loadData] Tag "+ _mainElement +" not found:";
//		return false;
//	}

//	// get the status from the
//	QDomNodeList statusNodes = root.elementsByTagName("status");
//	if(statusNodes.size() == 0)
//	{
//		qWarning() << "[SyncData::loadData] Tag status not found.";
//		return false;
//	}

//	QDomElement status = statusNodes.at(0).toElement();
//	_error = (Sync::Result) status.attribute( "id", QString::number( Sync::UNSUCCESSFULLY ) ).toInt();

//	_errorMsg = status.nodeValue();

//	if( _error != Sync::SUCCESSFULLY )
//	{
//		qWarning() << "[SynData::loadData] Sync status:" << _error;
//		return true;
//	}

//	QDomNodeList nodeList = root.elementsByTagName("data");

//	for(int i = 0; i < nodeList.size(); i++ )
//	{
//		QDomNode node = nodeList.at( i );

//		if( node.attributes().contains( "key" ) )
//		{
//			QString key = node.attributes().namedItem("key").nodeValue();
//			QString value = node.toElement().text();
//			addElement( key, QDateTime::currentDateTime(), value.toLatin1() );
//		}
//		else
//		{
//			qWarning() << "[SyncData::loadData] Invalid data, no key";
//		}

//	}

	return xmlStr;
}
