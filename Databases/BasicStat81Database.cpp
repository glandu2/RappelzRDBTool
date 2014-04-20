#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT16, "id"},
	 {1, TYPE_INT16, "str"},
	 {1, TYPE_INT16, "vit"},
	 {1, TYPE_INT16, "dex"},
	 {1, TYPE_INT16, "agi"},
	 {1, TYPE_INT16, "int"},
	 {1, TYPE_INT16, "men"},
	 {1, TYPE_INT16, "luk"}};

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

