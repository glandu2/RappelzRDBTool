#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {
    {1, TYPE_INT32, "enhance_id"},      {1, TYPE_INT8, "enhance_type"},     {1, TYPE_INT8, "fail_result"},
    {1, TYPE_INT8, "max_enhance"},      {1, TYPE_INT32, "local_flag"},      {1, TYPE_INT32, "need_item"},
    {1, TYPE_FLOAT32, "percentage_1"},  {1, TYPE_FLOAT32, "percentage_2"},  {1, TYPE_FLOAT32, "percentage_3"},
    {1, TYPE_FLOAT32, "percentage_4"},  {1, TYPE_FLOAT32, "percentage_5"},  {1, TYPE_FLOAT32, "percentage_6"},
    {1, TYPE_FLOAT32, "percentage_7"},  {1, TYPE_FLOAT32, "percentage_8"},  {1, TYPE_FLOAT32, "percentage_9"},
    {1, TYPE_FLOAT32, "percentage_10"}, {1, TYPE_FLOAT32, "percentage_11"}, {1, TYPE_FLOAT32, "percentage_12"},
    {1, TYPE_FLOAT32, "percentage_13"}, {1, TYPE_FLOAT32, "percentage_14"}, {1, TYPE_FLOAT32, "percentage_15"},
    {1, TYPE_FLOAT32, "percentage_16"}, {1, TYPE_FLOAT32, "percentage_17"}, {1, TYPE_FLOAT32, "percentage_18"},
    {1, TYPE_FLOAT32, "percentage_19"}, {1, TYPE_FLOAT32, "percentage_20"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif
