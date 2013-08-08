#ifndef SQLSOURCE_H
#define SQLSOURCE_H

#include "IDataSource.h"
#ifdef __unix__
#  include <sql.h>
#else
#  include <windows.h>
#endif
#include <sqlext.h>
#include <stdio.h>

namespace RappelzRDBBase {

class SQLLanguage;

class SQLSource : public IDataSource
{
	public:

		//takes ownership of language
		SQLSource(SQLLanguage *language);
		~SQLSource();

		virtual int open(const char* source, eOpenMode openMode,  const char* location = 0, const char* user = 0, const char* password = 0, const char* options = 0);
		virtual void close();

		virtual int prepareRead(IRowManipulator *row);
		virtual int prepareWrite(IRowManipulator *row, unsigned int rowCount);

		virtual int readRow();
		virtual int writeRow();

		virtual bool hasNext();

	protected:
		int createSQLTable(SQLHSTMT hstmt, const char *table);
		int prepareWriteRowQuery();
		int prepareReadRowQuery(SQLHSTMT hstmt);
		int prepareReadQuery();
		int prepareWriteQuery();
		char *strreplace(const char *input, char c, const char *rep);

		SQLHENV henv;
		SQLHDBC hdbc;
		SQLHSTMT hstmt;
		bool commitTransaction;

	private:
		FILE *outputFile;
		bool fileOutputMode;
		char *tableName;
		SQLLanguage *sqlLanguage;
		char query[256*1024];
		char *endOfHeader;

		bool endOfRecordSet;

};

} //namespace

#endif // SQLSOURCE_H
