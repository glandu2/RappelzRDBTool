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
	 {1, TYPE_INT32, "local_flag"},
	 {1, TYPE_INT32, "hunting_period"},
	 {1, TYPE_INT32, "objective_point"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "max_point"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "lobby_channel_id"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "lobby_x"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "lobby_y"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "dungeon_channel_id"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "dungeon_x"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "dungeon_y"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

