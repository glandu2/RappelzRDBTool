#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "sub_id"},
	 {1, TYPE_INT32, "group_id"},
	 {1, TYPE_INT32, "condition_type"},
	 {1, TYPE_INT32, "condition_opt_0"},
	 {1, TYPE_INT32, "condition_opt_1"},
	 {1, TYPE_INT8, "visible_if_0"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

