#include "DataType.h"
#include "ExportDLL.h"
#include "IRowManipulator.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32, "id"},
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
                               {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "unknown0"},
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

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
#endif
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst,
                                          eDataConvertionType mode,
                                          IRowManipulator* row,
                                          unsigned int rowNum) {
	if(mode == DCT_Read && dst != DF_RDB) {
		row->setDataInt8("unknown0", 1);
		row->setDataString("kagValue", "kag");

		if(dst == DF_SQL) {
			row->setDataInt8("is_effect0", row->getDataInt32("graphic_effect_start_file_ID") != 0);
			row->setDataInt8("is_effect1", row->getDataInt32("graphic_effect_middle_file_ID") != 0);
			row->setDataInt8("is_effect2", row->getDataInt32("graphic_effect_end_file_ID") != 0);

			row->setDataInt8("is_sound0", row->getDataInt32("sound_start_file_ID") != 0);
			row->setDataInt8("is_sound1", row->getDataInt32("sound_middle_file_ID") != 0);
			row->setDataInt8("is_sound2", row->getDataInt32("sound_end_file_ID") != 0);

			row->setDataInt8("sound_option0", 0);
			row->setDataInt8("sound_option1", 0);
			row->setDataInt8("sound_option2", 0);
		}
	}
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultTableName=_getDefaultTableName@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getDefaultTableName() {
	return "MotionFXSet";
}

#ifdef __cplusplus
}
#endif
