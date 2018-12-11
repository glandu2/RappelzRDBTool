#ifndef DATABASEDESCRIPTION_H
#define DATABASEDESCRIPTION_H

#include "DataType.h"
#include "IObject.h"
#include <stdint.h>

class IDatabaseDescription;

#ifdef __cplusplus
extern "C" {
#endif

EBASEDLL IDatabaseDescription* DLLCALLCONV createExternDescriptedDatabase();
EBASEDLL IDatabaseDescription* DLLCALLCONV createLuaDescriptedDatabase();

#ifdef __cplusplus
}
#endif

class IRowManipulator;

class IDatabaseDescription : public IObject {
public:
	virtual int DLLCALLCONV open(const char* databaseName, int* systemError) = 0;
	virtual void DLLCALLCONV registerDBStructure(FieldDescriptor** dfm, int* size) = 0;
	virtual const char* DLLCALLCONV getSQLColumnOrder() = 0;
	virtual const char* DLLCALLCONV getCSVColumnOrder() = 0;
	virtual void DLLCALLCONV convertData(eDataFormat dst,
	                                     eDataConvertionType mode,
	                                     IRowManipulator* row,
	                                     unsigned int rowNum) = 0;
	virtual int DLLCALLCONV getSpecialCaseID() = 0;

	virtual const char* DLLCALLCONV getFilename() = 0;

	// get the default filename, like db_string for StringDatabase (without extension).
	virtual const char* DLLCALLCONV getDefaultFileName() = 0;

	// get the default table name for use with sql servers.
	// (without dbo. or anything before that)
	virtual const char* DLLCALLCONV getDefaultTableName() = 0;

	virtual uint64_t DLLCALLCONV getRdbDate(eDataFormat dst,
	                                        eDataConvertionType mode,
	                                        unsigned long long originalDate) = 0;
};

#endif  // DATABASEDESCRIPTION_H
