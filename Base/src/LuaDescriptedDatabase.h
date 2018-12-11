#ifndef LUADESCRIPTEDDATABASE_H
#define LUADESCRIPTEDDATABASE_H

#include "ExportDLL.h"
#include "IDatabaseDescription.h"
#include <string>
#include <vector>

class IRowManipulator;

namespace RappelzRDBBase {

class LuaDescriptedDatabase : public CImplement<IDatabaseDescription> {
public:
	LuaDescriptedDatabase();
	~LuaDescriptedDatabase();

	virtual int DLLCALLCONV open(const char* databaseName, int* systemError);
	virtual void DLLCALLCONV registerDBStructure(FieldDescriptor** dfm, int* size);
	virtual const char* DLLCALLCONV getSQLColumnOrder();
	virtual const char* DLLCALLCONV getCSVColumnOrder();
	virtual void DLLCALLCONV convertData(eDataFormat dst,
	                                     eDataConvertionType mode,
	                                     IRowManipulator* row,
	                                     unsigned int rowNum);
	virtual int DLLCALLCONV getSpecialCaseID();

	virtual const char* DLLCALLCONV getFilename();

	virtual const char* DLLCALLCONV getDefaultFileName();
	virtual const char* DLLCALLCONV getDefaultTableName();

	virtual uint64_t DLLCALLCONV getRdbDate(eDataFormat dst, eDataConvertionType mode, uint64_t originalDate);

protected:
	void freeFields();
	bool registerLuaFieldsDescription();

private:
	std::string filename;
	std::string fallbackDefaultFileName, fallbackDefaultTableName;
	void* state;

	std::vector<FieldDescriptor> fields;
	std::string sqlColumnOrder;
	std::string csvColumnOrder;
	int convertDataFunctionRef;
	int getRdbDateFunctionRef;
	int specialCaseId;
	std::string overridenFilename;
	std::string overridenTablename;
};

}  // namespace RappelzRDBBase

#endif  // LUADESCRIPTEDDATABASE_H
