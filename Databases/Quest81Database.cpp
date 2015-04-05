#include "IRowManipulator.h"
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO: complete decription

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "text_id_quest"},
	 {1, TYPE_INT32, "text_id_summary"},
	 {1, TYPE_INT32, "text_id_status"},
	 {1, TYPE_INT32, "limit_begin_time"},
	 {1, TYPE_INT32, "limit_end_time"},
	 {1, TYPE_INT32, "limit_level"},
	 {1, TYPE_INT32, "limit_job_level"},
	 {1, TYPE_INT32, "limit_max_level"},
	 {1, TYPE_INT32, "limit_max_job_level"},
	 {1, TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "nv"},
	 {1, TYPE_BIT, "limit_deva"},
	 {1, TYPE_BIT, "limit_asura"},
	 {1, TYPE_BIT, "limit_gaia"},
	 {1, TYPE_BIT, "limit_fighter"},
	 {1, TYPE_BIT, "limit_hunter"},
	 {1, TYPE_BIT, "limit_magician"},
	 {1, TYPE_BIT, "limit_summoner"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "padding0"},
	 {1, TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "padding1"},
	 {1, TYPE_INT32, "limit_job"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "limit_favor_group_id"},
	 {1, TYPE_INT32, "limit_favor"},
	 {1, TYPE_INT8, "repeatable"},   //char(1) in DB
	 {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknown1"},
	 {1, TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknown2"},
	 {1, TYPE_INT32, "invoke_condition"},
	 {1, TYPE_INT32, "invoke_value"},
	 {1, TYPE_INT16, "time_limit_type"},
	 {1, TYPE_INT16, "job_depth"},	//not sure, 22 in rdb and most is 15 in DB
	 {1, TYPE_INT32, "time_limit"},
	 {1, TYPE_INT32, "type"},
	 {1, TYPE_INT32, "value1"},
	 {1, TYPE_INT32, "value2"},
	 {1, TYPE_INT32, "value3"},
	 {1, TYPE_INT32, "value4"},
	 {1, TYPE_INT32, "value5"},
	 {1, TYPE_INT32, "value6"},
	 {1, TYPE_INT32, "value7"},
	 {1, TYPE_INT32, "value8"},
	 {1, TYPE_INT32, "value9"},
	 {1, TYPE_INT32, "value10"},
	 {1, TYPE_INT32, "value11"},
	 {1, TYPE_INT32, "value12"},
	 {1, TYPE_INT32, "drop_group_id"},
	 {1, TYPE_INT32, "quest_difficulty"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "favor_group_id"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "hate_group_id"},
	 {1, TYPE_INT32, "favor"},
	 {1, TYPE_INT32, "exp"},
	 {1, TYPE_INT32, "jp"},
	 {1, TYPE_INT32, "holicpoint"},
	 {1, TYPE_INT32, "gold"},
	 {1, TYPE_INT32, "default_reward_id"},
	 {1, TYPE_INT32, "default_reward_level"},
	 {1, TYPE_INT32, "default_reward_quantity"},
	 {1, TYPE_INT32, "optional_reward_id1"},
	 {1, TYPE_INT32, "optional_reward_level1"},
	 {1, TYPE_INT32, "optional_reward_quantity1"},
	 {1, TYPE_INT32, "optional_reward_id2"},
	 {1, TYPE_INT32, "optional_reward_level2"},
	 {1, TYPE_INT32, "optional_reward_quantity2"},
	 {1, TYPE_INT32, "optional_reward_id3"},
	 {1, TYPE_INT32, "optional_reward_level3"},
	 {1, TYPE_INT32, "optional_reward_quantity3"},
	 {1, TYPE_INT32, "optional_reward_id4"},
	 {1, TYPE_INT32, "optional_reward_level4"},
	 {1, TYPE_INT32, "optional_reward_quantity4"},
	 {1, TYPE_INT32, "optional_reward_id5"},
	 {1, TYPE_INT32, "optional_reward_level5"},
	 {1, TYPE_INT32, "optional_reward_quantity5"},
	 {1, TYPE_INT32, "optional_reward_id6"},
	 {1, TYPE_INT32, "optional_reward_level6"},
	 {1, TYPE_INT32, "optional_reward_quantity6"},
	 {1, TYPE_INT32, "forequest1"},
	 {1, TYPE_INT32, "forequest2"},
	 {1, TYPE_INT32, "forequest3"},
	 {1, TYPE_INT8, "or_flag"},
	 {512, TYPE_CHAR | TYPE_RDBIGNORE, "script_start_text"},
	 {512, TYPE_CHAR | TYPE_RDBIGNORE, "script_end_text"},
	 {512, TYPE_CHAR | TYPE_RDBIGNORE, "script_drop_text"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknown4"},
	 {1, TYPE_INT8, "show_target_type"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknownPadding"},
	 {1, TYPE_INT32, "show_target_id"},
	 {1, TYPE_INT8, "mark_hide"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknown5"},
	 {1, TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknown6"},
	 {1, TYPE_INT32, "cool_time"},
	 {1, TYPE_INT32, "accept_cool_time"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && (dst == DF_SQL || dst == DF_CSV)) {
		row->setDataBit("nv", 0);
		row->setDataInt8("padding0", 0);
		row->setDataInt16("padding1", 0);
		row->setDataInt8("unknown1", 0);
		row->setDataInt16("unknown2", 0);
		row->setDataInt8("unknown4", 55);
		row->setDataInt8("unknown5", 0);
		row->setDataInt16("unknown6", 0);
		row->setDataInt8("unknownPadding", 0);
	} else if(mode == DCT_Read && dst == DF_RDB) {
		int questId = row->getDataInt32("id");

		if(questId >= 3600 && questId <= 3611 && questId != 3610)
			row->setDataInt32("hate_group_id", 1);
		else
			row->setDataInt32("hate_group_id", 0);

		row->setDataInt32("favor_group_id", 999);
		row->setDataInt32("limit_favor_group_id", 0);

		row->setDataInt8("or_flag", row->getDataInt8("or_flag") * -1);

		row->setDataString("script_start_text", "0");
		row->setDataString("script_end_text", "0");
		row->setDataString("script_drop_text", "0");
	}
}

#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return "id\0"
			"text_id_quest\0"
			"text_id_summary\0"
			"text_id_status\0"
			"limit_begin_time\0"
			"limit_end_time\0"
			"limit_level\0"
			"limit_job_level\0"
			"limit_max_level\0"
			"limit_max_job_level\0"
			"limit_deva\0"
			"limit_asura\0"
			"limit_gaia\0"
			"limit_fighter\0"
			"limit_hunter\0"
			"limit_magician\0"
			"limit_summoner\0"
			"limit_job\0"
			"job_depth\0"
			"limit_favor_group_id\0"
			"limit_favor\0"
			"repeatable\0"
			"invoke_condition\0"
			"invoke_value\0"
			"time_limit_type\0"
			"time_limit\0"
			"type\0"
			"value1\0"
			"value2\0"
			"value3\0"
			"value4\0"
			"value5\0"
			"value6\0"
			"value7\0"
			"value8\0"
			"value9\0"
			"value10\0"
			"value11\0"
			"value12\0"
			"drop_group_id\0"
			"quest_difficulty\0"
			"favor_group_id\0"
			"hate_group_id\0"
			"favor\0"
			"exp\0"
			"jp\0"
			"holicpoint\0"
			"gold\0"
			"default_reward_id\0"
			"default_reward_level\0"
			"default_reward_quantity\0"
			"optional_reward_id1\0"
			"optional_reward_level1\0"
			"optional_reward_quantity1\0"
			"optional_reward_id2\0"
			"optional_reward_level2\0"
			"optional_reward_quantity2\0"
			"optional_reward_id3\0"
			"optional_reward_level3\0"
			"optional_reward_quantity3\0"
			"optional_reward_id4\0"
			"optional_reward_level4\0"
			"optional_reward_quantity4\0"
			"optional_reward_id5\0"
			"optional_reward_level5\0"
			"optional_reward_quantity5\0"
			"optional_reward_id6\0"
			"optional_reward_level6\0"
			"optional_reward_quantity6\0"
			"forequest1\0"
			"forequest2\0"
			"forequest3\0"
			"or_flag\0"
			"script_start_text\0"
			"script_end_text\0"
			"script_drop_text\0"
			"show_target_type\0"
			"show_target_id\0"
			"mark_hide\0"
			"cool_time\0"
			"accept_cool_time\0";
}

#ifdef __cplusplus
}
#endif

