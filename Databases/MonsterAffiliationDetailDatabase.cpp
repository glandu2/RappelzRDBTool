#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "value1"},
	 {1, TYPE_INT32, "name_id"},
	 {256, TYPE_CHAR, "icon_tamable"},
	 {256, TYPE_CHAR, "icon_untamable"},
	 {256, TYPE_CHAR, "icon_tamed"}};

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

