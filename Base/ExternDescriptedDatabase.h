#ifndef EXTERNDESCRIPTEDDATABASE_H
#define EXTERNDESCRIPTEDDATABASE_H

#include "IDatabaseDescription.h"
#include "ExportDLL.h"
#include <string>

class IRowManipulator;

namespace RappelzRDBBase {

class ExternDescriptedDatabase : public CImplement<IDatabaseDescription>
{
	public:
        ExternDescriptedDatabase();
		~ExternDescriptedDatabase();

        virtual int DLLCALLCONV open(const char* databaseName, int* systemError);
		virtual void DLLCALLCONV registerDBStructure(FieldDescriptor **dfm, int *size);
		virtual const char * DLLCALLCONV getSQLColumnOrder();
		virtual const char * DLLCALLCONV getCSVColumnOrder();
		virtual void DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum);
		virtual int DLLCALLCONV getSpecialCaseID();

		virtual const char* getFilename();

		virtual const char* getDefaultFileName();
		virtual const char* getDefaultTableName();

	private:
		unsigned long long libHinst;

		typedef void (DLLCALLCONV *DLLREGISTERSTRUCTPROC)(FieldDescriptor **dfm, int *size);
		typedef const char* (DLLCALLCONV *DLLSQLORDERPROC)();
		typedef const char* (DLLCALLCONV *DLLCSVORDERPROC)();
		typedef void (DLLCALLCONV *DLLCONVERTDATAPROC)(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum);
		typedef int (DLLCALLCONV *DLLISSPECIALPROC)();
		typedef const char* (DLLCALLCONV *DLLDEFAULTFILENAMEPROC)();
		typedef const char* (DLLCALLCONV *DLLDEFAULTTABLENAMEPROC)();

		DLLREGISTERSTRUCTPROC DLLregisterDBStructure;
		DLLSQLORDERPROC DLLgetSQLColumnOrder;
		DLLCSVORDERPROC DLLgetCSVColumnOrder;
		DLLCONVERTDATAPROC DLLconvertData;
		DLLISSPECIALPROC DLLgetSpecialCaseID;
		DLLDEFAULTFILENAMEPROC DLLgetDefaultFileName;
		DLLDEFAULTTABLENAMEPROC DLLgetDefaultTableName;

		std::string filename;
		std::string fallbackDefaultFileName, fallbackDefaultTableName;
};

} //namespace

#endif // EXTERNDESCRIPTEDDATABASE_H
