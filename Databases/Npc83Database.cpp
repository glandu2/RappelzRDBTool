#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "id"},
                               {1, TYPE_INT32, "text_id"},
                               {1, TYPE_INT32, "name_text_id"},
                               {1, TYPE_INT32, "race_id"},
                               {1, TYPE_INT32, "sexsual_id"},
                               {1, TYPE_INT32, "x"},
                               {1, TYPE_INT32, "y"},
                               {1, TYPE_INT32, "z"},
                               {1, TYPE_INT32, "face"},
                               {1, TYPE_INT32, "local_flag"},
                               {1, TYPE_INT32, "face_x"},
                               {1, TYPE_INT32, "face_y"},
                               {1, TYPE_INT32, "face_z"},
                               {256, TYPE_CHAR, "model_file"},
                               {1, TYPE_INT32, "hair_id"},
                               {1, TYPE_INT32, "face_id"},
                               {1, TYPE_INT32, "body_id"},
                               {1, TYPE_INT32, "weapon_item_id"},
                               {1, TYPE_INT32, "shield_item_id"},
                               {1, TYPE_INT32, "clothes_item_id"},
                               {1, TYPE_INT32, "helm_item_id"},
                               {1, TYPE_INT32, "gloves_item_id"},
                               {1, TYPE_INT32, "boots_item_id"},
                               {1, TYPE_INT32, "belt_item_id"},
                               {1, TYPE_INT32, "mantle_item_id"},
                               {1, TYPE_INT32, "necklace_item_id"},
                               {1, TYPE_INT32, "earring_item_id"},
                               {1, TYPE_INT32, "ring1_item_id"},
                               {1, TYPE_INT32, "ring2_item_id"},
                               {1, TYPE_INT32, "motion_id"},
                               {1, TYPE_INT32, "is_roam"},
                               {1, TYPE_INT32, "roaming_id"},
                               {1, TYPE_INT32, "standard_walk_speed"},
                               {1, TYPE_INT32, "standard_run_speed"},
                               {1, TYPE_INT32, "walk_speed"},
                               {1, TYPE_INT32, "run_speed"},
                               {1, TYPE_INT32, "attackable"},
                               {1, TYPE_INT32, "offensive_type"},
                               {1, TYPE_INT32, "spawn_type"},
                               {1, TYPE_INT32, "chase_range"},
                               {1, TYPE_INT32, "regen_time"},
                               {1, TYPE_INT32, "level"},
                               {1, TYPE_INT32, "stat_id"},
                               {1, TYPE_INT32, "attack_range"},
                               {1, TYPE_INT32, "attack_speed_type"},
                               {1, TYPE_INT32, "hp"},
                               {1, TYPE_INT32, "mp"},
                               {1, TYPE_INT32, "attack_point"},
                               {1, TYPE_INT32, "magic_point"},
                               {1, TYPE_INT32, "defence"},
                               {1, TYPE_INT32, "magic_defence"},
                               {1, TYPE_INT32, "attack_speed"},
                               {1, TYPE_INT32, "magic_speed"},
                               {1, TYPE_INT32, "accuracy"},
                               {1, TYPE_INT32, "avoid"},
                               {1, TYPE_INT32, "magic_accuracy"},
                               {1, TYPE_INT32, "magic_avoid"},
                               {256, TYPE_CHAR, "ai_script"},
                               {256, TYPE_CHAR, "contact_script"},
                               {1, TYPE_INT32, "texture_group"},
                               {1, TYPE_INT8, "type"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getDefaultFileName() {
	return "db_npcresource";
}

#ifdef __cplusplus
}
#endif
