#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "text_id"},
	 {1, TYPE_INT32, "tooltip_id"},
	 {1, TYPE_INT32, "type"},
	 {1, TYPE_INT32, "local_flag"},
	 {1, TYPE_INT32, "casting_time"},
	 {1, TYPE_INT32, "use_count"},
	 {1, TYPE_INT32, "regen_time"},
	 {1, TYPE_INT32, "life_time"},
	 {1, TYPE_FLOAT32, "casting_range"},
	 {1, TYPE_FLOAT32, "target_fx_size"},
	 {1, TYPE_INT32, "limit_min_level"},
	 {1, TYPE_INT32, "limit_max_level"},
	 {1, TYPE_INT8, "limit_deva"},
	 {1, TYPE_INT8, "limit_asura"},
	 {1, TYPE_INT8, "limit_gaia"},
	 {1, TYPE_INT8, "limit_fighter"},
	 {1, TYPE_INT8, "limit_hunter"},
	 {1, TYPE_INT8, "limit_magician"},
	 {1, TYPE_INT8, "limit_summoner"},
	 {1, TYPE_INT32, "limit_job"},
	 {1, TYPE_INT32, "activation_condition"},
	 {1, TYPE_INT32, "activation_value1"},
	 {1, TYPE_INT32, "activation_value2"},
	 {1, TYPE_INT32, "activation2_condition"},
	 {1, TYPE_INT32, "activation2_value1"},
	 {1, TYPE_INT32, "activation2_value2"},
	 {1, TYPE_INT32, "activate_id"},
	 {1, TYPE_INT32, "drop_item_id_00"},
	 {1, TYPE_DECIMAL, "drop_item_ratio_00"},
	 {1, TYPE_INT32, "drop_item_count_min_00"},
	 {1, TYPE_INT32, "drop_item_count_max_00"},
	 {1, TYPE_INT32, "drop_item_level_min_00"},
	 {1, TYPE_INT32, "drop_item_level_max_00"},
	 {1, TYPE_INT32, "drop_item_id_01"},
	 {1, TYPE_DECIMAL, "drop_item_ratio_01"},
	 {1, TYPE_INT32, "drop_item_count_min_01"},
	 {1, TYPE_INT32, "drop_item_count_max_01"},
	 {1, TYPE_INT32, "drop_item_level_min_01"},
	 {1, TYPE_INT32, "drop_item_level_max_01"},
	 {256, TYPE_CHAR, "script_text"},
	 {256, TYPE_CHAR, "file_name"},
	 {1, TYPE_INT32, "terrain_check"},
	 {1, TYPE_INT16, "unknown"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_fieldpropresource";
}

#ifdef __cplusplus
}
#endif

