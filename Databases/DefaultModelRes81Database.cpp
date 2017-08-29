#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "model_id"},
	 {1, TYPE_INT32, "part_id"},
	 {1, TYPE_INT32, "lobby_resource_id"},
	 {256, TYPE_CHAR, "deva_male"},
	 {256, TYPE_CHAR, "deva_female"},
	 {256, TYPE_CHAR, "asura_male"},
	 {256, TYPE_CHAR, "asura_female"},
	 {256, TYPE_CHAR, "gaia_male"},
	 {256, TYPE_CHAR, "gaia_female"},
	 {1, TYPE_INT32, "texture_group_id"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultTableName=_getDefaultTableName@0")
#endif
EDATABASEDLL const char*  DLLCALLCONV getDefaultTableName() {
	return "DefaultModelResource";
}

#ifdef __cplusplus
}
#endif

