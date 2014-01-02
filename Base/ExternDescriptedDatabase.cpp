#include "ExternDescriptedDatabase.h"
#include "SpecialDatabaseRules.h"

#ifdef __linux__
#  include <dlfcn.h>
#else
#  include <windows.h>
#endif

#include <stdio.h>

namespace RappelzRDBBase {

ExternDescriptedDatabase::ExternDescriptedDatabase(const char* databaseName)
	: filename(databaseName)
{

#ifdef __unix__
	libHinst = (unsigned long long)dlopen(databaseName, RTLD_NOW);
	if(!libHinst) {
		char errorBuffer[256];
		sprintf(errorBuffer, "Unable to load Database description DLL, last error: %s", dlerror());
		puts(errorBuffer);
		throw errorBuffer;
	}
#else
	libHinst = (unsigned long long)LoadLibrary(databaseName);
	if(!libHinst) {
		char errorBuffer[256];
		sprintf(errorBuffer, "Unable to load Database description DLL, last error: 0x%08x", GetLastError());
		puts(errorBuffer);
		throw errorBuffer;
	}
#endif


#ifdef __unix__
	DLLregisterDBStructure = (DLLREGISTERSTRUCTPROC) dlsym((void*)libHinst, "registerDBStructure");
	DLLgetSQLColumnOrder = (DLLSQLORDERPROC) dlsym((void*)libHinst, "getSQLColumnOrder");
	DLLgetCSVColumnOrder = (DLLCSVORDERPROC) dlsym((void*)libHinst, "getCSVColumnOrder");
	DLLconvertData = (DLLCONVERTDATAPROC) dlsym((void*)libHinst, "convertData");
	DLLgetSpecialCaseID = (DLLISSPECIALPROC) dlsym((void*)libHinst, "getSpecialCaseID");
#else
	DLLregisterDBStructure = (DLLREGISTERSTRUCTPROC) GetProcAddress((HINSTANCE)libHinst, "registerDBStructure");
	DLLgetSQLColumnOrder = (DLLSQLORDERPROC) GetProcAddress((HINSTANCE)libHinst, "getSQLColumnOrder");
	DLLgetCSVColumnOrder = (DLLCSVORDERPROC) GetProcAddress((HINSTANCE)libHinst, "getCSVColumnOrder");
	DLLconvertData = (DLLCONVERTDATAPROC) GetProcAddress((HINSTANCE)libHinst, "convertData");
	DLLgetSpecialCaseID = (DLLISSPECIALPROC) GetProcAddress((HINSTANCE)libHinst, "getSpecialCaseID");
#endif

	if(DLLregisterDBStructure == 0) {
#ifdef __unix__
		dlclose((void*)libHinst);
#else
		FreeLibrary((HINSTANCE)libHinst);
#endif
		puts("Not a database description DLL");
		throw "Not a Database Description DLL";
	}
}

ExternDescriptedDatabase::~ExternDescriptedDatabase() {
#ifdef __unix__
	dlclose((void*)libHinst);
#else
	FreeLibrary((HINSTANCE)libHinst);
#endif
}

void ExternDescriptedDatabase::registerDBStructure(FieldDescriptor **dfm, int *size) {
	DLLregisterDBStructure(dfm, size);
}

const char* ExternDescriptedDatabase::getSQLColumnOrder() {
	if(DLLgetSQLColumnOrder) return DLLgetSQLColumnOrder();
	else return NULL;
}

const char* ExternDescriptedDatabase::getCSVColumnOrder() {
	if(DLLgetCSVColumnOrder) return DLLgetCSVColumnOrder();
	else return NULL;
}

void ExternDescriptedDatabase::convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(DLLconvertData) {
		DLLconvertData(dst, mode, row, rowNum);
	}
}


int ExternDescriptedDatabase::getSpecialCaseID() {
	if(DLLgetSpecialCaseID) return DLLgetSpecialCaseID();
	else return SPECIALCASE_NONE;
}

const char* ExternDescriptedDatabase::getFilename() {
	return filename.c_str();
}

} //namespace
