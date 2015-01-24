#ifndef ILOG_H
#define ILOG_H

#include "IObject.h"
#include "DataType.h"
#include "ExportDLL.h"

class ILog : public IObject
{
public:
	enum Level {
		LL_Fatal,
		LL_Error,
		LL_Warning,
		LL_Info,
		LL_Debug,
		LL_Trace
	};
	typedef void (*CallbackOnLogMessage)(ILog* logger, const char* message);

public:
	virtual void log(Level level, const char* message, ...) PRINTFCHECK(3, 4) = 0;
	virtual Level getMaxLevel() = 0;
	virtual void setMaxLevel(Level level) = 0;
	virtual bool wouldLog(Level level) = 0;

	virtual void setCallback(CallbackOnLogMessage callback) = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

EBASEDLL ILog * DLLCALLCONV getLogger();

#ifdef __cplusplus
}
#endif

#endif // ILOG_H
