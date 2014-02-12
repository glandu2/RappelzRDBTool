#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "effect_file_ID"},
	 {1, TYPE_INT32, "effect_loop"},
	 {1, TYPE_INT32, "effect_position"},
	 {1, TYPE_INT32, "swap_event_id"},
	 {1, TYPE_INT32, "swap_model_ID"},
	 {256, TYPE_CHAR, "bone_name_01"},
	 {256, TYPE_CHAR, "bone_name_02"},
	 {256, TYPE_CHAR, "bone_name_03"},
	 {256, TYPE_CHAR, "bone_name_04"},
	 {256, TYPE_CHAR, "bone_name_05"},
	 {256, TYPE_CHAR, "bone_name_06"},
	 {256, TYPE_CHAR, "bone_name_07"},
	 {256, TYPE_CHAR, "bone_name_08"},
	 {256, TYPE_CHAR, "bone_name_09"},
	 {256, TYPE_CHAR, "bone_name_10"},
	 {256, TYPE_CHAR, "bone_name_11"},
	 {256, TYPE_CHAR, "bone_name_12"},
	 {256, TYPE_CHAR, "bone_name_13"},
	 {256, TYPE_CHAR, "bone_name_14"},
	 {256, TYPE_CHAR, "bone_name_15"},
	 {1, TYPE_INT32, "bone_effect_01"},
	 {1, TYPE_INT32, "bone_effect_02"},
	 {1, TYPE_INT32, "bone_effect_03"},
	 {1, TYPE_INT32, "bone_effect_04"},
	 {1, TYPE_INT32, "bone_effect_05"},
	 {1, TYPE_INT32, "bone_effect_06"},
	 {1, TYPE_INT32, "bone_effect_07"},
	 {1, TYPE_INT32, "bone_effect_08"},
	 {1, TYPE_INT32, "bone_effect_09"},
	 {1, TYPE_INT32, "bone_effect_10"},
	 {1, TYPE_INT32, "bone_effect_11"},
	 {1, TYPE_INT32, "bone_effect_12"},
	 {1, TYPE_INT32, "bone_effect_13"},
	 {1, TYPE_INT32, "bone_effect_14"},
	 {1, TYPE_INT32, "bone_effect_15"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "id\0"
			"effect_file_ID\0"
			"effect_loop\0"
			"effect_position\0"
			"swap_event_id\0"
			"swap_model_ID\0"
			"bone_name_01\0"
			"bone_effect_01\0"
			"bone_name_02\0"
			"bone_effect_02\0"
			"bone_name_03\0"
			"bone_effect_03\0"
			"bone_name_04\0"
			"bone_effect_04\0"
			"bone_name_05\0"
			"bone_effect_05\0"
			"bone_name_06\0"
			"bone_effect_06\0"
			"bone_name_07\0"
			"bone_effect_07\0"
			"bone_name_08\0"
			"bone_effect_08\0"
			"bone_name_09\0"
			"bone_effect_09\0"
			"bone_name_10\0"
			"bone_effect_10\0"
			"bone_name_11\0"
			"bone_effect_11\0"
			"bone_name_12\0"
			"bone_effect_12\0"
			"bone_name_13\0"
			"bone_effect_13\0"
			"bone_name_14\0"
			"bone_effect_14\0"
			"bone_name_15\0"
			"bone_effect_15\0";
}

#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_modeleffectresource";
}

#ifdef __cplusplus
}
#endif

