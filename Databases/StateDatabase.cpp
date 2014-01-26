#include "IRowManipulator.h"
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "state_id"},
	 {1, TYPE_INT32, "text_id"},
	 {1, TYPE_INT32, "tooltip_id"},
	 {1, TYPE_INT32, "is_harmful"},
	 {1, TYPE_INT32, "state_time_type"},
	 {1, TYPE_INT32, "state_group"},
	 {1, TYPE_INT32, "duplicate_group_1"},
	 {1, TYPE_INT32, "duplicate_group_2"},
	 {1, TYPE_INT32, "duplicate_group_3"},
	 {1, TYPE_INT8, "uf_avatar"},
	 {1, TYPE_INT8, "uf_summon"},
	 {1, TYPE_INT8, "uf_monster"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "unknown0"},
	 {1, TYPE_INT8, "reiteration_count"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "unknown1"},
	 {1, TYPE_INT16 | TYPE_SQLIGNORE, "unknown2"},
	 {1, TYPE_INT32, "base_effect_id"},
	 {1, TYPE_INT32, "fire_interval"},
	 {1, TYPE_INT32, "elemental_type"},
	 {1, TYPE_FLOAT32, "amplify_base"},
	 {1, TYPE_FLOAT32, "amplify_per_skl"},
	 {1, TYPE_INT32, "add_damage_base"},
	 {1, TYPE_INT32, "add_damage_per_skl"},
	 {1, TYPE_INT32, "effect_type"},
	 {1, TYPE_INT32 | TYPE_SQLIGNORE, "unknown3"},
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
	 {1, TYPE_INT32, "special_output_fx_delay"},
	 {1, TYPE_INT32, "state_fx_id"},
	 {1, TYPE_INT32, "state_fx_pos_id"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Write && dst == DF_RDB) {
		*static_cast<char*>(row->getValuePtr("unknown0")) = 0;
		*static_cast<char*>(row->getValuePtr("unknown1")) = 0;
		*static_cast<short*>(row->getValuePtr("unknown2")) = 163;
		*static_cast<int*>(row->getValuePtr("unknown3")) = 15662196;
	}
}

#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "state_id\0"
			"text_id\0"
			"tooltip_id\0"
			"is_harmful\0"
			"state_time_type\0"
			"state_group\0"
			"duplicate_group_1\0"
			"duplicate_group_2\0"
			"duplicate_group_3\0"
			"uf_avatar\0"
			"uf_summon\0"
			"uf_monster\0"
			"reiteration_count\0"
			"base_effect_id\0"
			"fire_interval\0"
			"elemental_type\0"
			"amplify_base\0"
			"amplify_per_skl\0"
			"add_damage_base\0"
			"add_damage_per_skl\0"
			"effect_type\0"
			"value_0\0"
			"value_1\0"
			"value_2\0"
			"value_3\0"
			"value_4\0"
			"value_5\0"
			"value_6\0"
			"value_7\0"
			"value_8\0"
			"value_9\0"
			"value_10\0"
			"value_11\0"
			"value_12\0"
			"value_13\0"
			"value_14\0"
			"value_15\0"
			"value_16\0"
			"value_17\0"
			"value_18\0"
			"value_19\0"
			"icon_id\0"
			"icon_file_name\0"
			"fx_id\0"
			"pos_id\0"
			"cast_skill_id\0"
			"cast_fx_id\0"
			"cast_fx_pos_id\0"
			"hit_fx_id\0"
			"hit_fx_pos_id\0"
			"special_output_timing_id\0"
			"special_output_fx_id\0"
			"special_output_fx_pos_id\0"
			"special_output_fx_delay\0"
			"state_fx_id\0"
			"state_fx_pos_id\0";
}

#ifdef __cplusplus
}
#endif


