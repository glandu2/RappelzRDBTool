#include "IRowManipulator.h"
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{ 1, TYPE_INT32, "id" },
	{ 1, TYPE_INT32, "name_id" },
	{ 1, TYPE_INT32, "type" },
	{ 1, TYPE_INT32, "group" },
	{ 1, TYPE_INT32, "class" },
	{ 1, TYPE_INT32, "set_id" },
	{ 1, TYPE_INT32, "set_part_flag" },
	{ 1, TYPE_INT8, "grade" },
	{ 1, TYPE_INT32, "rank" },
	{ 1, TYPE_INT32, "level" },
	{ 1, TYPE_INT32, "enhance" },
	{ 1, TYPE_INT32, "socket" },
	{ 1, TYPE_INT32, "status_flag" },
	{ 1, TYPE_INT8, "job_depth" },
	{ 1, TYPE_INT32, "use_min_level" },
	{ 1, TYPE_INT32, "use_max_level" },
	{ 1, TYPE_INT32, "target_min_level" },
	{ 1, TYPE_INT32, "target_max_level" },
	{ MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "range" },
	{ 1, TYPE_FLOAT32, "weight" },
	{ 1, TYPE_INT32, "price" },
	{ 1, TYPE_INT32, "huntaholic_point" },
	{ 1, TYPE_INT32 | TYPE_RDBIGNORE, "arena_points" },
	{ 1, TYPE_INT32, "ethereal_durability" },
	{ 1, TYPE_INT32, "endurance" },
	{ 1, TYPE_INT32, "wear_type" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv0" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv1" },
	{ 1, TYPE_BIT, "limit_deva" },
	{ 1, TYPE_BIT, "limit_asura" },
	{ 1, TYPE_BIT, "limit_gaia" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv2" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv3" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv4" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv5" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv6" },
	{ 1, TYPE_BIT, "limit_fighter" },
	{ 1, TYPE_BIT, "limit_hunter" },
	{ 1, TYPE_BIT, "limit_magician" },
	{ 1, TYPE_BIT, "limit_summoner" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv7" },
	{ 1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv8" },
	{ 1, TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv9" },
	{ 1, TYPE_INT32, "item_use_flag" },
	{ 1, TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "item_use_flag2" },
	{ 1, TYPE_INT32, "material" },
	{ 1, TYPE_INT32, "summon_id" },
	{ 1, TYPE_INT32, "throw_range" },
	{ 1, TYPE_INT16, "base_type_0" },
	{ 1, TYPE_INT16, "base_type_1" },
	{ 1, TYPE_INT16, "base_type_2" },
	{ 1, TYPE_INT16, "base_type_3" },
	{ 1, TYPE_FLOAT64, "base_var1_0" },
	{ 1, TYPE_FLOAT64, "base_var1_1" },
	{ 1, TYPE_FLOAT64, "base_var1_2" },
	{ 1, TYPE_FLOAT64, "base_var1_3" },
	{ 1, TYPE_FLOAT64, "base_var2_0" },
	{ 1, TYPE_FLOAT64, "base_var2_1" },
	{ 1, TYPE_FLOAT64, "base_var2_2" },
	{ 1, TYPE_FLOAT64, "base_var2_3" },
	{ 1, TYPE_INT16, "opt_type_0" },
	{ 1, TYPE_INT16, "opt_type_1" },
	{ 1, TYPE_INT16, "opt_type_2" },
	{ 1, TYPE_INT16, "opt_type_3" },
	{ 1, TYPE_FLOAT64, "opt_var1_0" },
	{ 1, TYPE_FLOAT64, "opt_var1_1" },
	{ 1, TYPE_FLOAT64, "opt_var1_2" },
	{ 1, TYPE_FLOAT64, "opt_var1_3" },
	{ 1, TYPE_FLOAT64, "opt_var2_0" },
	{ 1, TYPE_FLOAT64, "opt_var2_1" },
	{ 1, TYPE_FLOAT64, "opt_var2_2" },
	{ 1, TYPE_FLOAT64, "opt_var2_3" },
	{ 1, TYPE_INT32, "effect_id" },
	{ 1, TYPE_INT16, "enhance_0_id" },
	{ 1, TYPE_INT16, "enhance_1_id" },
	{ 1, TYPE_FLOAT32, "enhance_0_01" },
	{ 1, TYPE_FLOAT32, "enhance_0_02" },
	{ 1, TYPE_FLOAT32, "enhance_0_03" },
	{ 1, TYPE_FLOAT32, "enhance_0_04" },
	{ 1, TYPE_FLOAT32, "enhance_1_01" },
	{ 1, TYPE_FLOAT32, "enhance_1_02" },
	{ 1, TYPE_FLOAT32, "enhance_1_03" },
	{ 1, TYPE_FLOAT32, "enhance_1_04" },
	{ 1, TYPE_INT32, "skill_id" },
	{ 1, TYPE_INT32, "state_id" },
	{ 1, TYPE_INT32, "state_level" },
	{ 1, TYPE_INT32, "state_time" },
	{ 1, TYPE_INT32, "cool_time" },
	{ 1, TYPE_INT16, "cool_time_group" },
	{ 1, TYPE_INT32, "available_period" },
	{ 1, TYPE_INT16, "decrease_type" },
	{ 1, TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknownValue1" },
	{ 1, TYPE_INT32, "model_type_dem" },
	{ 1, TYPE_INT32, "model_type_def" },
	{ 1, TYPE_INT32, "model_type_asm" },
	{ 1, TYPE_INT32, "model_type_asf" },
	{ 1, TYPE_INT32, "model_type_gam" },
	{ 1, TYPE_INT32, "model_type_gaf" },
	{ 1, TYPE_INT8, "deco_model_change" },
	{ 256, TYPE_CHAR, "model_00" },
	{ 256, TYPE_CHAR, "model_01" },
	{ 256, TYPE_CHAR, "model_02" },
	{ 256, TYPE_CHAR, "model_03" },
	{ 256, TYPE_CHAR, "model_04" },
	{ 256, TYPE_CHAR, "model_05" },
	{ 256, TYPE_CHAR, "model_06" },
	{ 256, TYPE_CHAR, "model_07" },
	{ 256, TYPE_CHAR, "model_08" },
	{ 256, TYPE_CHAR, "model_09" },
	{ 256, TYPE_CHAR, "model_10" },
	{ 256, TYPE_CHAR, "model_11" },
	{ 256, TYPE_CHAR, "model_12" },
	{ 256, TYPE_CHAR, "model_13" },
	{ 256, TYPE_CHAR, "model_14" },
	{ 256, TYPE_CHAR, "model_15" },
	{ 256, TYPE_CHAR, "model_16" },
	{ 256, TYPE_CHAR, "model_17" },
	{ 256, TYPE_CHAR, "texture_filename" },
	{ 256, TYPE_CHAR, "drop_type" },
	{ 3, TYPE_CHAR | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "unkCatValue" },	//always "Cat"
	{ 1, TYPE_INT32, "icon_id" },
	{ 256, TYPE_CHAR, "icon_file_name" },
	{ 512, TYPE_CHAR, "script_text" },
	{ 1, TYPE_INT32, "tooltip_id" },
	{ 1, TYPE_INT32, "unknownValue2" }};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "id\0"
			"name_id\0"
			"tooltip_id\0"
			"type\0"
			"group\0"
			"class\0"
			"wear_type\0"
			"set_id\0"
			"set_part_flag\0"
			"grade\0"
			"rank\0"
			"level\0"
			"enhance\0"
			"socket\0"
			"status_flag\0"
			"limit_deva\0"
			"limit_asura\0"
			"limit_gaia\0"
			"job_depth\0"
			"limit_fighter\0"
			"limit_hunter\0"
			"limit_magician\0"
			"limit_summoner\0"
			"use_min_level\0"
			"use_max_level\0"
			"target_min_level\0"
			"target_max_level\0"
			"range\0"
			"weight\0"
			"price\0"
			"huntaholic_point\0"
			"arena_points\0"
			"ethereal_durability\0"
			"endurance\0"
			"material\0"
			"summon_id\0"
			"item_use_flag\0"
			"available_period\0"
			"decrease_type\0"
			"throw_range\0"
			"base_type_0\0"
			"base_var1_0\0"
			"base_var2_0\0"
			"base_type_1\0"
			"base_var1_1\0"
			"base_var2_1\0"
			"base_type_2\0"
			"base_var1_2\0"
			"base_var2_2\0"
			"base_type_3\0"
			"base_var1_3\0"
			"base_var2_3\0"
			"opt_type_0\0"
			"opt_var1_0\0"
			"opt_var2_0\0"
			"opt_type_1\0"
			"opt_var1_1\0"
			"opt_var2_1\0"
			"opt_type_2\0"
			"opt_var1_2\0"
			"opt_var2_2\0"
			"opt_type_3\0"
			"opt_var1_3\0"
			"opt_var2_3\0"
			"effect_id\0"
			"enhance_0_id\0"
			"enhance_0_01\0"
			"enhance_0_02\0"
			"enhance_0_03\0"
			"enhance_0_04\0"
			"enhance_1_id\0"
			"enhance_1_01\0"
			"enhance_1_02\0"
			"enhance_1_03\0"
			"enhance_1_04\0"
			"skill_id\0"
			"state_id\0"
			"state_level\0"
			"state_time\0"
			"cool_time\0"
			"cool_time_group\0"
			"model_type_dem\0"
			"model_type_def\0"
			"model_type_asm\0"
			"model_type_asf\0"
			"model_type_gam\0"
			"model_type_gaf\0"
			"deco_model_change\0"
			"model_00\0"
			"model_01\0"
			"model_02\0"
			"model_03\0"
			"model_04\0"
			"model_05\0"
			"model_06\0"
			"model_07\0"
			"model_08\0"
			"model_09\0"
			"model_10\0"
			"model_11\0"
			"model_12\0"
			"model_13\0"
			"model_14\0"
			"model_15\0"
			"model_16\0"
			"model_17\0"
			"texture_filename\0"
			"drop_type\0"
			"icon_id\0"
			"icon_file_name\0"
			"script_text\0"
			"unknownValue2\0";
}

typedef struct {
	int id;
	int arena_points;
} ArenaPoints;

//Data values by Ne0@NCarbon
static ArenaPoints arenaPoints[] =
//      id     arena_points
	{{1100103, 20},
	 {3800209, 80},
	 {3800210, 80},
	 {3800211, 80},
	 {3800201, 90},
	 {3800202, 90},
	 {3800203, 110},
	 {3800204, 110},
	 {3800205, 110},
	 {3800206, 110},
	 {3800207, 110},
	 {3800208, 110},
	 {3800229, 6250},
	 {3800241, 6250},
	 {3800253, 6250},
	 {693030 , 6817},
	 {693031 , 6817},
	 {693032 , 6817},
	 {693033 , 6817},
	 {693034 , 6817},
	 {3800242, 8520},
	 {3800230, 8520},
	 {3800254, 8520},
	 {3800240, 11360},
	 {3800252, 11360},
	 {3800228, 11360},
	 {3800257, 18750},
	 {3800245, 18750},
	 {3800233, 18750},
	 {3800255, 19310},
	 {3800243, 19310},
	 {3800231, 19310},
	 {3800234, 25560},
	 {3800258, 25560},
	 {3800246, 25560},
	 {3800256, 34080},
	 {3800244, 34080},
	 {3800232, 34080},
	 {3800264, 56810},
	 {3800259, 57940},
	 {3800247, 57940},
	 {3800235, 57940},
	 {3800265, 78400},
	 {3800261, 93730},
	 {3800237, 93730},
	 {3800249, 93730},
	 {3800250, 127820},
	 {3800238, 127820},
	 {3800262, 127820},
	 {3800248, 170420},
	 {3800260, 170420},
	 {3800236, 170420},
	 {3800001, 227230},
	 {3800002, 227230},
	 {3800003, 227230},
	 {3800004, 227230},
	 {3800005, 227230},
	 {3800239, 289720},
	 {3800251, 289720},
	 {3800263, 289720},
	 {3800101, 1136160}};

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Write && dst == DF_RDB) {
		char *catValue = static_cast<char*>(row->getValuePtr("unkCatValue"));
		catValue[0] = 'C';
		catValue[1] = 'a';
		catValue[2] = 't';
		*static_cast<int*>(row->getValuePtr("item_use_flag2")) = *static_cast<int*>(row->getValuePtr("item_use_flag"));
		*static_cast<short*>(row->getValuePtr("unknownValue1")) = 49;

		char nvValues[4];
		int i;
		for(i=0; i<9; i++) {
			sprintf(nvValues, "nv%d", i);
			*static_cast<char*>(row->getValuePtr(nvValues)) = 0;
		}
		*static_cast<short*>(row->getValuePtr("nv9")) = 0;
	} else if(mode == DCT_Read && dst == DF_RDB) {
		int id = *static_cast<int*>(row->getValuePtr("id"));
		int* arena_points_column = static_cast<int*>(row->getValuePtr("arena_points"));
		int i;
		for(i = 0; i < sizeof(arenaPoints) / sizeof(ArenaPoints); i++) {
			if(arenaPoints[i].id == id) {
				*arena_points_column = arenaPoints[i].arena_points;
				break;
			}
		}
	}
}

#ifdef __cplusplus
}
#endif
