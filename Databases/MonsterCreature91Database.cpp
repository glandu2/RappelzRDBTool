#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

// RDU format in 9.1 client

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "sid"},
	{1, TYPE_INT32, "use_code"},
	{1, TYPE_INT32, "summon_code_1"},
	{1, TYPE_INT32, "summon_code_2"},
	{1, TYPE_INT32, "summon_code_3"},
	{1, TYPE_INT32, "summon_code_4"},
	{1, TYPE_INT32, "summon_code_5"},
	{1, TYPE_INT32, "summon_code_6"},
	{1, TYPE_INT32, "summon_code_7"},
	{1, TYPE_INT32, "summon_code_8"},
	{1, TYPE_INT32, "summon_code_9"},
	{1, TYPE_INT32, "summon_code_10"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_monster_creature";
}

#ifdef __cplusplus
}
#endif

