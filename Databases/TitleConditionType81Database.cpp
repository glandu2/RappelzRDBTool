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
	 {1, TYPE_INT32, "category"},
	 {1, TYPE_INT32, "value0"},
	 {1, TYPE_INT32, "value1"},
	 {1, TYPE_INT32, "value2"},
	 {1, TYPE_BIT | TYPE_RDBIGNORE, "is_set"},
	 {1, TYPE_BIT | TYPE_RDBIGNORE, "skip_db_update"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst == DF_RDB) {
		row->setDataBit("is_set", 0);
		row->setDataBit("skip_db_update", 0);
	}
}

#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_titleconditiontyperesource";
}

#ifdef __cplusplus
}
#endif

