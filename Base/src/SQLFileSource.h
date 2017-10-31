#ifndef SQLFileSource_H
#define SQLFileSource_H

#include "IDataSource.h"
#include <stdio.h>

class ICharsetConverter;

namespace RappelzRDBBase {

class SQLLanguage;

class SQLFileSource : public IDataSource {
public:
	// takes ownership of language
	SQLFileSource(SQLLanguage* language);
	~SQLFileSource();

	virtual int open(const char* source, eOpenMode openMode, const char* location = 0, const char* options = 0);
	virtual void close();

	virtual unsigned long long int getDate() { return 0; }

	virtual int prepareRead(IRowManipulator* row);
	virtual int prepareWrite(IRowManipulator* row, unsigned int rowCount);

	virtual int readRow();
	virtual int writeRow();

	virtual bool hasNext();

protected:
	int createSQLTable();
	int prepareWriteRowQuery();
	int prepareReadRowQuery();
	int prepareReadQuery();
	int prepareWriteQuery();
	char* strreplace(char* input, char c, const char* rep);

private:
	FILE* outputFile;
	char* tableName;
	SQLLanguage* sqlLanguage;
	char query[256 * 1024];
	char* endOfHeader;
	int currentRow;
};

}  // namespace RappelzRDBBase

#endif  // SQLFileSource_H
