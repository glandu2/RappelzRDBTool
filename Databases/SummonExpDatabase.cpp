#include "../Base/IRowManipulator.h"
#include "../Base/DataType.h"
#include "../Base/ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32 | TYPE_RDBIGNORE, "level"},
	 {1, TYPE_INT64, "normal_exp"},
	 {1, TYPE_INT64, "growth_exp"},
	 {1, TYPE_INT64, "evolve_exp"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	static int currentRow = 1;
	if(mode == DCT_Read && dst == DF_RDB) {
		*static_cast<int*>(row->getValuePtr("level")) = currentRow;
		currentRow++;
	}
}

#ifdef __cplusplus
}
#endif

