#include "DataType.h"
#include "ExportDLL.h"
#include "IRowManipulator.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] = {
    {1, TYPE_INT32, "id"},           {1, TYPE_INT32, "name_id"},      {1, TYPE_INT32, "tooltip_id"},
    {1, TYPE_INT32, "status_id"},    {1, TYPE_INT8, "rate"},          {1, TYPE_INT16, "opt_type_0"},
    {1, TYPE_INT16, "opt_type_1"},   {1, TYPE_INT16, "opt_type_2"},   {1, TYPE_INT16, "opt_type_3"},
    {1, TYPE_INT16, "opt_type_4"},   {1, TYPE_INT16, "opt_type_5"},   {1, TYPE_INT16, "opt_type_6"},
    {1, TYPE_INT16, "opt_type_7"},   {1, TYPE_FLOAT32, "opt_var1_0"}, {1, TYPE_FLOAT32, "opt_var1_1"},
    {1, TYPE_FLOAT32, "opt_var1_2"}, {1, TYPE_FLOAT32, "opt_var1_3"}, {1, TYPE_FLOAT32, "opt_var1_4"},
    {1, TYPE_FLOAT32, "opt_var1_5"}, {1, TYPE_FLOAT32, "opt_var1_6"}, {1, TYPE_FLOAT32, "opt_var1_7"},
    {1, TYPE_FLOAT32, "opt_var2_0"}, {1, TYPE_FLOAT32, "opt_var2_1"}, {1, TYPE_FLOAT32, "opt_var2_2"},
    {1, TYPE_FLOAT32, "opt_var2_3"}, {1, TYPE_FLOAT32, "opt_var2_4"}, {1, TYPE_FLOAT32, "opt_var2_5"},
    {1, TYPE_FLOAT32, "opt_var2_6"}, {1, TYPE_FLOAT32, "opt_var2_7"}, {1, TYPE_INT32, "effect_id"},
    {1, TYPE_INT32, "local_flag"},   {1, TYPE_INT8, "is_periodic"},   {1, TYPE_INT32, "begin_of_period"},
    {1, TYPE_INT32, "end_of_period"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getDefaultFileName() {
	return "db_titleresource";
}

#ifdef __cplusplus
}
#endif
