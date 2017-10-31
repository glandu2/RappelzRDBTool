#ifndef LOG_H
#define LOG_H

#include "DataType.h"
#include "ILog.h"

class Log : public CImplement<ILog> {
public:
	Log();

	void log(Level level, const char* message, ...) PRINTFCHECK(3, 4);
	Level getMaxLevel();
	void setMaxLevel(Level level);
	bool wouldLog(Level level);

	void setCallback(CallbackOnLogMessage callback);

private:
	static void defaultCallback(ILog* logger, const char* message);

private:
	CallbackOnLogMessage onMessage;
	Level maxLevel;
};

#endif  // LOG_H
