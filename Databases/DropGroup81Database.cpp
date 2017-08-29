#include "IRowManipulator.h"
#include <stdio.h>
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
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
	 {1, TYPE_FLOAT32, "drop_percentage_00"},
	 {1, TYPE_FLOAT32, "drop_percentage_01"},
	 {1, TYPE_FLOAT32, "drop_percentage_02"},
	 {1, TYPE_FLOAT32, "drop_percentage_03"},
	 {1, TYPE_FLOAT32, "drop_percentage_04"},
	 {1, TYPE_FLOAT32, "drop_percentage_05"},
	 {1, TYPE_FLOAT32, "drop_percentage_06"},
	 {1, TYPE_FLOAT32, "drop_percentage_07"},
	 {1, TYPE_FLOAT32, "drop_percentage_08"},
	 {1, TYPE_FLOAT32, "drop_percentage_09"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_00"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_01"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_02"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_03"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_04"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_05"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_06"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_07"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_08"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_min_count_09"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_00"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_01"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_02"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_03"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_04"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_05"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_06"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_07"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_08"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "drop_max_count_09"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
#endif
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(dst == DF_RDB && mode == DCT_Read) {
		char columnName[20];
		int i, value;

		for(i=0; i<10; i++) {
			sprintf(columnName, "drop_item_id_%02d", i);
			value = row->getDataInt32(columnName);
			sprintf(columnName, "drop_min_count_%02d", i);
			if(value != 0)
				row->setDataInt32(columnName, 1);
			else
				row->setDataInt32(columnName, 0);
			sprintf(columnName, "drop_max_count_%02d", i);
			if(value != 0)
				row->setDataInt32(columnName, 1);
			else
				row->setDataInt32(columnName, 0);
		}
	}
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "id\0"
			"drop_item_id_00\0"
			"drop_min_count_00\0"
			"drop_max_count_00\0"
			"drop_percentage_00\0"
			"drop_item_id_01\0"
			"drop_min_count_01\0"
			"drop_max_count_01\0"
			"drop_percentage_01\0"
			"drop_item_id_02\0"
			"drop_min_count_02\0"
			"drop_max_count_02\0"
			"drop_percentage_02\0"
			"drop_item_id_03\0"
			"drop_min_count_03\0"
			"drop_max_count_03\0"
			"drop_percentage_03\0"
			"drop_item_id_04\0"
			"drop_min_count_04\0"
			"drop_max_count_04\0"
			"drop_percentage_04\0"
			"drop_item_id_05\0"
			"drop_min_count_05\0"
			"drop_max_count_05\0"
			"drop_percentage_05\0"
			"drop_item_id_06\0"
			"drop_min_count_06\0"
			"drop_max_count_06\0"
			"drop_percentage_06\0"
			"drop_item_id_07\0"
			"drop_min_count_07\0"
			"drop_max_count_07\0"
			"drop_percentage_07\0"
			"drop_item_id_08\0"
			"drop_min_count_08\0"
			"drop_max_count_08\0"
			"drop_percentage_08\0"
			"drop_item_id_09\0"
			"drop_min_count_09\0"
			"drop_max_count_09\0"
			"drop_percentage_09\0";
}

#ifdef __cplusplus
}
#endif

