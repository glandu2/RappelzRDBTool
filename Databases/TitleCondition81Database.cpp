#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "title_id"},
                               {1, TYPE_INT32, "sub_id"},
                               {1, TYPE_INT32, "group_id"},
                               {1, TYPE_INT32, "condition_type_id"},
                               {1, TYPE_INT64, "count"},
                               {1, TYPE_BIT, "is_archieve_condition"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getDefaultFileName() {
	return "db_titleconditionresource";
}

#ifdef __cplusplus
}
#endif
