#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT16, "id"},
	 {1, TYPE_INT16, "strength"},
	 {1, TYPE_INT16, "vitality"},
	 {1, TYPE_INT16, "dexterity"},
	 {1, TYPE_INT16, "agility"},
	 {1, TYPE_INT16, "intel"},
	 {1, TYPE_INT16, "wisdom"},
	 {1, TYPE_INT16, "luck"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getDefaultTableName=_getDefaultTableName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultTableName() {
	return "StatResource";
}

#ifdef __cplusplus
}
#endif

