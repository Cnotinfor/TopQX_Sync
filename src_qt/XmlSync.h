#ifndef XMLSYNC_H
#define XMLSYNC_H

#include "SyncManager.h"
// Qt
#include <QDomDocument>

class SYNC_MANAGER_EXPORT XmlSync : public SyncManager
{
public:
	XmlSync();

	QDomDocument received();
	void setXmlToSend(const QDomDocument xml);
protected:
	QString dataToString();
private:
	QDomDocument _xml;
};

#endif // XMLSYNC_H
