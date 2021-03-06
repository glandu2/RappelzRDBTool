#include "DataType.h"
#include "ExportDLL.h"
#include "IRowManipulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "id"},
                               {1, TYPE_INT32, "name_id"},
                               {1, TYPE_INT32, "type"},
                               {1, TYPE_INT32, "group"},
                               {1, TYPE_INT32, "class"},
                               {1, TYPE_INT32, "rank"},
                               {1, TYPE_INT32, "use_min_level"},
                               {1, TYPE_INT32, "use_max_level"},
                               {1, TYPE_INT32, "target_min_level"},
                               {1, TYPE_INT32, "target_max_level"},
                               {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "range"},
                               {1, TYPE_INT32, "weight"},
                               {1, TYPE_INT32, "price"},
                               {1, TYPE_INT32, "endurance"},
                               {1, TYPE_INT32, "wear_type"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv0"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv1"},
                               {1, TYPE_BIT, "limit_deva"},
                               {1, TYPE_BIT, "limit_asura"},
                               {1, TYPE_BIT, "limit_gaia"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv2"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv3"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv4"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv5"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv6"},
                               {1, TYPE_BIT, "limit_fighter"},
                               {1, TYPE_BIT, "limit_hunter"},
                               {1, TYPE_BIT, "limit_magician"},
                               {1, TYPE_BIT, "limit_summoner"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv7"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv8"},
                               {1, TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv9"},
                               {1, TYPE_BIT, "flag_cashitem"},  // 0
                               {1, TYPE_BIT, "flag_wear"},      // 1
                               {1, TYPE_BIT, "flag_storage"},
                               {1, TYPE_BIT, "flag_enhance"},
                               {1, TYPE_BIT, "flag_use"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv10"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv11"},
                               {1, TYPE_BIT, "flag_duplicate"},  // 7
                               {1, TYPE_BIT, "flag_target_use"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv12"},
                               {1, TYPE_BIT, "flag_trade"},
                               {1, TYPE_BIT, "flag_sell"},
                               {1, TYPE_BIT, "flag_quest"},
                               {1, TYPE_BIT, "flag_overweight"},
                               {1, TYPE_BIT, "flag_riding"},  // not ok, always 0
                               {1, TYPE_BIT, "flag_drop"},    // ok
                               {1, TYPE_BIT, "flag_move"},
                               {1, TYPE_BIT, "flag_sit"},
                               {1, TYPE_BIT, "flag_raid"},
                               {1, TYPE_BIT, "flag_secroute"},
                               {1, TYPE_BIT, "flag_eventmap"},
                               {1, TYPE_BIT, "flag_huntaholic"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv13"},
                               {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv14"},
                               {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv15"},
                               {1, TYPE_INT32, "material"},
                               {1, TYPE_INT32, "summon_id"},
                               {1, TYPE_INT32, "throw_range"},
                               {1, TYPE_INT16, "base_type_0"},
                               {1, TYPE_INT16, "base_type_1"},
                               {1, TYPE_INT16, "base_type_2"},
                               {1, TYPE_INT16, "base_type_3"},
                               {1, TYPE_FLOAT64, "base_var1_0"},
                               {1, TYPE_FLOAT64, "base_var1_1"},
                               {1, TYPE_FLOAT64, "base_var1_2"},
                               {1, TYPE_FLOAT64, "base_var1_3"},
                               {1, TYPE_FLOAT64, "base_var2_0"},
                               {1, TYPE_FLOAT64, "base_var2_1"},
                               {1, TYPE_FLOAT64, "base_var2_2"},
                               {1, TYPE_FLOAT64, "base_var2_3"},
                               {1, TYPE_INT16, "opt_type_0"},
                               {1, TYPE_INT16, "opt_type_1"},
                               {1, TYPE_INT16, "opt_type_2"},
                               {1, TYPE_INT16, "opt_type_3"},
                               {1, TYPE_FLOAT64, "opt_var1_0"},
                               {1, TYPE_FLOAT64, "opt_var1_1"},
                               {1, TYPE_FLOAT64, "opt_var1_2"},
                               {1, TYPE_FLOAT64, "opt_var1_3"},
                               {1, TYPE_FLOAT64, "opt_var2_0"},
                               {1, TYPE_FLOAT64, "opt_var2_1"},
                               {1, TYPE_FLOAT64, "opt_var2_2"},
                               {1, TYPE_FLOAT64, "opt_var2_3"},
                               {1, TYPE_INT16, "enhance_0_id"},
                               {1, TYPE_INT16, "enhance_1_id"},
                               {1, TYPE_FLOAT32, "enhance_0_01"},
                               {1, TYPE_FLOAT32, "enhance_0_02"},
                               {1, TYPE_FLOAT32, "enhance_0_03"},
                               {1, TYPE_FLOAT32, "enhance_0_04"},
                               {1, TYPE_FLOAT32, "enhance_1_01"},
                               {1, TYPE_FLOAT32, "enhance_1_02"},
                               {1, TYPE_FLOAT32, "enhance_1_03"},
                               {1, TYPE_FLOAT32, "enhance_1_04"},
                               {1, TYPE_INT32, "skill_id"},
                               {1, TYPE_INT32, "state_id"},
                               {1, TYPE_INT32, "state_level"},
                               {1, TYPE_INT32, "state_time"},
                               {1, TYPE_INT8, "state_type?"},
                               {1, TYPE_INT32, "cool_time"},
                               {1, TYPE_INT16, "cool_time_group"},
                               {1, TYPE_INT32, "available_period"},
                               {1, TYPE_INT8, "decrease_type"},
                               {1, TYPE_INT32, "model_type_dem"},
                               {1, TYPE_INT32, "model_type_def"},
                               {1, TYPE_INT32, "model_type_asm"},
                               {1, TYPE_INT32, "model_type_asf"},
                               {1, TYPE_INT32, "model_type_gam"},
                               {1, TYPE_INT32, "model_type_gaf"},
                               {256, TYPE_CHAR, "model_00"},
                               {256, TYPE_CHAR, "model_01"},
                               {256, TYPE_CHAR, "model_02"},
                               {256, TYPE_CHAR, "model_03"},
                               {256, TYPE_CHAR, "model_04"},
                               {256, TYPE_CHAR, "model_05"},
                               {256, TYPE_CHAR, "model_06"},
                               {256, TYPE_CHAR, "model_07"},
                               {256, TYPE_CHAR, "model_08"},
                               {256, TYPE_CHAR, "model_09"},
                               {256, TYPE_CHAR, "model_10"},
                               {256, TYPE_CHAR, "model_11"},
                               {256, TYPE_CHAR, "model_12"},
                               {256, TYPE_CHAR, "model_13"},
                               {256, TYPE_CHAR, "model_14"},
                               {256, TYPE_CHAR, "model_15"},
                               {256, TYPE_CHAR, "model_16"},
                               {256, TYPE_CHAR, "model_17"},
                               {256, TYPE_CHAR, "drop_type"},
                               {1, TYPE_INT32, "icon_id"},
                               {256, TYPE_CHAR, "icon_file_name"},
                               {512, TYPE_CHAR, "script_text"},
                               {1, TYPE_INT32, "tooltip_id"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}
/*
#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
#endif
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
            "rank\0"
            "level\0"
            "enhance\0"
            "socket\0"
            "status_flag\0"
            "limit_deva\0"
            "limit_asura\0"
            "limit_gaia\0"
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
            "endurance\0"
            "material\0"
            "summon_id\0"
            "flag_cashitem\0"
            "flag_wear\0"
            "flag_use\0"
            "flag_target_use\0"
            "flag_duplicate\0"
            "flag_drop\0"
            "flag_trade\0"
            "flag_sell\0"
            "flag_storage\0"
            "flag_overweight\0"
            "flag_riding\0"
            "flag_move\0"
            "flag_sit\0"
            "flag_enhance\0"
            "flag_quest\0"
            "flag_raid\0"
            "flag_secroute\0"
            "flag_eventmap\0"
            "flag_huntaholic\0"
            "available_period\0"
            "decrease_type\0"
            "throw_range\0"
            "distribute_type\0" //
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
            "state_type\0"         //
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
            "drop_type\0"
            "icon_id\0"
            "icon_file_name\0"
            "script_text\0";
}
*/
#ifndef _WIN64
#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
#endif
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst,
                                          eDataConvertionType mode,
                                          IRowManipulator* row,
                                          unsigned int rowNum) {
	/*if(mode == DCT_Read && dst != DF_RDB) {
	    char nvValues[4];
	    int i;
	    for(i=0; i<16; i++) {
	        sprintf(nvValues, "nv%d", i);
	        row->setDataBit(nvValues, 0);
	    }
	}*/
}

#ifdef __cplusplus
}
#endif
