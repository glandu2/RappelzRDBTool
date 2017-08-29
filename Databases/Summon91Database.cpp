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
	 {1, TYPE_INT32, "model_id"},
	 {1, TYPE_INT32, "name_id"},
	 {1, TYPE_INT32, "type"},
	 {1, TYPE_INT32, "affiliation" },
	 {1, TYPE_INT32, "species_id" },
	 {1, TYPE_INT32, "color" },
	 {1, TYPE_INT32, "magic_type"},
	 {1, TYPE_INT8, "rate"},
	 {1, TYPE_INT32, "stat_id"},
	 {1, TYPE_FLOAT32, "size"},
	 {1, TYPE_FLOAT32, "scale"},
	 {1, TYPE_FLOAT32, "target_fx_size"},
	 {1, TYPE_INT32, "standard_walk_speed"},
	 {1, TYPE_INT32, "standard_run_speed"},
	 {1, TYPE_INT32, "riding_speed"},
	 {1, TYPE_INT32, "run_speed"},
	 {1, TYPE_INT8, "is_riding_only"},
	 {1, TYPE_INT32, "riding_motion_type"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "attack_range"},
	 {1, TYPE_INT32, "walk_type"},
	 {1, TYPE_INT32, "slant_type"},
	 {1, TYPE_INT32, "material"},
	 {1, TYPE_INT32, "weapon_type"},
	 {1, TYPE_INT32, "attack_motion_speed"},
	 {1, TYPE_INT32, "form"},
	 {1, TYPE_INT32, "evolve_target"},
	 {1, TYPE_INT32, "camera_x"},
	 {1, TYPE_INT32, "camera_y"},
	 {1, TYPE_INT32, "camera_z"},
	 {1, TYPE_FLOAT32, "target_x"},
	 {1, TYPE_FLOAT32, "target_y"},
	 {1, TYPE_FLOAT32, "target_z"},
	 {256, TYPE_CHAR, "model"},
	 {1, TYPE_INT32, "motion_file_id"},
	 {1, TYPE_INT32, "face_id"},
	 {256, TYPE_CHAR, "face_file_name"},
	 {1, TYPE_INT32, "card_id"},
	 {256, TYPE_CHAR | TYPE_RDBIGNORE, "script_text"},
	 {256, TYPE_CHAR, "illust_file_name"},
	 {1, TYPE_INT32, "text_feature_id"},
	 {1, TYPE_INT32, "text_name_id"},
	 {1, TYPE_INT32, "skill_tree1_id"},
	 {1, TYPE_INT32, "skill_tree2_id"},
	 {1, TYPE_INT32, "skill_tree3_id"},
	 {1, TYPE_INT32, "skill_tree4_id"},
	 {1, TYPE_INT32, "skill_tree5_id"},
	 {1, TYPE_INT32, "texture_group"},
	 {1, TYPE_INT32, "local_flag"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
EDATABASEDLL void DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
#endif
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst == DF_RDB) {
		int id = row->getDataInt32("id");
		if(id == 540017 || id == 540021)
			row->setDataString("script_text", "creature_learn_riding()");
		else
			row->setDataString("script_text", "0");
	}
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_creature";
}

#ifdef __cplusplus
}
#endif

