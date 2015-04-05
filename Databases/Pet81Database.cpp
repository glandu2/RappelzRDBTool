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
	 {1, TYPE_INT32, "type"},
	 {1, TYPE_INT32, "name_id"},
	 {1, TYPE_INT32, "cage_id"},
	 {1, TYPE_INT32 | TYPE_SQLIGNORE, "rate"},
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

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst == DF_SQL) {
		int id = row->getDataInt32("id");
		if(id >= 500 && id < 600)
			row->setDataInt32("rate", 21);
		else
			row->setDataInt32("rate", 5);
	}
}

#ifdef __cplusplus
}
#endif

