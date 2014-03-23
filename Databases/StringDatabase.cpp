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

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getCSVColumnOrder=_getCSVColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getCSVColumnOrder() {
	return  "code\0"
			"group_id\0"
			"name\0"
			"value\0";
}

#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "name\0"
			"group_id\0"
			"code\0"
			"value\0";
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	static bool initialized = false;
	static int unknownValue0Idx;
	static int unknownValue1Idx;
	static int unknownValue2Idx;
	static int unknownValue3Idx;

	if(mode == DCT_Read && (dst == DF_SQL || dst == DF_CSV)) {
		if(initialized == false) {
			unknownValue0Idx = row->getColumnIndex("unknownValue0");
			unknownValue1Idx = row->getColumnIndex("unknownValue1");
			unknownValue2Idx = row->getColumnIndex("unknownValue2");
			unknownValue3Idx = row->getColumnIndex("unknownValue3");
			initialized = true;
		}
		*static_cast<int*>(row->getValuePtr(unknownValue0Idx)) = 0;
		*static_cast<int*>(row->getValuePtr(unknownValue1Idx)) = 0;
		*static_cast<int*>(row->getValuePtr(unknownValue2Idx)) = 0;
		*static_cast<int*>(row->getValuePtr(unknownValue3Idx)) = 0;
	}
	
}

#ifdef __cplusplus
}
#endif

