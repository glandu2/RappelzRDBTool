#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "id"},
                               {1, TYPE_INT32, "species_id"},
                               {1, TYPE_INT32, "type"},
                               {1, TYPE_INT32, "model_id"},
                               {1, TYPE_INT32, "name_id"},
                               {1, TYPE_INT32, "cage_id"},
                               {1, TYPE_INT32, "rate"},
                               {1, TYPE_FLOAT32, "size"},
                               {1, TYPE_FLOAT32, "scale"},
                               {1, TYPE_FLOAT32, "target_fx_size"},
                               {1, TYPE_INT32, "walk_type"},
                               {1, TYPE_INT32, "slant_type"},
                               {1, TYPE_INT32, "camera_x"},
                               {1, TYPE_INT32, "camera_y"},
                               {1, TYPE_INT32, "camera_z"},
                               {1, TYPE_FLOAT32, "target_x"},
                               {1, TYPE_FLOAT32, "target_y"},
                               {1, TYPE_FLOAT32, "target_z"},
                               {256, TYPE_CHAR, "model"},
                               {1, TYPE_INT32, "motion_file_id"},
                               {1, TYPE_INT32, "texture_group"},
                               {1, TYPE_INT32, "local_flag"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif
