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
	 {1, TYPE_INT32, "limit_max_joblevel"},
	 {1, TYPE_BIT | TYPE_SQLIGNORE, "nv"},
	 {1, TYPE_BIT, "limit_deva"},
	 {1, TYPE_BIT, "limit_asura"},
	 {1, TYPE_BIT, "limit_gaia"},
	 {1, TYPE_BIT, "limit_fighter"},
	 {1, TYPE_BIT, "limit_hunter"},
	 {1, TYPE_BIT, "limit_magician"},
	 {1, TYPE_BIT, "limit_summoner"},
	 {1, TYPE_INT8, "unknown0"},
	 {1, TYPE_INT8, "unknown1"},
	 {1, TYPE_INT8, "unknown2"},
	 {1, TYPE_INT32, "limit_job"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "limit_favor_group_id"},
	 {1, TYPE_INT32, "limit_favor"},
	 {1, TYPE_INT32, "repeatable"},
	 {1, TYPE_INT32, "invoke_condition"},
	 {1, TYPE_INT32, "invoke_value"},
	 {1, TYPE_INT16, "time_limit_type"},
	 {1, TYPE_INT16, "limit_job_depth"},	//not sure
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
	 {1, TYPE_INT16 | TYPE_SQLIGNORE, "unknownFlag"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "unknownPadding"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst != DF_RDB) {
		int questId = row->getDataInt32("id");
		row->setDataBit("nv", 0);
		row->setDataInt8("unknown0", 0);
		row->setDataInt8("unknown1", 0);
		row->setDataInt8("unknown2", 0);
		row->setDataInt8("unknownPadding", 0);
		row->setDataInt16("limit_favor_group_id", 0);
		row->setDataInt16("favor_group_id", 999);
		if(questId >= 3600 && questId <= 3611 && questId != 3610)
			row->setDataInt16("hate_group_id", 1);
		else
			row->setDataInt16("hate_group_id", 0);
	}
}

#ifdef __cplusplus
}
#endif

