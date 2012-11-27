#ifndef SYNC_H
#define SYNC_H

#include <QString>

namespace Sync
{
	enum Result
	{
		SUCCESSFULLY = 0,
		UNSUCCESSFULLY = 1,
		INVALIDD_USER = 2,
		ABORTED = -1,
		UNDEFINED = 1024
	};

	const QString endline = "\r\n";

}
#endif // SYNC_H
