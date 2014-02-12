#include "../Base/IRowManipulator.h"
#include "../Base/DataType.h"
#include "../Base/ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "count"},
	 {1, TYPE_INT32, "skill_1"},
	 {1, TYPE_INT32, "skill_2"},
	 {1, TYPE_INT32, "skill_3"},
	 {1, TYPE_INT32, "skill_4"},
	 {1, TYPE_INT32, "skill_5"},
	 {1, TYPE_INT32, "skill_6"},
	 {1, TYPE_INT32, "skill_7"},
	 {1, TYPE_INT32, "skill_8"},
	 {1, TYPE_INT32, "skill_9"},
	 {1, TYPE_INT32, "skill_10"},
	 {1, TYPE_INT32, "skill_11"},
	 {1, TYPE_INT32, "skill_12"},
	 {1, TYPE_INT32, "skill_13"},
	 {1, TYPE_INT32, "skill_14"},
	 {1, TYPE_INT32, "skill_15"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_summonrandomskill";
}

#ifdef __cplusplus
}
#endif
