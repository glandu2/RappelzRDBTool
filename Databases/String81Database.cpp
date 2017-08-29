#include "IRowManipulator.h"
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{MAKEINDEXEDVAR(0, 1), TYPE_VARCHAR_SIZE | TYPE_SQLIGNORE, ""},
	 {MAKEINDEXEDVAR(1, 1), TYPE_VARCHAR_SIZE | TYPE_SQLIGNORE, ""},
	 {MAKEINDEXEDVAR(0, 64), TYPE_VARCHAR_STR, "name"},
	 {MAKEINDEXEDVAR(1, 3999), TYPE_NVARCHAR_STR, "value"},
	 {1, TYPE_INT32, "code"},
	 {1, TYPE_INT32, "group_id"},
	 {1, TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "unknownValue0"},
	 {1, TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "unknownValue1"},
	 {1, TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "unknownValue2"},
	 {1, TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "unknownValue3"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getCSVColumnOrder=_getCSVColumnOrder@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getCSVColumnOrder() {
	return  "code\0"
			"group_id\0"
			"name\0"
			"value\0";
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "name\0"
			"group_id\0"
			"code\0"
			"value\0";
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
#endif
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst != DF_RDB) {
		row->setDataInt32("unknownValue0", 0);
		row->setDataInt32("unknownValue1", 0);
		row->setDataInt32("unknownValue2", 0);
		row->setDataInt32("unknownValue3", 0);
	}
	
}

#ifdef __cplusplus
}
#endif

