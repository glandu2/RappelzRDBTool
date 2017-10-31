#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "icon_id"}, {256, TYPE_CHAR, "icon_file"}};

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
	return "db_defaulttextureiconresource";
}

#ifdef __cplusplus
}
#endif
