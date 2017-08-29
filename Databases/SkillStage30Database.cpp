#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT8, "elemental"},
	 {1, TYPE_INT8, "type"},
	 {1, TYPE_INT8, "deal_damage"},
	 {1, TYPE_INT8, "has_target"},
	 {1, TYPE_INT8, "corpse_target"},
	 {1, TYPE_INT8, "is_toggle"},
	 {1, TYPE_INT8, "is_creature_skill"},
	 {257, TYPE_CHAR, "stage_type_name"},
	 {1, TYPE_INT32, "stage_type_id"},
	 {1, TYPE_INT32, "casting_type_id"},
	 {1, TYPE_INT32, "casting_start_motion_id"},
	 {1, TYPE_INT32, "casting_middle_motion_id"},
	 {1, TYPE_INT32, "casting_fx_set_id"},
	 {1, TYPE_INT32, "fire_motion_id"},
	 {1, TYPE_INT32, "fire_fx_set_id"},
	 {1, TYPE_FLOAT32, "stage_data_01"},
	 {1, TYPE_FLOAT32, "stage_data_02"},
	 {1, TYPE_FLOAT32, "stage_data_03"},
	 {1, TYPE_FLOAT32, "stage_data_04"},
	 {1, TYPE_FLOAT32, "stage_data_05"},
	 {1, TYPE_FLOAT32, "stage_data_06"},
	 {1, TYPE_FLOAT32, "stage_data_07"},
	 {1, TYPE_FLOAT32, "stage_data_08"},
	 {1, TYPE_FLOAT32, "stage_data_09"},
	 {1, TYPE_FLOAT32, "stage_data_10"},
	 {1, TYPE_FLOAT32, "stage_data_11"},
	 {1, TYPE_FLOAT32, "stage_data_12"},
	 {1, TYPE_FLOAT32, "stage_data_13"},
	 {1, TYPE_FLOAT32, "stage_data_14"},
	 {1, TYPE_FLOAT32, "stage_data_15"},
	 {1, TYPE_FLOAT32, "stage_data_16"},
	 {1, TYPE_FLOAT32, "stage_data_17"},
	 {1, TYPE_FLOAT32, "stage_data_18"},
	 {1, TYPE_FLOAT32, "stage_data_19"},
	 {1, TYPE_FLOAT32, "stage_data_20"},
	 {1, TYPE_INT32, "sub_stage_on_hit_id"},
	 {1, TYPE_INT32, "sub_stage_on_hit_continuous"},
	 {1, TYPE_INT32, "hit_fx_output_delay"},
	 {1, TYPE_INT32, "hit_fx_set_id"},
	 {1, TYPE_INT32, "hit_fx_set_position_id"},
	 {1, TYPE_INT32, "fire_fx_output_delay"},
	 {1, TYPE_INT32, "sub_fire_fx_set_id"},
	 {1, TYPE_INT32, "sub_fire_fx_position"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultTableName=_getDefaultTableName@0")
#endif
EDATABASEDLL const char*  DLLCALLCONV getDefaultTableName() {
	return "SkillStage";
}

#ifdef __cplusplus
}
#endif
