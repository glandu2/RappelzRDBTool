#ifndef DEBUGOUTPUT_H
#define DEBUGOUTPUT_H


#ifdef _WIN32
#include <windows.h>

#define debugoutput(format, ...) \
{ \
	char buffer[512]; \
	sprintf(buffer, format "\n", #__VA_ARGS__); \
	OutputDebugString(buffer); \
}
#else
#include <stdio.h>
#define debugoutput(format, ...) fprintf(stderr, format, #__VA_ARGS__)
#endif

#endif // DEBUGOUTPUT_H
