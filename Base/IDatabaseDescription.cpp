#include "IDatabaseDescription.h"
#include "ExternDescriptedDatabase.h"

#pragma comment(linker, "/EXPORT:createExternDescriptedDatabase=_createExternDescriptedDatabase@0")
IDatabaseDescription * DLLCALLCONV createExternDescriptedDatabase() {
	return new RappelzRDBBase::ExternDescriptedDatabase();
}
