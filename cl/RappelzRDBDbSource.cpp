#ifdef __unix
#include <dlfcn.h>
#define DLOPEN(x) dlopen(x, RTLD_NOW)
#define DLGET(x, name) dlsym(x, name)
#define DLTYPE void*
#define DLCLOSE(x) dlclose(x)
#define DLERROR() dlerror()
#else
#include <windows.h>
#define DLOPEN(x) LoadLibrary(x)
#define DLGET(x, name) GetProcAddress(x, name)
#define DLTYPE HMODULE
#define DLCLOSE(x) FreeLibrary(x)
#define DLERROR() ""
#endif

#include <stdio.h>
#include <string.h>
#include "ExportDLL.h"
#include "DataType.h"

#define DECL_CASE(name) case name: strcat(out, #name); break
#define DECL_CASE_ADD(name) case name: strcat(out, " | " #name); break

typedef void (DLLCALLCONV *DLLREGISTERSTRUCTPROC)(FieldDescriptor **dfm, int *size);

bool getSizeString(int size, char* out) {
	int len, index;

	len = GET_VAR_LNGTH(size);
	index = GET_VAR_INDEX(size);

	if(index)
		sprintf(out, "MAKEINDEXEDVAR(%d, %d)", index, len);
	else
		sprintf(out, "%d", len);

	return true;
}

bool getTypeString(int in, char* out) {
	int type, ignoreType, flags;

	type = GET_TYPE(in);
	ignoreType = GET_IGNORETYPE(in);
	flags = GET_FLAGS(in);

	out[0] = 0;

	switch(type) {
		DECL_CASE(TYPE_BIT);
		DECL_CASE(TYPE_INT8);
		DECL_CASE(TYPE_INT16);
		DECL_CASE(TYPE_INT32);
		DECL_CASE(TYPE_INT64);
		DECL_CASE(TYPE_FLOAT32);
		DECL_CASE(TYPE_FLOAT64);
		DECL_CASE(TYPE_CHAR);
		DECL_CASE(TYPE_VARCHAR_SIZE);
		DECL_CASE(TYPE_VARCHAR_STR);
		DECL_CASE(TYPE_DECIMAL);
		DECL_CASE(TYPE_NVARCHAR_STR);

		default: sprintf(out, "%d", type);
	}

	switch(ignoreType) {
		DECL_CASE_ADD(TYPE_RDBIGNORE);
		DECL_CASE_ADD(TYPE_SQLIGNORE);
		DECL_CASE_ADD(TYPE_CSVIGNORE);
		DECL_CASE_ADD(TYPE_GUIIGNORE);
		case 0: break;

		default: sprintf(out, "%s | %d", out, ignoreType);
	}

	switch(flags) {
		DECL_CASE_ADD(TYPE_FLAG_KEY);
		DECL_CASE_ADD(TYPE_FLAG_SORT);
		case 0: break;

		default: sprintf(out, "%s | 0x%04X", out, flags);
	}

	return true;
}

int main(int argc, char** argv) {
	DLTYPE dll;
	DLLREGISTERSTRUCTPROC registerDBStructurePtr;
	FieldDescriptor* dfm;
	int dfmSize;

	char sizeStr[100];
	char typeStr[100];

	if(argc < 2) {
		printf("Usage: %s <dll>\n", argv[0]);
		return 1;
	}

	dll = DLOPEN(argv[1]);
	if(dll == NULL) {
        printf("Can't load dll %s: %s\n", argv[1], DLERROR());
		return 2;
	}

	registerDBStructurePtr = (DLLREGISTERSTRUCTPROC) DLGET(dll, "registerDBStructure");
	if(registerDBStructurePtr == NULL) {
        printf("Can't get registerDBStructure function: %s\n", DLERROR());
        DLCLOSE(dll);
		return 3;
	}

	(*registerDBStructurePtr)(&dfm, &dfmSize);

	for(int i = 0; i < dfmSize; i++) {
		getSizeString(dfm[i].dataSize, sizeStr);
		getTypeString(dfm[i].dataType, typeStr);

		printf("{%s, %s, \"%s\"},\n", sizeStr, typeStr, dfm[i].columnName);
	}

	DLCLOSE(dll);

	return 0;
}
