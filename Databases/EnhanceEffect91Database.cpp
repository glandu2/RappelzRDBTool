#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

// RDU format in 9.1 client

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "sid"},
	{1, TYPE_INT16, "sub_id"},
	{1, TYPE_INT16, "effect_id"},
	{1, TYPE_FLOAT64, "value_01"},
	{1, TYPE_FLOAT64, "value_02"},
	{1, TYPE_FLOAT64, "value_03"},
	{1, TYPE_FLOAT64, "value_04"},
	{1, TYPE_FLOAT64, "value_05"},
	{1, TYPE_FLOAT64, "value_06"},
	{1, TYPE_FLOAT64, "value_07"},
	{1, TYPE_FLOAT64, "value_08"},
	{1, TYPE_FLOAT64, "value_09"},
	{1, TYPE_FLOAT64, "value_10"},
	{1, TYPE_FLOAT64, "value_11"},
	{1, TYPE_FLOAT64, "value_12"},
	{1, TYPE_FLOAT64, "value_13"},
	{1, TYPE_FLOAT64, "value_14"},
	{1, TYPE_FLOAT64, "value_15"},
	{1, TYPE_FLOAT64, "value_16"},
	{1, TYPE_FLOAT64, "value_17"},
	{1, TYPE_FLOAT64, "value_18"},
	{1, TYPE_FLOAT64, "value_19"},
	{1, TYPE_FLOAT64, "value_20"},
	{1, TYPE_FLOAT64, "value_21"},
	{1, TYPE_FLOAT64, "value_22"},
	{1, TYPE_FLOAT64, "value_23"},
	{1, TYPE_FLOAT64, "value_24"},
	{1, TYPE_FLOAT64, "value_25"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

