#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "mix_type"},
	 {1, TYPE_INT32, "mix_value_01"},
	 {1, TYPE_INT32, "mix_value_02"},
	 {1, TYPE_INT32, "mix_value_03"},
	 {1, TYPE_INT32, "mix_value_04"},
	 {1, TYPE_INT32, "mix_value_05"},
	 {1, TYPE_INT32, "mix_value_06"},
	 {1, TYPE_INT32, "sub_material_count"},
	 {1, TYPE_INT32, "main_type_01"},
	 {1, TYPE_INT32, "main_value_01"},
	 {1, TYPE_INT32, "main_type_02"},
	 {1, TYPE_INT32, "main_value_02"},
	 {1, TYPE_INT32, "main_type_03"},
	 {1, TYPE_INT32, "main_value_03"},
	 {1, TYPE_INT32, "main_type_04"},
	 {1, TYPE_INT32, "main_value_04"},
	 {1, TYPE_INT32, "main_type_05"},
	 {1, TYPE_INT32, "main_value_05"},
	 {1, TYPE_INT32, "sub01_type_01"},
	 {1, TYPE_INT32, "sub01_value_01"},
	 {1, TYPE_INT32, "sub01_type_02"},
	 {1, TYPE_INT32, "sub01_value_02"},
	 {1, TYPE_INT32, "sub01_type_03"},
	 {1, TYPE_INT32, "sub01_value_03"},
	 {1, TYPE_INT32, "sub01_type_04"},
	 {1, TYPE_INT32, "sub01_value_04"},
	 {1, TYPE_INT32, "sub01_type_05"},
	 {1, TYPE_INT32, "sub01_value_05"},
	 {1, TYPE_INT32, "sub02_type_01"},
	 {1, TYPE_INT32, "sub02_value_01"},
	 {1, TYPE_INT32, "sub02_type_02"},
	 {1, TYPE_INT32, "sub02_value_02"},
	 {1, TYPE_INT32, "sub02_type_03"},
	 {1, TYPE_INT32, "sub02_value_03"},
	 {1, TYPE_INT32, "sub02_type_04"},
	 {1, TYPE_INT32, "sub02_value_04"},
	 {1, TYPE_INT32, "sub02_type_05"},
	 {1, TYPE_INT32, "sub02_value_05"},
	 {1, TYPE_INT32, "sub03_type_01"},
	 {1, TYPE_INT32, "sub03_value_01"},
	 {1, TYPE_INT32, "sub03_type_02"},
	 {1, TYPE_INT32, "sub03_value_02"},
	 {1, TYPE_INT32, "sub03_type_03"},
	 {1, TYPE_INT32, "sub03_value_03"},
	 {1, TYPE_INT32, "sub03_type_04"},
	 {1, TYPE_INT32, "sub03_value_04"},
	 {1, TYPE_INT32, "sub03_type_05"},
	 {1, TYPE_INT32, "sub03_value_05"},
	 {1, TYPE_INT32, "sub04_type_01"},
	 {1, TYPE_INT32, "sub04_value_01"},
	 {1, TYPE_INT32, "sub04_type_02"},
	 {1, TYPE_INT32, "sub04_value_02"},
	 {1, TYPE_INT32, "sub04_type_03"},
	 {1, TYPE_INT32, "sub04_value_03"},
	 {1, TYPE_INT32, "sub04_type_04"},
	 {1, TYPE_INT32, "sub04_value_04"},
	 {1, TYPE_INT32, "sub04_type_05"},
	 {1, TYPE_INT32, "sub04_value_05"},
	 {1, TYPE_INT32, "sub05_type_01"},
	 {1, TYPE_INT32, "sub05_value_01"},
	 {1, TYPE_INT32, "sub05_type_02"},
	 {1, TYPE_INT32, "sub05_value_02"},
	 {1, TYPE_INT32, "sub05_type_03"},
	 {1, TYPE_INT32, "sub05_value_03"},
	 {1, TYPE_INT32, "sub05_type_04"},
	 {1, TYPE_INT32, "sub05_value_04"},
	 {1, TYPE_INT32, "sub05_type_05"},
	 {1, TYPE_INT32, "sub05_value_05"},
	 {1, TYPE_INT32, "sub06_type_01"},
	 {1, TYPE_INT32, "sub06_value_01"},
	 {1, TYPE_INT32, "sub06_type_02"},
	 {1, TYPE_INT32, "sub06_value_02"},
	 {1, TYPE_INT32, "sub06_type_03"},
	 {1, TYPE_INT32, "sub06_value_03"},
	 {1, TYPE_INT32, "sub06_type_04"},
	 {1, TYPE_INT32, "sub06_value_04"},
	 {1, TYPE_INT32, "sub06_type_05"},
	 {1, TYPE_INT32, "sub06_value_05"},
	 {1, TYPE_INT32, "sub07_type_01"},
	 {1, TYPE_INT32, "sub07_value_01"},
	 {1, TYPE_INT32, "sub07_type_02"},
	 {1, TYPE_INT32, "sub07_value_02"},
	 {1, TYPE_INT32, "sub07_type_03"},
	 {1, TYPE_INT32, "sub07_value_03"},
	 {1, TYPE_INT32, "sub07_type_04"},
	 {1, TYPE_INT32, "sub07_value_04"},
	 {1, TYPE_INT32, "sub07_type_05"},
	 {1, TYPE_INT32, "sub07_value_05"},
	 {1, TYPE_INT32, "sub08_type_01"},
	 {1, TYPE_INT32, "sub08_value_01"},
	 {1, TYPE_INT32, "sub08_type_02"},
	 {1, TYPE_INT32, "sub08_value_02"},
	 {1, TYPE_INT32, "sub08_type_03"},
	 {1, TYPE_INT32, "sub08_value_03"},
	 {1, TYPE_INT32, "sub08_type_04"},
	 {1, TYPE_INT32, "sub08_value_04"},
	 {1, TYPE_INT32, "sub08_type_05"},
	 {1, TYPE_INT32, "sub08_value_05"},
	 {1, TYPE_INT32, "sub09_type_01"},
	 {1, TYPE_INT32, "sub09_value_01"},
	 {1, TYPE_INT32, "sub09_type_02"},
	 {1, TYPE_INT32, "sub09_value_02"},
	 {1, TYPE_INT32, "sub09_type_03"},
	 {1, TYPE_INT32, "sub09_value_03"},
	 {1, TYPE_INT32, "sub09_type_04"},
	 {1, TYPE_INT32, "sub09_value_04"},
	 {1, TYPE_INT32, "sub09_type_05"},
	 {1, TYPE_INT32, "sub09_value_05"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_combineres";
}

#ifdef __cplusplus
}
#endif
