#ifndef DATABASEDESCRIPTION_H
#define DATABASEDESCRIPTION_H

#include "DataType.h"
#include "IObject.h"

class IDatabaseDescription;

#ifdef __cplusplus
extern "C" {
#endif

EBASEDLL IDatabaseDescription * DLLCALLCONV createExternDescriptedDatabase();
EBASEDLL IDatabaseDescription * DLLCALLCONV createLuaDescriptedDatabase();

#ifdef __cplusplus
}
#endif

class IRowManipulator;

class IDatabaseDescription : public IObject
{
	public:
		virtual int DLLCALLCONV open(const char* databaseName, int* systemError) = 0;
		virtual void DLLCALLCONV registerDBStructure(FieldDescriptor **dfm, int *size) = 0;
		virtual const char * DLLCALLCONV getSQLColumnOrder() = 0;
		virtual const char * DLLCALLCONV getCSVColumnOrder() = 0;
		virtual void DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) = 0;
		virtual int DLLCALLCONV getSpecialCaseID() = 0;

		virtual const char* getFilename() = 0;

		virtual const char* getDefaultFileName() = 0;    //get the default filename, like db_string for StringDatabase (without extension).
		virtual const char* getDefaultTableName() = 0;   //get the default table name for use with sql servers. (without dbo. or anything before that)

		//Incoming version:
		/*
		virtual int getDBStructVersion();            //get the database description object version. To prevent version mismash and crashs. Might also used for a compatibility layer.
		*/
};

#endif // DATABASEDESCRIPTION_H
