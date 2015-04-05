#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "sid"},
	 {1, TYPE_INT32, "affiliation_id"},
	 {1, TYPE_INT32, "string_id"},
	 {256, TYPE_CHAR, "possible_icon"},
	 {256, TYPE_CHAR, "impossible_icon"},
	 {256, TYPE_CHAR, "exist_icon"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_monsteraffiliationdetailresource";
}

#ifdef __cplusplus
}
#endif

