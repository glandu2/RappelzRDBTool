#include "SQLFileSource.h"
#include "RowManipulator.h"
#include "SQLLanguage.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "ICharsetConverter.h"
#include "ILog.h"

namespace RappelzRDBBase {

char *SQLFileSource::strreplace(char *input, char c, const char *rep) {
	int occurences;
	char *replacedStr;
	const char *p;
	char *rp;

	for(p = input, occurences = 0; *p; p++) {
		if(*p == c)
			occurences++;
	}

	if(occurences == 0)
		return input;

	replacedStr = (char*) malloc(p - input + occurences*(strlen(rep)-1) + 1);
	for(p = input, rp = replacedStr; *p; p++) {
		if(*p == c) {
			strcpy(rp, rep);
			rp += strlen(rep);
		} else {
			*rp = *p;
			rp++;
		}
	}
	*rp = 0;

	return replacedStr;
}

SQLFileSource::SQLFileSource(SQLLanguage *language) {
	tableName = 0;
	sqlLanguage = language;
	outputFile = 0;
}

SQLFileSource::~SQLFileSource() {
	close();
	if(tableName) free(tableName);
	delete sqlLanguage;
}

int SQLFileSource::open(const char* source, eOpenMode openMode, const char *location, const char* options) {
	IDataSource::open(source, openMode, location, options);

	if(openMode != OM_Write) {	//only write sql file supported
		getLogger()->log(ILog::LL_Error, "SQLFileSource: Reading from SQL script file is not supported\n");
		return ENOSYS;
	}

	outputFile = fopen(source, "w");

	if(!outputFile) {
		getLogger()->log(ILog::LL_Error, "SQLFileSource: Cannot open output file \"%s\": %s\n", source, strerror(errno));
		if(errno == ENOENT)
			return ENOENT;
		else
			return EIO;
	}

	const char *p = source + strlen(source) - 1;
	while(p >= source && *p != '/' && *p != '\\')
		p--;


	if(*p == '/' || *p == '\\')
		p++;

	const char *ext = strrchr(p, '.');
	if(!ext)
		ext = p + strlen(p);

	tableName = (char*)malloc(ext - p + 1);
	memcpy(tableName, p, ext - p);
	tableName[ext - p] = '\0';

	return 0;
}

void SQLFileSource::close() {
	if(outputFile)
		fclose(outputFile);
	outputFile = 0;

	if(tableName)
		free(tableName);
	tableName = 0;
}

int SQLFileSource::prepareRead(IRowManipulator *row) {
	getLogger()->log(ILog::LL_Error, "SQLFileSource: prepareRead: not supported\n");
	return ENOSYS;
}

int SQLFileSource::prepareWrite(IRowManipulator *row, unsigned int rowCount) {
	int curCol;
	bool isFirstColumn = true;
	char *ptr = query;

	fprintf(outputFile, "DROP TABLE %s;\n", tableName);

	if(createSQLTable()) {
		getLogger()->log(ILog::LL_Error, "SQLFileSource: Couldn't write SQL create table query to file\n");
		return ENOENT;
	}

	fputc('\n', outputFile);

	sprintf(ptr, "INSERT INTO %s (", tableName);
	ptr += strlen(ptr);

	for(curCol=0; curCol<row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE))
			continue;

		if(isFirstColumn)
			isFirstColumn = false;
		else {
			strcpy(ptr, ",");
			ptr += strlen(ptr);
		}
		strcpy(ptr, "\"");
		ptr++;
		strcpy(ptr, row->getColumnName(curCol));
		ptr += strlen(ptr);
		strcpy(ptr, "\"");
		ptr++;
	}
	strcpy(ptr, ") VALUES (");
	ptr += strlen(ptr);

	endOfHeader = ptr;
	currentRow = 0;

	return 0;
}

int SQLFileSource::createSQLTable() {
	int isFirstColumn = 1, curCol;
	IRowManipulator *row = getRowManipulator();

	fprintf(outputFile, "CREATE TABLE %s (", tableName);

	for(curCol=0; curCol<row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE))
			continue;

		if(isFirstColumn)
			isFirstColumn = 0;
		else
			fputc(',', outputFile);

		fprintf(outputFile, "\n\t\"%s\" ", row->getColumnName(curCol));

		char buffer[20];
		sqlLanguage->getTypeName(buffer, row->getType(curCol), row->getMaxDataCount(curCol), row->getDataIndex(curCol));
		fputs(buffer, outputFile);

		fputs(" NOT NULL", outputFile);

		if(GET_FLAGBIT(row->getFlags(curCol),TYPE_FLAG_KEY)) {
			fputs(" PRIMARY KEY", outputFile);
		}
	}
	fputs("\n);\nGO\n", outputFile);

	return ferror(outputFile);
}

int SQLFileSource::readRow() {
	getLogger()->log(ILog::LL_Error, "SQLFileSource: readRow: no supported\n");
	return ENOSYS;
}

int SQLFileSource::writeRow() {
	bool isFirstColumn = true;
	char *ptr = endOfHeader;
	IRowManipulator *row = getRowManipulator();
	void *buffer;
	int curCol, count;

	for(curCol=0; curCol<row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE)) continue;

		if(isFirstColumn)
			isFirstColumn = false;
		else {
			strcpy(ptr, ",");
			ptr += strlen(ptr);
		}

		count = row->getDataCount(curCol);
		buffer = row->getValuePtr(curCol);

		switch(row->getType(curCol)) {
			case TYPE_BIT:
			case TYPE_INT8:
				ptr += sprintf(ptr, "\'%d\'", *static_cast<char*>(buffer));
				break;

			case TYPE_INT16:
				ptr += sprintf(ptr, "\'%d\'", *static_cast<short*>(buffer));
				break;

			case TYPE_INT32:
				ptr += sprintf(ptr, "\'%d\'", *static_cast<int*>(buffer));
				break;

			case TYPE_INT64:
				ptr += sprintf(ptr, "\'%I64d\'", *static_cast<long long int*>(buffer));
				break;

			case TYPE_CHAR: {
				char *sqlEscapedStr = strreplace(static_cast<char*>(buffer), '\'', "\'\'");
				ptr += sprintf(ptr, "\'%.*s\'", strlen(sqlEscapedStr), sqlEscapedStr);
				if(sqlEscapedStr != buffer)
					free(sqlEscapedStr);
				break;
			}

			case TYPE_FLOAT32:
				ptr += sprintf(ptr, "\'%e\'", *static_cast<float*>(buffer));
				break;

			case TYPE_FLOAT64:
				ptr += sprintf(ptr, "\'%le\'", *static_cast<double*>(buffer));
				break;

			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR: {
				char *sqlEscapedStr = strreplace(static_cast<char*>(buffer), '\'', "\'\'");
				ptr += sprintf(ptr, "\'%.*s\'", strlen(sqlEscapedStr), sqlEscapedStr);
				if(sqlEscapedStr != buffer)
					free(sqlEscapedStr);
				break;
			}

			case TYPE_DECIMAL:
				ptr += sprintf(ptr, "\'%lf\'", (double)(*(int*)buffer)/pow(10.0, row->getDataIndex(curCol)));
				break;
		}
	}
	strcpy(ptr, ");\n");

	fputs(query, outputFile);

	currentRow++;
	if(currentRow % 100 == 0)
		fputs("GO\n", outputFile);

	return 0;
}

bool SQLFileSource::hasNext() {
	return false;
}

} //namespace
