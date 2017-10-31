#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {
    {128, TYPE_CHAR, "ani_name"},       {1, TYPE_INT32, "play_speed"},      {1, TYPE_INT32, "frame_Length"},
    {1, TYPE_INT32, "event_time01"},    {1, TYPE_INT32, "event_time02"},    {1, TYPE_INT32, "event_time03"},
    {1, TYPE_INT32, "event_time04"},    {1, TYPE_INT32, "event_time05"},    {1, TYPE_INT32, "event_time06"},
    {1, TYPE_INT32, "event_time07"},    {256, TYPE_CHAR, "event_script01"}, {256, TYPE_CHAR, "event_script02"},
    {256, TYPE_CHAR, "event_script03"}, {256, TYPE_CHAR, "event_script04"}, {256, TYPE_CHAR, "event_script05"},
    {256, TYPE_CHAR, "event_script06"}, {256, TYPE_CHAR, "event_script07"}, {1, TYPE_INT32, "sound_time01"},
    {1, TYPE_INT32, "sound_time02"},    {1, TYPE_INT32, "sound_time03"},    {1, TYPE_INT32, "sound_time04"},
    {1, TYPE_INT32, "sound_time05"},    {1, TYPE_INT32, "sound_time06"},    {1, TYPE_INT32, "sound_time07"},
    {256, TYPE_CHAR, "sound_script01"}, {256, TYPE_CHAR, "sound_script02"}, {256, TYPE_CHAR, "sound_script03"},
    {256, TYPE_CHAR, "sound_script04"}, {256, TYPE_CHAR, "sound_script05"}, {256, TYPE_CHAR, "sound_script06"},
    {256, TYPE_CHAR, "sound_script07"}, {1, TYPE_INT32, "race_id"},         {1, TYPE_INT32, "sex_id"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return "race_id\0"
	       "sex_id\0"
	       "ani_name\0"
	       "frame_Length\0"
	       "play_speed\0"
	       "event_time01\0"
	       "event_script01\0"
	       "sound_time01\0"
	       "sound_script01\0"
	       "event_time02\0"
	       "event_script02\0"
	       "sound_time02\0"
	       "sound_script02\0"
	       "event_time03\0"
	       "event_script03\0"
	       "sound_time03\0"
	       "sound_script03\0"
	       "event_time04\0"
	       "event_script04\0"
	       "sound_time04\0"
	       "sound_script04\0"
	       "event_time05\0"
	       "event_script05\0"
	       "sound_time05\0"
	       "sound_script05\0"
	       "event_time06\0"
	       "event_script06\0"
	       "sound_time06\0"
	       "sound_script06\0"
	       "event_time07\0"
	       "event_script07\0"
	       "sound_time07\0"
	       "sound_script07\0";
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getDefaultFileName() {
	return "db_aniinfo";
}

#ifdef __cplusplus
}
#endif
