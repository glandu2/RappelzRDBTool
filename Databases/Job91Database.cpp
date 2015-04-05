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
	 {1, TYPE_INT32, "text_id"},
	 {1, TYPE_INT32, "stati_id"},
	 {1, TYPE_INT32, "skill_tree_id"},
	 {1, TYPE_INT8, "job_class"},
	 {1, TYPE_INT8, "job_depth"},
	 {1, TYPE_INT16, "up_lv"},
	 {1, TYPE_INT16, "up_jlv"},
	 {1, TYPE_INT16, "avable_job_0"},
	 {1, TYPE_INT16, "avable_job_1"},
	 {1, TYPE_INT16, "avable_job_2"},
	 {1, TYPE_INT16, "avable_job_3"},
	 {1, TYPE_INT16 | TYPE_SQLIGNORE, "unknown_value"},
	 {1, TYPE_INT32, "icon_id"},
	 {256, TYPE_CHAR, "icon_file_name"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst == DF_SQL) {
		row->setDataInt16("unknown_value", 163);
	}
}

#ifdef __cplusplus
}
#endif

