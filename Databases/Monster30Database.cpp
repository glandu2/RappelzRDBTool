#include "DataType.h"
#include "ExportDLL.h"
#include "IRowManipulator.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

int encodeMap[32];
int decodeMap[32];

static FieldDescriptor df[] = {{1, TYPE_INT32, "id"},
                               {1, TYPE_INT32, "monster_group"},
                               {1, TYPE_INT32, "monster_type"},
                               {1, TYPE_INT32, "name_id"},
                               {1, TYPE_INT32, "location_id"},
                               {1, TYPE_INT32, "race"},
                               {1, TYPE_INT32, "grp"},
                               {1, TYPE_INT32, "transform_level"},
                               {1, TYPE_INT32, "level"},
                               {1, TYPE_FLOAT32, "size"},
                               {1, TYPE_FLOAT32, "scale"},
                               {1, TYPE_FLOAT32, "target_fx_size"},
                               {1, TYPE_INT32, "walk_type"},
                               {1, TYPE_INT32, "slant_type"},
                               {1, TYPE_INT32, "visible_range"},
                               {1, TYPE_INT32, "chase_range"},
                               {1, TYPE_INT32, "magic_type"},
                               {1, TYPE_BIT, "f_fisrt_attack"},
                               {1, TYPE_BIT, "f_group_first_attack"},
                               {1, TYPE_BIT, "f_response_casting"},
                               {1, TYPE_BIT, "f_response_race"},
                               {1, TYPE_BIT, "f_response_battle"},
                               {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "padding0"},
                               {1, TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "padding1"},
                               {1, TYPE_INT32, "hp"},
                               {1, TYPE_INT32, "mp"},
                               {1, TYPE_INT32, "attack_point"},
                               {1, TYPE_INT32, "magic_point"},
                               {1, TYPE_INT32, "defence"},
                               {1, TYPE_INT32, "magic_defence"},
                               {1, TYPE_INT32, "attack_speed"},
                               {1, TYPE_INT32 | TYPE_SQLIGNORE, "attack_speed_type"},
                               {1, TYPE_INT32, "magic_speed"},
                               {1, TYPE_INT32, "accuracy"},
                               {1, TYPE_INT32, "avoid"},
                               {1, TYPE_INT32, "magic_accuracy"},
                               {1, TYPE_INT32, "magic_avoid"},
                               {1, TYPE_INT32, "taming_id"},
                               {1, TYPE_FLOAT32, "taming_percentage"},
                               {1, TYPE_FLOAT32, "taming_exp_mod"},
                               {1, TYPE_INT32, "exp"},
                               {1, TYPE_INT32, "jp"},
                               {1, TYPE_INT32, "stat_id"},
                               {1, TYPE_INT32, "ability"},
                               {1, TYPE_INT32, "standard_walk_spped"},
                               {1, TYPE_INT32, "standard_run_spped"},
                               {1, TYPE_INT32, "walk_speed"},
                               {1, TYPE_INT32, "run_speed"},
                               {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "attack_range"},
                               {1, TYPE_INT32, "gold_drop_percentage"},
                               {1, TYPE_INT32, "gold_min"},
                               {1, TYPE_INT32, "gold_max"},
                               {1, TYPE_INT32, "chaos_drop_percentage"},
                               {1, TYPE_INT32, "chaos_min"},
                               {1, TYPE_INT32, "chaos_max"},
                               {1, TYPE_INT32, "drop_item_id_00"},
                               {1, TYPE_INT32, "drop_item_id_01"},
                               {1, TYPE_INT32, "drop_item_id_02"},
                               {1, TYPE_INT32, "drop_item_id_03"},
                               {1, TYPE_INT32, "drop_item_id_04"},
                               {1, TYPE_INT32, "drop_item_id_05"},
                               {1, TYPE_INT32, "drop_item_id_06"},
                               {1, TYPE_INT32, "drop_item_id_07"},
                               {1, TYPE_INT32, "drop_item_id_08"},
                               {1, TYPE_INT32, "drop_item_id_09"},
                               {1, TYPE_INT32, "drop_percentage_00"},
                               {1, TYPE_INT32, "drop_percentage_01"},
                               {1, TYPE_INT32, "drop_percentage_02"},
                               {1, TYPE_INT32, "drop_percentage_03"},
                               {1, TYPE_INT32, "drop_percentage_04"},
                               {1, TYPE_INT32, "drop_percentage_05"},
                               {1, TYPE_INT32, "drop_percentage_06"},
                               {1, TYPE_INT32, "drop_percentage_07"},
                               {1, TYPE_INT32, "drop_percentage_08"},
                               {1, TYPE_INT32, "drop_percentage_09"},
                               {20, TYPE_CHAR, "unknown"},
                               {1, TYPE_INT32, "unknown3"},
                               {1, TYPE_INT32, "exp_2"},
                               {1, TYPE_INT32, "jp_2"},
                               {1, TYPE_INT32, "gold_min_2"},
                               {1, TYPE_INT32, "gold_max_2"},
                               {1, TYPE_INT32, "chaos_min_2"},
                               {1, TYPE_INT32, "chaos_max_2"},
                               {1, TYPE_INT32, "drop_table_link_id"},
                               {1, TYPE_INT32, "texture_group"},
                               {1, TYPE_INT32, "local_flag"},
                               {1, TYPE_INT32, "skill1_id"},
                               {1, TYPE_INT32, "skill1_lv"},
                               {1, TYPE_INT32, "skill1_probability"},
                               {1, TYPE_INT32, "skill2_id"},
                               {1, TYPE_INT32, "skill2_lv"},
                               {1, TYPE_INT32, "skill2_probability"},
                               {1, TYPE_INT32, "skill3_id"},
                               {1, TYPE_INT32, "skill3_lv"},
                               {1, TYPE_INT32, "skill3_probability"},
                               {1, TYPE_INT32, "skill4_id"},
                               {1, TYPE_INT32, "skill4_lv"},
                               {1, TYPE_INT32, "skill4_probability"},
                               {256, TYPE_CHAR, "model"},
                               {1, TYPE_INT32, "motion_file_id"},
                               {1, TYPE_INT32, "weapon_type"},
                               {1, TYPE_INT32, "camera_x"},
                               {1, TYPE_INT32, "camera_y"},
                               {1, TYPE_INT32, "camera_z"},
                               {1, TYPE_FLOAT32, "target_x"},
                               {1, TYPE_FLOAT32, "target_y"},
                               {1, TYPE_FLOAT32, "target_z"},
                               {1, TYPE_INT32, "material"},
                               {1, TYPE_INT32, "attack_motion_speed"},
                               {1, TYPE_INT32, "fight_type"},
                               {1, TYPE_INT32, "trigger_00_type"},
                               {1, TYPE_FLOAT32, "trigger_00_value_1"},
                               {1, TYPE_FLOAT32, "trigger_00_value_2"},
                               {255, TYPE_CHAR, "trigger_00"},
                               {1, TYPE_INT32, "trigger_01_type"},
                               {1, TYPE_FLOAT32, "trigger_01_value_1"},
                               {1, TYPE_FLOAT32, "trigger_01_value_2"},
                               {255, TYPE_CHAR, "trigger_01"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);

	// bit swaping map for monster id
	int i, j, oldValue;
	for(i = 0; i < 32; i++)
		encodeMap[i] = i;
	for(i = 0, j = 3; i < 32; i++) {
		oldValue = encodeMap[i];
		encodeMap[i] = encodeMap[j];
		encodeMap[j] = oldValue;
		j = (j + i + 3) % 32;
	}
	for(i = 0; i < 32; i++)
		decodeMap[encodeMap[i]] = i;
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
#endif
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst,
                                          eDataConvertionType mode,
                                          IRowManipulator* row,
                                          unsigned int rowNum) {
	int value, result, i;
	if(mode == DCT_Read && dst == DF_RDB) {
		value = row->getDataInt32("id");
		for(i = 0, result = 0; i < 32; i++) {
			if((1 << i) & value)
				result |= 1 << decodeMap[i];
		}
		row->setDataInt32("id", result);
	} else if(mode == DCT_Write && dst == DF_RDB) {
		value = row->getDataInt32("id");
		for(i = 0, result = 0; i < 32; i++) {
			if((1 << i) & value)
				result |= 1 << encodeMap[i];
		}
		row->setDataInt32("id", result);
		row->setDataInt8("padding0", 0);
		row->setDataInt16("padding1", 0);
	} else if(mode == DCT_Read && DF_SQL) {
		row->setDataInt32("attack_speed_type", 15);
	}
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return "id\0"
	       "monster_group\0"
	       "name_id\0"
	       "location_id\0"
	       "model\0"
	       "motion_file_id\0"
	       "transform_level\0"
	       "walk_type\0"
	       "slant_type\0"
	       "size\0"
	       "scale\0"
	       "target_fx_size\0"
	       "camera_x\0"
	       "camera_y\0"
	       "camera_z\0"
	       "target_x\0"
	       "target_y\0"
	       "target_z\0"
	       "level\0"
	       "grp\0"
	       "magic_type\0"
	       "race\0"
	       "visible_range\0"
	       "chase_range\0"
	       "f_fisrt_attack\0"
	       "f_group_first_attack\0"
	       "f_response_casting\0"
	       "f_response_race\0"
	       "f_response_battle\0"
	       "monster_type\0"
	       "stat_id\0"
	       "fight_type\0"
	       "monster_skill_link_id\0"
	       "material\0"
	       "weapon_type\0"
	       "attack_motion_speed\0"
	       "ability\0"
	       "standard_walk_spped\0"
	       "standard_run_spped\0"
	       "walk_speed\0"
	       "run_speed\0"
	       "attack_range\0"
	       "hidesense_range\0"
	       "hp\0"
	       "mp\0"
	       "attack_point\0"
	       "magic_point\0"
	       "defence\0"
	       "magic_defence\0"
	       "attack_speed\0"
	       "magic_speed\0"
	       "accuracy\0"
	       "avoid\0"
	       "magic_accuracy\0"
	       "magic_avoid\0"
	       "taming_id\0"
	       "taming_percentage\0"
	       "taming_exp_mod\0"
	       "exp\0"
	       "jp\0"
	       "gold_drop_percentage\0"
	       "gold_min\0"
	       "gold_max\0"
	       "chaos_drop_percentage\0"
	       "chaos_min\0"
	       "chaos_max\0"
	       "exp_2\0"
	       "jp_2\0"
	       "gold_min_2\0"
	       "gold_max_2\0"
	       "chaos_min_2\0"
	       "chaos_max_2\0"
	       "drop_table_link_id\0"
	       "texture_group\0"
	       "local_flag\0"
	       "script_on_dead\0";
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getCSVColumnOrder=_getSQLColumnOrder@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getCSVColumnOrder() {
	return getSQLColumnOrder();
}

#ifdef __cplusplus
}
#endif
