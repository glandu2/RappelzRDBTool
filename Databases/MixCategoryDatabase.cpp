#include "../Base/DataType.h"
#include "../Base/ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT16, "id"},
	 {1, TYPE_INT8, "mix_id"},
	 {1, TYPE_INT32, "local_flag"},
	 {1, TYPE_INT16, "high_category_id"},
	 {1, TYPE_INT16, "middle_category_id"},
	 {1, TYPE_INT16, "low_category_id"},
	 {1, TYPE_INT32, "category_text_id"},
	 {1, TYPE_INT32, "formal_id"},
	 {1, TYPE_INT32, "result_id"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

