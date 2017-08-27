#include "IDatabaseDescription.h"
#include "ExternDescriptedDatabase.h"
#include "LuaDescriptedDatabase.h"

#pragma comment(linker, "/EXPORT:createExternDescriptedDatabase=_createExternDescriptedDatabase@0")
IDatabaseDescription * DLLCALLCONV createExternDescriptedDatabase() {
	return new RappelzRDBBase::ExternDescriptedDatabase();
}

#pragma comment(linker, "/EXPORT:createLuaDescriptedDatabase=_createLuaDescriptedDatabase@0")
IDatabaseDescription * DLLCALLCONV createLuaDescriptedDatabase() {
	return new RappelzRDBBase::LuaDescriptedDatabase();
}
