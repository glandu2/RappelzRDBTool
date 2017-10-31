#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "id"},
                               {1, TYPE_INT32, "decompose_type"},
                               {1, TYPE_INT32, "needed_price"},
                               {1, TYPE_INT32, "type_value_01"},
                               {1, TYPE_INT32, "type_value_02"},
                               {1, TYPE_INT32, "type_value_03"},
                               {1, TYPE_INT32, "type_value_04"},
                               {1, TYPE_INT32, "type_value_05"},
                               {1, TYPE_INT32, "type_value_06"},
                               {1, TYPE_INT32, "material_type_01"},
                               {1, TYPE_INT32, "material_value_01"},
                               {1, TYPE_INT32, "material_type_02"},
                               {1, TYPE_INT32, "material_value_02"},
                               {1, TYPE_INT32, "material_type_03"},
                               {1, TYPE_INT32, "material_value_03"},
                               {1, TYPE_INT32, "material_type_04"},
                               {1, TYPE_INT32, "material_value_04"},
                               {1, TYPE_INT32, "material_type_05"},
                               {1, TYPE_INT32, "material_value_05"}};

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
