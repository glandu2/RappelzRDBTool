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
	 {1, TYPE_INT32, "fx_type"},
	 {1, TYPE_INT32, "add_time"},
	 {1, TYPE_INT32, "following"},
	 {1, TYPE_INT32, "graphic_effect_start_file_ID"},
	 {1, TYPE_INT32, "graphic_effect_middle_file_ID"},
	 {1, TYPE_INT32, "graphic_effect_end_file_ID"},
	 {1, TYPE_INT32, "graphic_effect_direction"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "is_effect0"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "is_effect1"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "is_effect2"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknown0"},
	 {1, TYPE_INT32, "sound_start_file_ID"},
	 {1, TYPE_INT32, "sound_middle_file_ID"},
	 {1, TYPE_INT32, "sound_end_file_ID"},
	 {1, TYPE_INT32, "sound_play_probability"},
	 {1, TYPE_INT32, "sound_volumn"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "is_sound0"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "is_sound1"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "is_sound2"},
	 {1, TYPE_INT8, "sound_reverb_apply"},
	 {1, TYPE_INT8, "sound_filter_apply"},
	 {1, TYPE_INT8, "sound_stereo_apply"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "sound_option0"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "sound_option1"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE, "sound_option2"},
	 {3, TYPE_CHAR | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "kagValue"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst != DF_RDB) {
		*static_cast<int*>(row->getValuePtr("unknown0")) = 0;
		strncpy(static_cast<char*>(row->getValuePtr("kagValue")), "kag", 3);
	}
}

#pragma comment(linker, "/EXPORT:getDefaultTableName=_getDefaultTableName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultTableName() {
	return "MonsterFxSet";
}

#ifdef __cplusplus
}
#endif

