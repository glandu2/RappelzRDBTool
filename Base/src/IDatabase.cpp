#include "IDatabase.h"
#include "Database.h"

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:createDatabase=_createDatabase@4")
#endif
IDatabase* DLLCALLCONV createDatabase(IDatabaseDescription* databaseDescription) {
	try {
		return new RappelzRDBBase::Database(databaseDescription);
	} catch(...) {
		return 0;
	}
}
