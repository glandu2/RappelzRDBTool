#include "IDatabaseDescription.h"
#include "ExternDescriptedDatabase.h"

#pragma comment(linker, "/EXPORT:createExternDescriptedDatabase=_createExternDescriptedDatabase@4")
IDatabaseDescription * DLLCALLCONV createExternDescriptedDatabase(const char* databaseName) {
	try {
		return new RappelzRDBBase::ExternDescriptedDatabase(databaseName);
	} catch(...) {
		return 0;
	}
}
