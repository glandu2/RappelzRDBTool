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
	 {1, TYPE_INT32, "sub01_type_02"},
	 {1, TYPE_INT32, "sub01_type_03"},
	 {1, TYPE_INT32, "sub01_type_04"},
	 {1, TYPE_INT32, "sub01_type_05"},
	 {1, TYPE_INT32, "sub01_value_01"},
	 {1, TYPE_INT32, "sub01_value_02"},
	 {1, TYPE_INT32, "sub01_value_03"},
	 {1, TYPE_INT32, "sub01_value_04"},
	 {1, TYPE_INT32, "sub01_value_05"},
	 {1, TYPE_INT32, "sub02_type_01"},
	 {1, TYPE_INT32, "sub02_type_02"},
	 {1, TYPE_INT32, "sub02_type_03"},
	 {1, TYPE_INT32, "sub02_type_04"},
	 {1, TYPE_INT32, "sub02_value_01"},
	 {1, TYPE_INT32, "sub02_value_02"},
	 {1, TYPE_INT32, "sub02_value_03"},
	 {1, TYPE_INT32, "sub02_value_04"},
	 {1, TYPE_INT32, "sub02_value_05"},
	 {1, TYPE_INT32, "sub03_type_01"},
	 {1, TYPE_INT32, "sub03_type_02"},
	 {1, TYPE_INT32, "sub03_type_03"},
	 {1, TYPE_INT32, "sub03_type_04"},
	 {1, TYPE_INT32, "sub03_type_05"},
	 {1, TYPE_INT32, "sub03_value_01"},
	 {1, TYPE_INT32, "sub03_value_02"},
	 {1, TYPE_INT32, "sub03_value_03"},
	 {1, TYPE_INT32, "sub03_value_04"},
	 {1, TYPE_INT32, "sub03_value_05"},
	 {1, TYPE_INT32, "sub04_type_01"},
	 {1, TYPE_INT32, "sub04_type_02"},
	 {1, TYPE_INT32, "sub04_type_03"},
	 {1, TYPE_INT32, "sub04_type_04"},
	 {1, TYPE_INT32, "sub04_type_05"},
	 {1, TYPE_INT32, "sub04_value_01"},
	 {1, TYPE_INT32, "sub04_value_02"},
	 {1, TYPE_INT32, "sub04_value_03"},
	 {1, TYPE_INT32, "sub04_value_04"},
	 {1, TYPE_INT32, "sub04_value_05"},
	 {1, TYPE_INT32, "sub05_type_01"},
	 {1, TYPE_INT32, "sub05_type_02"},
	 {1, TYPE_INT32, "sub05_type_03"},
	 {1, TYPE_INT32, "sub05_type_04"},
	 {1, TYPE_INT32, "sub05_type_05"},
	 {1, TYPE_INT32, "sub05_value_01"},
	 {1, TYPE_INT32, "sub05_value_02"},
	 {1, TYPE_INT32, "sub05_value_03"},
	 {1, TYPE_INT32, "sub05_value_04"},
	 {1, TYPE_INT32, "sub05_value_05"},
	 {1, TYPE_INT32, "sub06_type_01"},
	 {1, TYPE_INT32, "sub06_type_02"},
	 {1, TYPE_INT32, "sub06_type_03"},
	 {1, TYPE_INT32, "sub06_type_04"},
	 {1, TYPE_INT32, "sub06_type_05"},
	 {1, TYPE_INT32, "sub06_value_01"},
	 {1, TYPE_INT32, "sub06_value_02"},
	 {1, TYPE_INT32, "sub06_value_03"},
	 {1, TYPE_INT32, "sub06_value_04"},
	 {1, TYPE_INT32, "sub06_value_05"},
	 {1, TYPE_INT32, "sub07_type_01"},
	 {1, TYPE_INT32, "sub07_type_02"},
	 {1, TYPE_INT32, "sub07_type_03"},
	 {1, TYPE_INT32, "sub07_type_04"},
	 {1, TYPE_INT32, "sub07_type_05"},
	 {1, TYPE_INT32, "sub07_value_01"},
	 {1, TYPE_INT32, "sub07_value_02"},
	 {1, TYPE_INT32, "sub07_value_03"},
	 {1, TYPE_INT32, "sub07_value_04"},
	 {1, TYPE_INT32, "sub07_value_05"},
	 {1, TYPE_INT32, "sub08_type_01"},
	 {1, TYPE_INT32, "sub08_type_02"},
	 {1, TYPE_INT32, "sub08_type_03"},
	 {1, TYPE_INT32, "sub08_type_04"},
	 {1, TYPE_INT32, "sub08_type_05"},
	 {1, TYPE_INT32, "sub08_value_01"},
	 {1, TYPE_INT32, "sub08_value_02"},
	 {1, TYPE_INT32, "sub08_value_03"},
	 {1, TYPE_INT32, "sub08_value_04"},
	 {1, TYPE_INT32, "sub08_value_05"},
	 {1, TYPE_INT32, "sub09_type_01"},
	 {1, TYPE_INT32, "sub09_type_02"},
	 {1, TYPE_INT32, "sub09_type_03"},
	 {1, TYPE_INT32, "sub09_type_04"},
	 {1, TYPE_INT32, "sub09_type_05"},
	 {1, TYPE_INT32, "sub09_value_01"},
	 {1, TYPE_INT32, "sub09_value_02"},
	 {1, TYPE_INT32, "sub09_value_03"},
	 {1, TYPE_INT32, "sub09_value_04"},
	 {1, TYPE_INT32, "sub09_value_05"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return "id\0"
		   "mix_type\0"
		   "mix_value_01\0"
		   "mix_value_02\0"
		   "mix_value_03\0"
		   "mix_value_04\0"
		   "mix_value_05\0"
		   "mix_value_06\0"
		   "sub_material_count\0"
		   "main_type_01\0"
		   "main_value_01\0"
		   "main_type_02\0"
		   "main_value_02\0"
		   "main_type_03\0"
		   "main_value_03\0"
		   "main_type_04\0"
		   "main_value_04\0"
		   "main_type_05\0"
		   "main_value_05\0"
		   "sub01_type_01\0"
		   "sub01_value_01\0"
		   "sub01_type_02\0"
		   "sub01_value_02\0"
		   "sub01_type_03\0"
		   "sub01_value_03\0"
		   "sub01_type_04\0"
		   "sub01_value_04\0"
		   "sub01_type_05\0"
		   "sub01_value_05\0"
		   "sub02_type_01\0"
		   "sub02_value_01\0"
		   "sub02_type_02\0"
		   "sub02_value_02\0"
		   "sub02_type_03\0"
		   "sub02_value_03\0"
		   "sub02_type_04\0"
		   "sub02_value_04\0"
		   "sub02_type_05\0"
		   "sub02_value_05\0"
		   "sub03_type_01\0"
		   "sub03_value_01\0"
		   "sub03_type_02\0"
		   "sub03_value_02\0"
		   "sub03_type_03\0"
		   "sub03_value_03\0"
		   "sub03_type_04\0"
		   "sub03_value_04\0"
		   "sub03_type_05\0"
		   "sub03_value_05\0"
		   "sub04_type_01\0"
		   "sub04_value_01\0"
		   "sub04_type_02\0"
		   "sub04_value_02\0"
		   "sub04_type_03\0"
		   "sub04_value_03\0"
		   "sub04_type_04\0"
		   "sub04_value_04\0"
		   "sub04_type_05\0"
		   "sub04_value_05\0"
		   "sub05_type_01\0"
		   "sub05_value_01\0"
		   "sub05_type_02\0"
		   "sub05_value_02\0"
		   "sub05_type_03\0"
		   "sub05_value_03\0"
		   "sub05_type_04\0"
		   "sub05_value_04\0"
		   "sub05_type_05\0"
		   "sub05_value_05\0"
		   "sub06_type_01\0"
		   "sub06_value_01\0"
		   "sub06_type_02\0"
		   "sub06_value_02\0"
		   "sub06_type_03\0"
		   "sub06_value_03\0"
		   "sub06_type_04\0"
		   "sub06_value_04\0"
		   "sub06_type_05\0"
		   "sub06_value_05\0"
		   "sub07_type_01\0"
		   "sub07_value_01\0"
		   "sub07_type_02\0"
		   "sub07_value_02\0"
		   "sub07_type_03\0"
		   "sub07_value_03\0"
		   "sub07_type_04\0"
		   "sub07_value_04\0"
		   "sub07_type_05\0"
		   "sub07_value_05\0"
		   "sub08_type_01\0"
		   "sub08_value_01\0"
		   "sub08_type_02\0"
		   "sub08_value_02\0"
		   "sub08_type_03\0"
		   "sub08_value_03\0"
		   "sub08_type_04\0"
		   "sub08_value_04\0"
		   "sub08_type_05\0"
		   "sub08_value_05\0"
		   "sub09_type_01\0"
		   "sub09_value_01\0"
		   "sub09_type_02\0"
		   "sub09_value_02\0"
		   "sub09_type_03\0"
		   "sub09_value_03\0"
		   "sub09_type_04\0"
		   "sub09_value_04\0"
		   "sub09_type_05\0"
		   "sub09_value_05\0";
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char*  DLLCALLCONV getDefaultFileName() {
	return "db_combineres";
}

#ifdef __cplusplus
}
#endif

