#include "ILog.h"
#include "Log.h"

#pragma comment(linker, "/EXPORT:createDatabase=_createDatabase@4")
ILog * DLLCALLCONV getLogger() {
	static Log log;
	return &log;
}
