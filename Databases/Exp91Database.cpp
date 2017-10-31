#include "DataType.h"
#include "ExportDLL.h"
#include "IRowManipulator.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {{1, TYPE_INT32 | TYPE_RDBIGNORE | TYPE_FLAG_SORT, "level"},
                               {1, TYPE_INT64, "exp"},
                               {1, TYPE_INT64, "jp_0"},
                               {1, TYPE_INT64, "jp_1"},
                               {1, TYPE_INT64, "jp_2"},
                               {1, TYPE_INT64, "jp_3"}};

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
	if(mode == DCT_Read && dst == DF_RDB) {
		row->setDataInt32("level", rowNum + 1);
	}
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultTableName=_getDefaultTableName@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getDefaultTableName() {
	return "LevelResource";
}

#ifdef __cplusplus
}
#endif
