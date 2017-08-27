#include "LuaDescriptedDatabase.h"
#include "SpecialDatabaseRules.h"
#include <errno.h>
#include <string.h>
#include <algorithm>
#include "Log.h"
#include <lua.hpp>
#include "IRowManipulator.h"

#include <stdio.h>

namespace RappelzRDBBase {

static const char* const LUA_ROWMANIPULATOR_METATABLE = "rdb.RowManipulator";

LuaDescriptedDatabase::LuaDescriptedDatabase() :
    state(nullptr),
    convertDataFunctionRef(LUA_NOREF),
    specialCaseId(SPECIALCASE_NONE)
{}

LuaDescriptedDatabase::~LuaDescriptedDatabase() {
	lua_State *L = (lua_State*) state;
	if(convertDataFunctionRef != LUA_NOREF) {
		luaL_unref(L, LUA_REGISTRYINDEX, convertDataFunctionRef);
		convertDataFunctionRef = LUA_NOREF;
	}
	freeFields();
	if(L)
		lua_close(L);
}

bool LuaDescriptedDatabase::registerLuaFieldsDescription() {
	lua_State* L = (lua_State*) state;

	if(lua_type(L, -1) != LUA_TTABLE) {
		getLogger()->log(ILog::LL_Error, "Lua register: \"rdb.fields\" must be a table containing fields descriptions\n");
		return false;
	}

	freeFields();

	for(size_t i = 1; lua_geti(L, -1, i) != LUA_TNIL; i++)
	{
		FieldDescriptor currentField;
		std::string fieldName;

		// field details
		if(lua_type(L, -1) != LUA_TTABLE) {
			getLogger()->log(ILog::LL_Error, "Lua register: error for field %s: value must be a table containing field details\n", lua_tostring(L, -2));
			lua_pop(L, 1);
			return false;
		}

		// first: field name
		if(lua_geti(L, -1, 1) != LUA_TSTRING) {
			getLogger()->log(ILog::LL_Error, "Lua register: error for field %d: first detail must be the field name as a string\n", i);
			lua_pop(L, 2);
			return false;
		}
		fieldName.assign(lua_tostring(L, -1));
		lua_pop(L, 1);

		// check number of details
		size_t optionNumber = lua_rawlen(L, -1);
		if(optionNumber > 3) {
			getLogger()->log(ILog::LL_Error, "Lua register: Bad number of options for field %s, must have field name, field type and optionnaly field length (for arrays)\n",
			                  fieldName.c_str());
			lua_pop(L, 1);
			return false;
		}

		// second: field type
		if(lua_geti(L, -1, 2) != LUA_TNUMBER || !lua_isinteger(L, -1)) {
			getLogger()->log(ILog::LL_Error, "Lua register: error for field %s: second detail must be an integer using TYPE_*, got %s with type %s\n",
			                 fieldName.c_str(),
			                 lua_tostring(L, -1),
			                 lua_typename(L, lua_type(L, -1)));
			lua_pop(L, 2);
			return false;
		}
		lua_Integer fieldType = lua_tointeger(L, -1);
		lua_pop(L, 1);

		// third: field length
		int thirdOptionType = lua_geti(L, -1, 3);
		if(thirdOptionType != LUA_TNIL && !lua_isinteger(L, -1)) {
			getLogger()->log(ILog::LL_Error, "Lua register: error for field %s: third detail is number of fields, must be an integer\n",
			                 fieldName.c_str());
			lua_pop(L, 2);
			return false;
		}
		lua_Integer fieldNumber = 1; //default to 1 if absent
		if(thirdOptionType != LUA_TNIL) {
			fieldNumber = lua_tointeger(L, -1);
		}
		lua_pop(L, 1);

		currentField.dataType = static_cast<int>(fieldType);
		currentField.dataSize = static_cast<int>(fieldNumber);
		currentField.columnName = strdup(fieldName.c_str());
		fields.push_back(currentField);

		// removes 'value'; keeps 'key' for next iteration
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	return true;
}

static int lua_script_row_manipulator_index(lua_State* L) {
	IRowManipulator *row = (IRowManipulator*) luaL_checkudata(L, 1, LUA_ROWMANIPULATOR_METATABLE);
	const char* columnName = luaL_checkstring(L, 2);

	int columnIndex = row->getColumnIndex(columnName);
	if(columnIndex < 0 || columnIndex > row->getColumnCount()) {
		return 0;
	}

	int columnType = row->getType(columnIndex);

	switch(columnType) {
		case TYPE_BIT:   lua_pushboolean(L, row->getDataBit(columnName)); break;
		case TYPE_INT8:  lua_pushinteger(L, row->getDataInt8(columnName)); break;
		case TYPE_INT16: lua_pushinteger(L, row->getDataInt16(columnName)); break;
		case TYPE_INT32: lua_pushinteger(L, row->getDataInt32(columnName)); break;
		case TYPE_INT64: lua_pushinteger(L, row->getDataInt64(columnName)); break;
		case TYPE_FLOAT32: lua_pushnumber(L, row->getDataFloat32(columnName)); break;
		case TYPE_FLOAT64: lua_pushnumber(L, row->getDataFloat64(columnName)); break;
		case TYPE_DECIMAL: lua_pushnumber(L, row->getDataDecimal(columnName)); break;

		case TYPE_CHAR:
		case TYPE_VARCHAR_STR:
		case TYPE_NVARCHAR_STR:
			lua_pushstring(L, row->getDataString(columnName));
			break;

		case TYPE_VARCHAR_SIZE:
			getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to get a string size column %s, which can only be get via the string itself (use the TYPE_VARCHAR_STR field instead)\n",
			                 columnName);
			lua_pushnil(L);
			break;

		default:
			getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to get column %s with unknown type %s\n",
			                 columnName, columnType);
			lua_pushnil(L);
			break;

	}

	return 1;
}

static int lua_script_row_manipulator_newindex(lua_State* L) {
	IRowManipulator *row = (IRowManipulator*) luaL_checkudata(L, 1, LUA_ROWMANIPULATOR_METATABLE);
	const char* columnName = luaL_checkstring(L, 2);

	int columnIndex = row->getColumnIndex(columnName);
	if(columnIndex < 0 || columnIndex > row->getColumnCount()) {
		return 0;
	}

	int columnType = row->getType(columnIndex);

	switch(columnType) {
		case TYPE_BIT: {
			int value = lua_toboolean(L, 3);
			row->setDataBit(columnName, value);
			break;
		}

		case TYPE_INT8: {
			int isNum;
			long long int value = lua_tointegerx(L, 3, &isNum);
			if(!isNum) {
				getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set integer column %s with a non integer value with type %s\n",
				                 columnName, lua_typename(L, lua_type(L, 3)));
			} else {
				row->setDataInt8(columnName, static_cast<char>(value));
			}
			break;
		}

		case TYPE_INT16: {
			int isNum;
			long long int value = lua_tointegerx(L, 3, &isNum);
			if(!isNum) {
				getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set integer column %s with a non integer value with type %s\n",
				                 columnName, lua_typename(L, lua_type(L, 3)));
			} else {
				row->setDataInt16(columnName, static_cast<short>(value));
			}
			break;
		}

		case TYPE_INT32: {
			int isNum;
			long long int value = lua_tointegerx(L, 3, &isNum);
			if(!isNum) {
				getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set integer column %s with a non integer value with type %s\n",
				                 columnName, lua_typename(L, lua_type(L, 3)));
			} else {
				row->setDataInt32(columnName, static_cast<int>(value));
			}
			break;
		}

		case TYPE_INT64: {
			int isNum;
			long long int value = lua_tointegerx(L, 3, &isNum);
			if(!isNum) {
				getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set integer column %s with a non integer value with type %s\n",
				                 columnName, lua_typename(L, lua_type(L, 3)));
			} else {
				row->setDataInt64(columnName, value);
			}
			break;
		}

		case TYPE_FLOAT32: {
			int isNum;
			double value = lua_tonumberx(L, 3, &isNum);
			if(!isNum) {
				getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set float column %s with a non number value with type %s\n",
				                 columnName, lua_typename(L, lua_type(L, 3)));
			} else {
				row->setDataFloat32(columnName, static_cast<float>(value));
			}
			break;
		}

		case TYPE_FLOAT64: {
			int isNum;
			double value = lua_tonumberx(L, 3, &isNum);
			if(!isNum) {
				getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set float column %s with a non number value with type %s\n",
				                 columnName, lua_typename(L, lua_type(L, 3)));
			} else {
				row->setDataFloat64(columnName, value);
			}
			break;
		}

		case TYPE_DECIMAL: {
			int isNum;
			double value = lua_tonumberx(L, 3, &isNum);
			if(!isNum) {
				getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set decimal column %s with a non number value with type %s\n",
				                 columnName, lua_typename(L, lua_type(L, 3)));
			} else {
				row->setDataDecimal(columnName, static_cast<float>(value));
			}
			break;
		}

		case TYPE_CHAR:
		case TYPE_VARCHAR_STR:
		case TYPE_NVARCHAR_STR: {
			const char* value = lua_tostring(L, 3);
			row->setDataString(columnName, value);
			break;
		}

		case TYPE_VARCHAR_SIZE:
			getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set a string size column %s, which can only be set via the string itself (use the TYPE_VARCHAR_STR field instead)\n",
			                 columnName);
			break;

		default:
			getLogger()->log(ILog::LL_Error, "Lua RowManipulator: Attempt to set column %s with unknown type %s\n",
			                 columnName, columnType);
			break;

	}

	return 0;
}

static const struct luaL_Reg lua_script_row_manipulator_metatable[] = {
    {"__index", &lua_script_row_manipulator_index},
    {"__newindex", &lua_script_row_manipulator_newindex},
    {NULL, NULL}
};

static int lua_script_field_descriptor_makeindexedvar(lua_State *L) {
	luaL_checkinteger(L, 1);
	luaL_checkinteger(L, 2);

	lua_pushinteger(L, MAKEINDEXEDVAR(lua_tointeger(L, 1), lua_tointeger(L, 2)));
	return 1;
}

static int lua_script_print_to_log(lua_State* L) {
	int nargs = lua_gettop(L);

	for (int i=1; i <= nargs; i++) {
		const char* str = lua_tostring(L, i);
		getLogger()->log(ILog::LL_Info, "Lua message: %s\n", str);
	}

	return 0;
}

static int lua_script_field_descriptor_init(lua_State *L) {
	luaL_newmetatable(L, LUA_ROWMANIPULATOR_METATABLE);
	luaL_setfuncs(L, lua_script_row_manipulator_metatable, 0);
	lua_pop(L, 1);

#define DEFINE_LUA_CONST(_name) lua_pushinteger(L, _name); lua_setglobal(L, #_name);
#define DEFINE_LUA_FUNCTION(_name, _ptr) lua_pushcfunction(L, _ptr); lua_setglobal(L, _name);

	DEFINE_LUA_FUNCTION("MAKEINDEXEDVAR", &lua_script_field_descriptor_makeindexedvar);
	DEFINE_LUA_FUNCTION("print", &lua_script_print_to_log);

	DEFINE_LUA_CONST(TYPE_BIT);
	DEFINE_LUA_CONST(TYPE_INT8);
	DEFINE_LUA_CONST(TYPE_INT16);
	DEFINE_LUA_CONST(TYPE_INT32);
	DEFINE_LUA_CONST(TYPE_INT64);
	DEFINE_LUA_CONST(TYPE_FLOAT32);
	DEFINE_LUA_CONST(TYPE_FLOAT64);
	DEFINE_LUA_CONST(TYPE_CHAR);
	DEFINE_LUA_CONST(TYPE_VARCHAR_SIZE);
	DEFINE_LUA_CONST(TYPE_VARCHAR_STR);
	DEFINE_LUA_CONST(TYPE_DECIMAL);
	DEFINE_LUA_CONST(TYPE_NVARCHAR_STR);

	DEFINE_LUA_CONST(TYPE_RDBIGNORE);
	DEFINE_LUA_CONST(TYPE_SQLIGNORE);
	DEFINE_LUA_CONST(TYPE_CSVIGNORE);
	DEFINE_LUA_CONST(TYPE_GUIIGNORE);

	DEFINE_LUA_CONST(TYPE_FLAG_KEY);
	DEFINE_LUA_CONST(TYPE_FLAG_SORT);

	DEFINE_LUA_CONST(DF_None);
	DEFINE_LUA_CONST(DF_RDB);
	DEFINE_LUA_CONST(DF_CSV);
	DEFINE_LUA_CONST(DF_SQL);

	DEFINE_LUA_CONST(DCT_Read);
	DEFINE_LUA_CONST(DCT_Write);

	DEFINE_LUA_CONST(SPECIALCASE_NONE);
	DEFINE_LUA_CONST(SPECIALCASE_DOUBLEFORRDB);
	DEFINE_LUA_CONST(SPECIALCASE_SKILLJPSP);

#undef DEFINE_LUA_FUNCTION
#undef DEFINE_LUA_CONST

	return 1;
}

int LuaDescriptedDatabase::open(const char* databaseName, int* systemError) {
	int dummy;
	if(!systemError) systemError = &dummy;

	*systemError = 0;

	lua_State *L = luaL_newstate();
	state = L;

	luaL_openlibs(L);
	lua_script_field_descriptor_init(L);

	lua_newtable(L);
	lua_setglobal(L, "rdb");

	int result  = luaL_loadfilex(L, databaseName, nullptr);
	if (result) {
		getLogger()->log(ILog::LL_Error, "Lua register: Cannot load lua description file %s: %s\n",
		                 databaseName,
		                 lua_tostring(L, -1));
		*systemError = errno;
		return ENOENT;
	}

	result = lua_pcall(L, 0, 0, 0);
	if (result) {
		getLogger()->log(ILog::LL_Error, "Lua register: Cannot run lua description file %s: %s\n",
		                 databaseName,
		                 lua_tostring(L, -1));
		*systemError = -1;
		return EINVAL;
	}

	lua_getglobal(L, "rdb");
	lua_getfield(L, -1, "fields");
	if(!registerLuaFieldsDescription()) {
		lua_pop(L, 1);
		*systemError = -2;
		return EINVAL;
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "sqlColumnOrder");
	if(!lua_isnil(L, -1)) {
		if(lua_isstring(L, -1)) {
			size_t len;
			const char* s = lua_tolstring(L, -1, &len);
			sqlColumnOrder.assign(s, len);
		} else {
			getLogger()->log(ILog::LL_Warning, "Lua register: rdb.sqlColumnOrder must be a string with a list of SQL columns separated by \\0\n");
		}
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "csvColumnOrder");
	if(!lua_isnil(L, -1)) {
		if(lua_isstring(L, -1)) {
			size_t len;
			const char* s = lua_tolstring(L, -1, &len);
			csvColumnOrder.assign(s, len);
		} else {
			getLogger()->log(ILog::LL_Warning, "Lua register: rdb.csvColumnOrder must be a string with a list of CSV columns separated by \\0\n");
		}
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "convertData");
	if(!lua_isnil(L, -1)) {
		if(lua_isfunction(L, -1)) {
			convertDataFunctionRef = luaL_ref(L, LUA_REGISTRYINDEX);
			lua_pushnil(L); // for the next pop as luaL_ref pop the value
		} else {
			getLogger()->log(ILog::LL_Warning, "Lua register: rdb.convertData must be a lua function matching its C counterpart: void convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum)\n");
		}
	}
	lua_pop(L, 1);

	specialCaseId = SPECIALCASE_NONE;
	lua_getfield(L, -1, "specialCaseId");
	if(!lua_isnil(L, -1)) {
		if(lua_isinteger(L, -1)) {
			specialCaseId = static_cast<int>(lua_tointeger(L, -1));
		} else {
			getLogger()->log(ILog::LL_Warning, "Lua register: rdb.specialCaseId must be a integer using one of SPECIALCASE_* constants\n");
		}
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "defaultFileName");
	if(!lua_isnil(L, -1)) {
		if(lua_isstring(L, -1)) {
			const char* s = lua_tostring(L, -1);
			overridenFilename.assign(s);
		} else {
			getLogger()->log(ILog::LL_Warning, "Lua register: rdb.defaultFileName must be a string with the default RDB filename\n");
		}
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "defaultTableName");
	if(!lua_isnil(L, -1)) {
		if(lua_isstring(L, -1)) {
			const char* s = lua_tostring(L, -1);
			overridenTablename.assign(s);
		} else {
			getLogger()->log(ILog::LL_Warning, "Lua register: rdb.defaultTableName must be a string with the default SQL table name\n");
		}
	}
	lua_pop(L, 1);


	filename = databaseName;

	return 0;
}

void LuaDescriptedDatabase::registerDBStructure(FieldDescriptor **dfm, int *size) {
	*dfm = fields.data();
	*size = fields.size();
}

const char* LuaDescriptedDatabase::getSQLColumnOrder() {
	if(!sqlColumnOrder.empty())
		return sqlColumnOrder.c_str();
	else
		return nullptr;
}

const char* LuaDescriptedDatabase::getCSVColumnOrder() {
	if(!csvColumnOrder.empty())
		return csvColumnOrder.c_str();
	else
		return nullptr;
}

void LuaDescriptedDatabase::convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	lua_State *L = (lua_State*) state;

	if(convertDataFunctionRef != LUA_REFNIL && convertDataFunctionRef != LUA_NOREF) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, convertDataFunctionRef);
		lua_pushinteger(L, dst);
		lua_pushinteger(L, mode);
		lua_pushlightuserdata(L, row);
		luaL_getmetatable(L, LUA_ROWMANIPULATOR_METATABLE);
		lua_setmetatable(L, -2);
		lua_pushinteger(L, rowNum);

		int result = lua_pcall(L, 4, 0, 0);
		if (result) {
			getLogger()->log(ILog::LL_Error, "Cannot run lua convertData function: %d:%s\n",
			                 result,
			                 lua_tostring(L, -1));
		}
	}
}


int LuaDescriptedDatabase::getSpecialCaseID() {
	return specialCaseId;
}

const char* LuaDescriptedDatabase::getFilename() {
	return filename.c_str();
}

const char* LuaDescriptedDatabase::getDefaultFileName() {
	if(!overridenFilename.empty())
		return overridenFilename.c_str();

	if(fallbackDefaultFileName.size() > 0)
		return fallbackDefaultFileName.c_str();

	size_t endPos = filename.find("Database.");
	if(endPos == std::string::npos)
		endPos = filename.find_last_of('.');
	if(endPos == std::string::npos)
		endPos = filename.size();

	//Discard version number
	while(endPos > 0 && isdigit(filename.at(endPos-1)))
		endPos--;

	size_t beginPos = filename.find_last_of("/\\");

	if(beginPos == std::string::npos || beginPos > endPos)
		beginPos = 0;
	else
		beginPos++;

	fallbackDefaultFileName = std::string("db_") + filename.substr(beginPos, endPos - beginPos);
	std::transform(fallbackDefaultFileName.begin() + 3, fallbackDefaultFileName.end(), fallbackDefaultFileName.begin() + 3, ::tolower);

	return fallbackDefaultFileName.c_str();
}

const char* LuaDescriptedDatabase::getDefaultTableName() {
	if(!overridenTablename.empty())
		return overridenTablename.c_str();

	if(fallbackDefaultTableName.size() > 0)
		return fallbackDefaultTableName.c_str();

	size_t endPos = filename.find("Database.");
	if(endPos == std::string::npos)
		endPos = filename.find_last_of('.');
	if(endPos == std::string::npos)
		endPos = filename.size();

	//Discard version number
	while(endPos > 0 && isdigit(filename.at(endPos-1)))
		endPos--;

	size_t beginPos = filename.find_last_of("/\\");

	if(beginPos == std::string::npos || beginPos > endPos)
		beginPos = 0;
	else
		beginPos++;

	fallbackDefaultTableName = filename.substr(beginPos, endPos - beginPos) + "Resource";

	return fallbackDefaultTableName.c_str();
}

void LuaDescriptedDatabase::freeFields()
{
	for(size_t i = 0; i < fields.size(); i++) {
		if(fields[i].columnName) {
			free((void*)fields[i].columnName);
			fields[i].columnName = nullptr;
		}
	}
	fields.clear();
}

} //namespace
