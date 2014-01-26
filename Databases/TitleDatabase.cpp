#include "IRowManipulator.h"
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "name_id"},
	 {1, TYPE_INT32, "requisite_id"},
	 {1, TYPE_INT32, "status_id"},
	 {1, TYPE_INT8, "difficulty"},
	 {1, TYPE_INT16, "stat_1_present"},
	 {1, TYPE_INT16, "stat_2_present"},
	 {1, TYPE_INT16, "stat_3_present"},
	 {1, TYPE_INT16, "stat_4_present"},
	 {1, TYPE_INT16, "stat_5_present"},
	 {1, TYPE_INT16, "stat_6_present"},
	 {1, TYPE_INT16, "stat_7_present"},
	 {1, TYPE_INT16, "stat_8_present"},
	 {1, TYPE_FLOAT32, "stat_flag_1"},
	 {1, TYPE_FLOAT32, "stat_flag_2"},
	 {1, TYPE_FLOAT32, "stat_flag_3"},
	 {1, TYPE_FLOAT32, "stat_flag_4"},
	 {1, TYPE_FLOAT32, "stat_flag_5"},
	 {1, TYPE_FLOAT32, "stat_flag_6"},
	 {1, TYPE_FLOAT32, "stat_flag_7"},
	 {1, TYPE_FLOAT32, "stat_flag_8"},
	 {1, TYPE_FLOAT32, "stat_1"},
	 {1, TYPE_FLOAT32, "stat_2"},
	 {1, TYPE_FLOAT32, "stat_3"},
	 {1, TYPE_FLOAT32, "stat_4"},
	 {1, TYPE_FLOAT32, "stat_5"},
	 {1, TYPE_FLOAT32, "stat_6"},
	 {1, TYPE_FLOAT32, "stat_7"},
	 {1, TYPE_FLOAT32, "stat_8"},
	 {1, TYPE_INT32, "effect_1"},
	 {1, TYPE_INT32, "effect_2"},
	 {1, TYPE_INT8, "unknownValue0"},
	 {1, TYPE_INT32, "unknownValue1"},
	 {1, TYPE_INT32, "unknownValue2"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}
/*
#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Write && dst == DF_RDB) {
		*static_cast<short*>(row->getValuePtr("unknown_value")) = 162;
	}
}*/

#ifdef __cplusplus
}
#endif

