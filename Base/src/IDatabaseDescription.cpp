#include "IDatabaseDescription.h"
#include "ExternDescriptedDatabase.h"
#include "LuaDescriptedDatabase.h"

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:createExternDescriptedDatabase=_createExternDescriptedDatabase@0")
#endif
IDatabaseDescription* DLLCALLCONV createExternDescriptedDatabase() {
	return new RappelzRDBBase::ExternDescriptedDatabase();
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:createLuaDescriptedDatabase=_createLuaDescriptedDatabase@0")
#endif
IDatabaseDescription* DLLCALLCONV createLuaDescriptedDatabase() {
	return new RappelzRDBBase::LuaDescriptedDatabase();
}
