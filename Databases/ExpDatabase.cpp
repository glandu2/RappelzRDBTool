#include "IRowManipulator.h"
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32 | TYPE_RDBIGNORE, "lvl"},
	 {1, TYPE_INT64, "normal_exp"},
	 {1, TYPE_INT32, "jl1"},
	 {1, TYPE_INT32, "jl2"},
	 {1, TYPE_INT32, "jl3"},
	 {1, TYPE_INT32, "jl4"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst == DF_RDB) {
		*static_cast<int*>(row->getValuePtr("lvl")) = rowNum + 1;
	}
}

#ifdef __cplusplus
}
#endif

