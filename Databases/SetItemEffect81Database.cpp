#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Fixme: not sure for the order

static FieldDescriptor df[] = {
    {1, TYPE_INT32, "set_id"},        {1, TYPE_INT32, "set_part_id"},   {1, TYPE_INT32, "text_id"},
    {1, TYPE_INT32, "tooltip_id"},    {1, TYPE_INT16, "base_type_0"},   {1, TYPE_INT16, "base_type_1"},
    {1, TYPE_INT16, "base_type_2"},   {1, TYPE_INT16, "base_type_3"},   {1, TYPE_FLOAT64, "base_var1_0"},
    {1, TYPE_FLOAT64, "base_var1_1"}, {1, TYPE_FLOAT64, "base_var1_2"}, {1, TYPE_FLOAT64, "base_var1_3"},
    {1, TYPE_FLOAT64, "base_var2_0"}, {1, TYPE_FLOAT64, "base_var2_1"}, {1, TYPE_FLOAT64, "base_var2_2"},
    {1, TYPE_FLOAT64, "base_var2_3"}, {1, TYPE_INT16, "opt_type_0"},    {1, TYPE_INT16, "opt_type_1"},
    {1, TYPE_INT16, "opt_type_2"},    {1, TYPE_INT16, "opt_type_3"},    {1, TYPE_FLOAT64, "opt_var1_0"},
    {1, TYPE_FLOAT64, "opt_var1_1"},  {1, TYPE_FLOAT64, "opt_var1_2"},  {1, TYPE_FLOAT64, "opt_var1_3"},
    {1, TYPE_FLOAT64, "opt_var2_0"},  {1, TYPE_FLOAT64, "opt_var2_1"},  {1, TYPE_FLOAT64, "opt_var2_2"},
    {1, TYPE_FLOAT64, "opt_var2_3"},  {1, TYPE_INT32, "effect_id"}};

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
#endif
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor** dfmPtr, int* sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return "set_id\0"
	       "set_part_id\0"
	       "text_id\0"
	       "tooltip_id\0"
	       "base_type_0\0"
	       "base_var1_0\0"
	       "base_var2_0\0"
	       "base_type_1\0"
	       "base_var1_1\0"
	       "base_var2_1\0"
	       "base_type_2\0"
	       "base_var1_2\0"
	       "base_var2_2\0"
	       "base_type_3\0"
	       "base_var1_3\0"
	       "base_var2_3\0"
	       "opt_type_0\0"
	       "opt_var1_0\0"
	       "opt_var2_0\0"
	       "opt_type_1\0"
	       "opt_var1_1\0"
	       "opt_var2_1\0"
	       "opt_type_2\0"
	       "opt_var1_2\0"
	       "opt_var2_2\0"
	       "opt_type_3\0"
	       "opt_var1_3\0"
	       "opt_var2_3\0"
	       "effect_id\0";
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:getDefaultFileName=_getDefaultFileName@0")
#endif
EDATABASEDLL const char* DLLCALLCONV getDefaultFileName() {
	return "db_setitemeffectresource";
}

#ifdef __cplusplus
}
#endif
