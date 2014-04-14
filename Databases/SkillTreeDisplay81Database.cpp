#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "job_id"},
	 {1, TYPE_INT32, "slot_v"},
	 {1, TYPE_INT32, "skill1_id"},
	 {1, TYPE_INT32, "skill2_id"},
	 {1, TYPE_INT32, "skill3_id"},
	 {1, TYPE_INT32, "skill4_id"},
	 {1, TYPE_INT32, "skill5_id"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getDefaultTableName=_getDefaultTableName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultTableName() {
	return "SkillTreeDisplay";
}

#ifdef __cplusplus
}
#endif

