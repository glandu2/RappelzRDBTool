#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "id"},
                               {MAKEINDEXEDVAR(0, 1), TYPE_VARCHAR_SIZE | TYPE_SQLIGNORE | TYPE_CSVIGNORE, ""},
                               {MAKEINDEXEDVAR(0, 3999), TYPE_NVARCHAR_STR, "value"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif
