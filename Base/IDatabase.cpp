#include "IDatabase.h"
#include "Database.h"

#pragma comment(linker, "/EXPORT:createDatabase=_createDatabase@4")
IDatabase * DLLCALLCONV createDatabase(IDatabaseDescription *databaseDescription) {
	try {
		return new RappelzRDBBase::Database(databaseDescription);
	} catch(...) {
		return 0;
	}
}
