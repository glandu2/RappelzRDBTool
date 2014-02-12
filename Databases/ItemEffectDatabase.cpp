#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "ordinal_id"},
	 {1, TYPE_INT32, "tooltip_id"},
	 {1, TYPE_INT8, "effect_type"},
	 {1, TYPE_INT16, "effect_id"},
	 {1, TYPE_INT16, "effect_level"},
	 {1, TYPE_FLOAT64, "value_0"},
	 {1, TYPE_FLOAT64, "value_1"},
	 {1, TYPE_FLOAT64, "value_2"},
	 {1, TYPE_FLOAT64, "value_3"},
	 {1, TYPE_FLOAT64, "value_4"},
	 {1, TYPE_FLOAT64, "value_5"},
	 {1, TYPE_FLOAT64, "value_6"},
	 {1, TYPE_FLOAT64, "value_7"},
	 {1, TYPE_FLOAT64, "value_8"},
	 {1, TYPE_FLOAT64, "value_9"},
	 {1, TYPE_FLOAT64, "value_10"},
	 {1, TYPE_FLOAT64, "value_11"},
	 {1, TYPE_FLOAT64, "value_12"},
	 {1, TYPE_FLOAT64, "value_13"},
	 {1, TYPE_FLOAT64, "value_14"},
	 {1, TYPE_FLOAT64, "value_15"},
	 {1, TYPE_FLOAT64, "value_16"},
	 {1, TYPE_FLOAT64, "value_17"},
	 {1, TYPE_FLOAT64, "value_18"},
	 {1, TYPE_FLOAT64, "value_19"},
	 {1, TYPE_INT32, "effect_id2"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_itemeffectresource";
}

#ifdef __cplusplus
}
#endif

