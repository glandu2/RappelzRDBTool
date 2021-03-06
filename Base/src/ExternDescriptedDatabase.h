#ifndef EXTERNDESCRIPTEDDATABASE_H
#define EXTERNDESCRIPTEDDATABASE_H

#include "ExportDLL.h"
#include "IDatabaseDescription.h"
#include <string>

class IRowManipulator;

namespace RappelzRDBBase {

class ExternDescriptedDatabase : public CImplement<IDatabaseDescription> {
public:
	ExternDescriptedDatabase();
	~ExternDescriptedDatabase();

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
	void* getProcAddress(const char* name, int argumentsBytes);
	static void* nativeGetProcAddress(unsigned long long handle, const char* name);

private:
	unsigned long long libHinst;

	typedef void(DLLCALLCONV* DLLREGISTERSTRUCTPROC)(FieldDescriptor** dfm, int* size);
	typedef const char*(DLLCALLCONV* DLLSQLORDERPROC)();
	typedef const char*(DLLCALLCONV* DLLCSVORDERPROC)();
	typedef void(DLLCALLCONV* DLLCONVERTDATAPROC)(eDataFormat dst,
	                                              eDataConvertionType mode,
	                                              IRowManipulator* row,
	                                              unsigned int rowNum);
	typedef int(DLLCALLCONV* DLLISSPECIALPROC)();
	typedef const char*(DLLCALLCONV* DLLDEFAULTFILENAMEPROC)();
	typedef const char*(DLLCALLCONV* DLLDEFAULTTABLENAMEPROC)();
	typedef uint64_t(DLLCALLCONV* DLLRDBDATEPROC)(eDataFormat dst, eDataConvertionType mode, uint64_t originalDate);

	DLLREGISTERSTRUCTPROC DLLregisterDBStructure;
	DLLSQLORDERPROC DLLgetSQLColumnOrder;
	DLLCSVORDERPROC DLLgetCSVColumnOrder;
	DLLCONVERTDATAPROC DLLconvertData;
	DLLISSPECIALPROC DLLgetSpecialCaseID;
	DLLDEFAULTFILENAMEPROC DLLgetDefaultFileName;
	DLLDEFAULTTABLENAMEPROC DLLgetDefaultTableName;
	DLLRDBDATEPROC DLLgetRdbDate;

	std::string filename;
	std::string fallbackDefaultFileName, fallbackDefaultTableName;
};

}  // namespace RappelzRDBBase

#endif  // EXTERNDESCRIPTEDDATABASE_H
