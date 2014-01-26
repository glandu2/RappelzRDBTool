#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "category_id"},
	 {1, TYPE_INT32, "sub_category_id"},
	 {1, TYPE_INT32, "name_id"},
	 {1, TYPE_INT32, "local_flag"},
	 {1, TYPE_INT32, "item_group"},
	 {1, TYPE_INT32, "item_class"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

