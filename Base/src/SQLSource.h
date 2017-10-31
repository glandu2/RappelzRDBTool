#ifndef SQLSOURCE_H
#define SQLSOURCE_H

#include "IDataSource.h"
#ifdef __unix__
#include <sql.h>
#else
#include <windows.h>
#endif
#include <sqlext.h>
#include <stdio.h>
#include <vector>

class ICharsetConverter;

namespace RappelzRDBBase {

class SQLLanguage;

class SQLSource : public IDataSource {
public:
	// takes ownership of language
	SQLSource(SQLLanguage* language);
	~SQLSource();

	virtual int open(const char* source, eOpenMode openMode, const char* location = 0, const char* options = 0);
	virtual void close();

	virtual unsigned long long int getDate() { return 0; }

	virtual int prepareRead(IRowManipulator* row);
	virtual int prepareWrite(IRowManipulator* row, unsigned int rowCount);

	virtual int readRow();
	virtual int writeRow();

	virtual bool hasNext();

protected:
	struct ParameterInfo {
		ParameterInfo(void* data = 0, SQLLEN dataSizeOrInd = 0) : data(data), dataSizeOrInd(dataSizeOrInd) {}
		void* data;
		SQLLEN dataSizeOrInd;
	};

	int createSQLTable(SQLHSTMT hstmt, const char* table);
	int prepareWriteRowQuery();
	int prepareReadRowQuery(SQLHSTMT hstmt);
	int prepareReadQuery();
	int prepareWriteQuery();
	char* strreplace(const char* input, char c, const char* rep);
	char* appendColumnNames(char* p);
	int checkSqlResult(int result, const char* functionName, bool ignoreError = false);

	void clearBoundParameters(int newCount);
	ParameterInfo* initParameter(int index, SQLLEN dataSizeOrInd);
	void* getParameter(int index);
	SQLLEN* getParameterSizeOrInd(int index);
	void setParameterData(int index, void* data, SQLLEN size);

	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
	bool commitTransaction;

private:
	char* tableName;
	SQLLanguage* sqlLanguage;
	char query[256 * 1024];
	bool reuseTableSchema;

	std::vector<ParameterInfo> boundBuffers;
	std::vector<int> columnsToProcess;

	bool endOfRecordSet;
	ICharsetConverter* utf16To8bits;
};

}  // namespace RappelzRDBBase

#endif  // SQLSOURCE_H
