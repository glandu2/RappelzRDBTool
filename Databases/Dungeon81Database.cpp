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
	 {1, TYPE_INT32, "local_flag"},
	 {1, TYPE_INT32 | TYPE_SQLIGNORE, "unknown1"},
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
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "num_party_raid"}};

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
	if(dst == DF_SQL && mode == DCT_Read) {
		row->setDataInt32("unknown1", row->getDataInt32("connector_id") == 0);
	} else if(dst == DF_RDB && mode == DCT_Read) {
		row->setDataInt32("num_party_raid", row->getDataInt32("num_party_guild") / 2);
	}
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_dungeonresource";
}

#ifdef __cplusplus
}
#endif

