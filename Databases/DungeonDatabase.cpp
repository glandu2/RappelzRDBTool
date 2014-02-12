#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "local_flag"},
	 {1, TYPE_INT32, "dungeon_level"},
	 {1, TYPE_INT32, "raid_start_pos_x"},
	 {1, TYPE_INT32, "raid_start_pos_y"},
	 {1, TYPE_INT32, "defence_pos_x"},
	 {1, TYPE_INT32, "defence_pos_y"},
	 {1, TYPE_INT32, "connector_id"},
	 {1, TYPE_INT32, "connector_pos_x"},
	 {1, TYPE_INT32, "connector_pos_y"},
	 {1, TYPE_INT32, "connector_direction"},
	 {1, TYPE_INT32, "core_id"},
	 {1, TYPE_INT32, "core_pos_x"},
	 {1, TYPE_INT32, "core_pos_y"},
	 {1, TYPE_FLOAT32, "core_offset_x"},
	 {1, TYPE_FLOAT32, "core_offset_y"},
	 {1, TYPE_FLOAT32, "core_offset_z"},
	 {1, TYPE_FLOAT32, "core_around_x"},
	 {1, TYPE_FLOAT32, "core_around_y"},
	 {1, TYPE_FLOAT32, "core_around_z"},
	 {1, TYPE_FLOAT32, "scale_x"},
	 {1, TYPE_FLOAT32, "scale_y"},
	 {1, TYPE_FLOAT32, "scale_z"},
	 {1, TYPE_INT8, "is_height_locked"},
	 {1, TYPE_FLOAT32, "lock_height"},
	 {1, TYPE_INT32, "boss01_id"},
	 {1, TYPE_INT32, "boss02_id"},
	 {1, TYPE_INT32, "raid_opening_time"},
	 {1, TYPE_INT32, "raid_closing_time"},
	 {1, TYPE_INT32, "siege_opening_time"},
	 {1, TYPE_INT32, "siege_closing_time"},
	 {1, TYPE_INT32, "seamless_x"},
	 {1, TYPE_INT32, "seamless_y"},
	 {1, TYPE_INT32, "num_party_guild"},
	 {1, TYPE_INT32, "num_party_mercenary"},
	 {1, TYPE_INT32, "num_party_raid"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_dungeonresource";
}

#ifdef __cplusplus
}
#endif

