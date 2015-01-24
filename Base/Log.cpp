#include "Log.h"
#include <stdarg.h>
#include <string>
#include <string.h>
#include <stdio.h>

#if defined(_MSC_VER)
	#define snprintf(buffer, size, ...) _snprintf_s(buffer, size, _TRUNCATE, __VA_ARGS__)
	#if _MSC_VER < 1800
		#define va_copy(d,s) ((d) = (s))
	#endif
#endif

Log::Log()
{
	maxLevel = LL_Info;
	onMessage = &defaultCallback;
}

static int c99vsnprintf(char* dest, int size, const char* format, va_list args) {
	va_list argsForCount;
	va_copy(argsForCount, args);

	int result = vsnprintf(dest, size, format, args);

#ifdef _WIN32
	if(result == -1)
		result = _vscprintf(format, argsForCount);
#endif

	return result;
}

static void stringformat(std::string& dest, const char* message, va_list args) {
	va_list argsFor2ndPass;
	va_copy(argsFor2ndPass, args);

	dest.resize(128);
	int result = c99vsnprintf(&dest[0], dest.size(), message, args);

	if(result < 0) {
		dest = message;
		return;
	}

	if(result < (int)dest.size()) {
		dest.resize(result);
	} else if(result >= (int)dest.size()) {
		dest.resize(result+1);

		vsnprintf(&dest[0], dest.size(), message, argsFor2ndPass);
		dest.resize(result);
	}
}

void Log::log(Level level, const char* message, ...) {
	if(!wouldLog(level))
		return;
	static const char* LEVEL_NAME[] = { "Fatal: ", "Error: ", "Warning: ", "Info: ", "Debug: ", "Trace: " };

	va_list args;
	std::string messageStr;

	va_start(args, message);
	stringformat(messageStr, message, args);
	va_end(args);

	onMessage(this, (std::string(LEVEL_NAME[level]) + messageStr).c_str());
}

ILog::Level Log::getMaxLevel() {
	return maxLevel;
}

void Log::setMaxLevel(Level level) {
	maxLevel = level;
}

bool Log::wouldLog(Level level) {
	return level <= maxLevel;
}


void Log::setCallback(CallbackOnLogMessage callback) {
	if(callback)
		onMessage = callback;
	else
		onMessage = &defaultCallback;
}

void Log::defaultCallback(ILog* logger, const char* message) {
	fwrite(message, strlen(message), 1, stderr);
}
