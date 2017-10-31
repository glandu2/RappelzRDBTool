#include "DataType.h"
#include "ExportDLL.h"
#include "IRowManipulator.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "state_id"},
                               {1, TYPE_INT32, "text_id"},
                               {1, TYPE_INT32, "tooltip_id"},
                               {1, TYPE_INT8, "is_harmful"},
                               {1, TYPE_INT8 | TYPE_SQLIGNORE, "padding0"},
                               {1, TYPE_INT16 | TYPE_SQLIGNORE, "padding1"},
                               {1, TYPE_INT32, "state_time_type"},
                               {1, TYPE_INT32, "state_group"},
                               {1, TYPE_INT8, "uf_avatar"},
                               {1, TYPE_INT8, "uf_summon"},
                               {1, TYPE_INT8, "uf_monster"},
                               {1, TYPE_INT8, "reiteration_count"},
                               {1, TYPE_INT32, "effect_type"},
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
                               {1, TYPE_INT32, "icon_id"},
                               {256, TYPE_CHAR, "icon_file_name"},
                               {1, TYPE_INT32, "fx_id"},
                               {1, TYPE_INT32, "pos_id"},
                               {1, TYPE_INT32, "cast_fx_id"},
                               {1, TYPE_INT32, "cast_fx_pos_id"},
                               {1, TYPE_INT32, "cast_skill_id"},
                               {1, TYPE_INT32, "hit_fx_id"},
                               {1, TYPE_INT32, "hit_fx_pos_id"},
                               {1, TYPE_INT32, "special_output_timing_id"},
                               {1, TYPE_INT32, "special_output_fx_id"},
                               {1, TYPE_INT32, "special_output_fx_pos_id"},
                               {1, TYPE_INT32, "special_output_fx_delay"}};

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
	return "db_tenacity";
}

#ifdef __cplusplus
}
#endif
