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
		ExternDescriptedDatabase(const char* databaseName);
		~ExternDescriptedDatabase();

		virtual void DLLCALLCONV registerDBStructure(FieldDescriptor **dfm, int *size);
		virtual const char * DLLCALLCONV getSQLColumnOrder();
		virtual const char * DLLCALLCONV getCSVColumnOrder();
		virtual void DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum);
		virtual int DLLCALLCONV getSpecialCaseID();

		virtual const char* getFilename();

	private:
		int libHinst;

		typedef void (DLLCALLCONV *DLLREGISTERSTRUCTPROC)(FieldDescriptor **dfm, int *size);
		typedef const char* (DLLCALLCONV *DLLSQLORDERPROC)();
		typedef const char* (DLLCALLCONV *DLLCSVORDERPROC)();
		typedef void (DLLCALLCONV *DLLCONVERTDATAPROC)(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum);
		typedef int (DLLCALLCONV *DLLISSPECIALPROC)();

		DLLREGISTERSTRUCTPROC DLLregisterDBStructure;
		DLLSQLORDERPROC DLLgetSQLColumnOrder;
		DLLCSVORDERPROC DLLgetCSVColumnOrder;
		DLLCONVERTDATAPROC DLLconvertData;
		DLLISSPECIALPROC DLLgetSpecialCaseID;

		std::string filename;
};

} //namespace

#endif // EXTERNDESCRIPTEDDATABASE_H
