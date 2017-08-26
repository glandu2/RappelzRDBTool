#include <stdio.h>
#include <string.h>
#include "ExportDLL.h"
#include "DataType.h"
#include "SpecialDatabaseRules.h"
#include "IDatabaseDescription.h"
#include <string>
#include <algorithm>

#define DECL_CASE(name) case name: strcat(out, #name); break
#define DECL_CASE_ADD(name) case name: strcat(out, " | " #name); break

#define DECL_ADD_FLAG(var, name) if(var & name) strcat(out, " | " #name);

bool getSizeString(int type, int size, char* out) {
	int len, index;

	len = GET_VAR_LNGTH(size);
	index = GET_VAR_INDEX(size);

	if(GET_TYPE(type) == TYPE_VARCHAR_SIZE || GET_TYPE(type) == TYPE_VARCHAR_STR || GET_TYPE(type) == TYPE_NVARCHAR_STR)
		sprintf(out, "MAKEINDEXEDVAR(%d, %d)", index, len);
	else
		sprintf(out, "%d", len);

	return true;
}

static bool getTypeString(int in, char* out) {
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

	DECL_ADD_FLAG(ignoreType, TYPE_RDBIGNORE);
	DECL_ADD_FLAG(ignoreType, TYPE_SQLIGNORE);
	DECL_ADD_FLAG(ignoreType, TYPE_CSVIGNORE);
	DECL_ADD_FLAG(ignoreType, TYPE_GUIIGNORE);

	DECL_ADD_FLAG(flags, TYPE_FLAG_KEY);
	DECL_ADD_FLAG(flags, TYPE_FLAG_SORT);

	return true;
}

static const char* getSpecialCaseString(int specialCaseId) {
	switch(specialCaseId) {
		case SPECIALCASE_NONE: return "SPECIALCASE_NONE";
		case SPECIALCASE_DOUBLEFORRDB: return "SPECIALCASE_DOUBLEFORRDB";
		case SPECIALCASE_SKILLJPSP: return "SPECIALCASE_SKILLJPSP";
	}

	return "";
}



std::string getDefaultFileName(std::string filename) {
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

	std::string fallbackDefaultFileName = std::string("db_") + filename.substr(beginPos, endPos - beginPos);
	std::transform(fallbackDefaultFileName.begin() + 3, fallbackDefaultFileName.end(), fallbackDefaultFileName.begin() + 3, ::tolower);

	return fallbackDefaultFileName;
}

std::string getDefaultTableName(std::string filename) {
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

	std::string fallbackDefaultTableName = filename.substr(beginPos, endPos - beginPos) + "Resource";

	return fallbackDefaultTableName;
}

void printC(IDatabaseDescription* dbDescription) {
	FieldDescriptor* dfm;
	int dfmSize;

	char sizeStr[100];
	char typeStr[100];

	dbDescription->registerDBStructure(&dfm, &dfmSize);

	for(int i = 0; i < dfmSize; i++) {
		getSizeString(dfm[i].dataType, dfm[i].dataSize, sizeStr);
		getTypeString(dfm[i].dataType, typeStr);

		printf("{%s, %s, \"%s\"},\n", sizeStr, typeStr, dfm[i].columnName);
	}
}

void printLua(IDatabaseDescription* dbDescription) {
	FieldDescriptor* dfm;
	int dfmSize;

	char sizeStr[100];
	char typeStr[100];

	dbDescription->registerDBStructure(&dfm, &dfmSize);

	printf("rdb.fields = {\n");

	for(int i = 0; i < dfmSize; i++) {
		getSizeString(dfm[i].dataType, dfm[i].dataSize, sizeStr);
		getTypeString(dfm[i].dataType, typeStr);

		int type = GET_TYPE(dfm[i].dataType);
		if(dfm[i].dataSize > 1 || type == TYPE_VARCHAR_SIZE || type == TYPE_VARCHAR_STR || type == TYPE_NVARCHAR_STR)
			printf("\t{\"%s\", %s, %s},\n", dfm[i].columnName, typeStr, sizeStr);
		else
			printf("\t{\"%s\", %s},\n", dfm[i].columnName, typeStr);
	}

	printf("}\n\n");

	const char* sqlColumnOrder = dbDescription->getSQLColumnOrder();
	if(sqlColumnOrder) {
		printf("rdb.sqlColumnOrder = table.concat({\n");
		for(; *sqlColumnOrder != '\0'; sqlColumnOrder += strlen(sqlColumnOrder) + 1) {
			printf("\t\"%s\\0\",\n", sqlColumnOrder);
		}
		printf("})\n\n");
	}

	const char* csvColumnOrder = dbDescription->getCSVColumnOrder();
	if(csvColumnOrder) {
		printf("rdb.csvColumnOrder = table.concat({\n");
		for(; *csvColumnOrder != '\0'; csvColumnOrder += strlen(csvColumnOrder) + 1) {
			printf("\t\"%s\\0\",\n", csvColumnOrder);
		}
		printf("})\n\n");
	}

	int specialCaseId = dbDescription->getSpecialCaseID();
	if(specialCaseId != SPECIALCASE_NONE) {
		printf("rdb.specialCaseId = %s\n\n", getSpecialCaseString(specialCaseId));
	}

	const char* defaultFileName = dbDescription->getDefaultFileName();
	if(getDefaultFileName(dbDescription->getFilename()) != std::string(defaultFileName)) {
		printf("rdb.defaultFileName = \"%s\"\n\n", defaultFileName);
	}

	const char* defaultTableName = dbDescription->getDefaultTableName();
	if(getDefaultTableName(dbDescription->getFilename()) != std::string(defaultTableName)) {
		printf("rdb.defaultTableName = \"%s\"\n\n", defaultTableName);
	}
}

int main(int argc, char** argv) {
	IDatabaseDescription* dbDescription;

	if(argc < 2) {
		printf("Usage: %s <dll>\n", argv[0]);
		return 1;
	}

	const char *ext = strrchr(argv[1], '.');
	if (ext && !strcmp(ext, ".lua")) {
		dbDescription = createLuaDescriptedDatabase();
	} else {
		dbDescription = createExternDescriptedDatabase();
	}

	int result = dbDescription->open(argv[1], 0);
	if(result) {
		printf("Can't load dll %s: %d:%s\n", argv[1], result, strerror(result));
		return 2;
	}
/*
	printf("C description:\n");
	printf("-----------------\n");
	printC(dbDescription);
	printf("-----------------\n\n");

	printf("LUA description:\n");
	printf("-----------------\n");*/
	printLua(dbDescription);
	//printf("-----------------\n\n");

	dbDescription->destroy();

	return 0;
}
