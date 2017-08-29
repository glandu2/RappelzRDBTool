#include "ILog.h"
#include "Log.h"

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:createDatabase=_createDatabase@4")
#endif
ILog * DLLCALLCONV getLogger() {
	static Log log;
	return &log;
}
