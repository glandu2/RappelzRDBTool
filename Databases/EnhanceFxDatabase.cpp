#include "../Base/DataType.h"
#include "../Base/ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "enhance_level"},
	 {1, TYPE_INT32, "enhance_color_r"},
	 {1, TYPE_INT32, "enhance_color_g"},
	 {1, TYPE_INT32, "enhance_color_b"},
	 {1, TYPE_INT32, "wt_length"},
	 {1, TYPE_INT32, "wt_width"},
	 {1, TYPE_INT32, "wt00_a"},
	 {1, TYPE_INT32, "wt00_r"},
	 {1, TYPE_INT32, "wt00_g"},
	 {1, TYPE_INT32, "wt00_b"},
	 {1, TYPE_INT32, "wt01_a"},
	 {1, TYPE_INT32, "wt01_r"},
	 {1, TYPE_INT32, "wt01_g"},
	 {1, TYPE_INT32, "wt01_b"},
	 {1, TYPE_INT32, "wt10_a"},
	 {1, TYPE_INT32, "wt10_r"},
	 {1, TYPE_INT32, "wt10_g"},
	 {1, TYPE_INT32, "wt10_b"},
	 {1, TYPE_INT32, "wt11_a"},
	 {1, TYPE_INT32, "wt11_r"},
	 {1, TYPE_INT32, "wt11_g"},
	 {1, TYPE_INT32, "wt11_b"},
	 {1, TYPE_INT32, "wt_texture_ID"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

