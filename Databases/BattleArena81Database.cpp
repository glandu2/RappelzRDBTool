#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "type"},
	 {1, TYPE_INT8, "team_count"},
	 {1, TYPE_INT8, "min_member"},
	 {1, TYPE_INT8, "max_member"},
	 {1, TYPE_INT32, "objective_point"},
	 {1, TYPE_INT32, "min_playtime"},
	 {1, TYPE_INT32, "region_left"},
	 {1, TYPE_INT32, "region_top"},
	 {1, TYPE_INT32, "region_right"},
	 {1, TYPE_INT32, "region_bottom"},
	 {1, TYPE_INT32, "count_down"},
	 {1, TYPE_INT32, "battle_duration"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

