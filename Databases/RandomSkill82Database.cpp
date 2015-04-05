#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "sid"},
	 {1, TYPE_INT32, "use_code"},
	 {1, TYPE_INT32, "skill_id_1"},
	 {1, TYPE_INT32, "skill_id_2"},
	 {1, TYPE_INT32, "skill_id_3"},
	 {1, TYPE_INT32, "skill_id_4"},
	 {1, TYPE_INT32, "skill_id_5"},
	 {1, TYPE_INT32, "skill_id_6"},
	 {1, TYPE_INT32, "skill_id_7"},
	 {1, TYPE_INT32, "skill_id_8"},
	 {1, TYPE_INT32, "skill_id_9"},
	 {1, TYPE_INT32, "skill_id_10"},
	 {1, TYPE_INT32, "skill_id_11"},
	 {1, TYPE_INT32, "skill_id_12"},
	 {1, TYPE_INT32, "skill_id_13"},
	 {1, TYPE_INT32, "skill_id_14"},
	 {1, TYPE_INT32, "skill_id_15"}};

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
