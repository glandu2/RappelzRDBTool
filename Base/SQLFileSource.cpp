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

namespace RappelzRDBBase {

char *SQLFileSource::strreplace(const char *input, char c, const char *rep) {
	int occurences;
	char *replacedStr;
	const char *p;
	char *rp;

	for(p = input, occurences = 0; *p; p++) {
		if(*p == c)
			occurences++;
	}

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
	setlocale(LC_ALL, "C");
}

SQLFileSource::~SQLFileSource() {
	close();
	if(tableName) free(tableName);
	delete sqlLanguage;
}

int SQLFileSource::open(const char* source, eOpenMode openMode,  const char* location, const char* user, const char* password, const char* options) {
	IDataSource::open(source, openMode, location, user, password);

	if(openMode != OM_Write)	//only write sql file supported
		return ENOSYS;

	outputFile = fopen(source, "w");
	if(!outputFile)
		return errno;

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
	return ENOSYS;
}

int SQLFileSource::prepareWrite(IRowManipulator *row, unsigned int rowCount) {
	int curCol;
	bool isFirstColumn = true;
	char *ptr = query;

	fprintf(outputFile, "DROP TABLE %s;\n", tableName);

	if(createSQLTable())
		return EACCES;

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
	fputs("\n);\n", outputFile);

	return ferror(outputFile);
}

int SQLFileSource::readRow() {
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
				sprintf(ptr, "\'%d\'", *static_cast<char*>(buffer));
				ptr += strlen(ptr);
				break;

			case TYPE_INT16:
				sprintf(ptr, "\'%d\'", *static_cast<short*>(buffer));
				ptr += strlen(ptr);
				break;

			case TYPE_INT32:
				sprintf(ptr, "\'%d\'", *static_cast<int*>(buffer));
				ptr += strlen(ptr);
				break;

			case TYPE_INT64:
				sprintf(ptr, "\'%I64d\'", *static_cast<long long int*>(buffer));
				ptr += strlen(ptr);
				break;

			case TYPE_CHAR: {
				char *sqlEscapedStr = strreplace(static_cast<char*>(buffer), '\'', "\'\'");
				sprintf(ptr, "\'%.*s\'", strlen(sqlEscapedStr), sqlEscapedStr);
				ptr += strlen(ptr);
				free(sqlEscapedStr);
				break;
			}

			case TYPE_FLOAT32:
				sprintf(ptr, "\'%f\'", *static_cast<float*>(buffer));
				ptr += strlen(ptr);
				break;

			case TYPE_FLOAT64:
				sprintf(ptr, "\'%lf\'", *static_cast<double*>(buffer));
				ptr += strlen(ptr);
				break;

			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR: {
				char *sqlEscapedStr = strreplace(static_cast<char*>(buffer), '\'', "\'\'");
				sprintf(ptr, "\'%.*s\'", strlen(sqlEscapedStr), sqlEscapedStr);
				ptr += strlen(ptr);
				free(sqlEscapedStr);
				break;
			}

			case TYPE_DECIMAL: {
				register int initialVal, intVal, decimalVal, tenPow;
				tenPow = static_cast<int>(pow((float)10, row->getDataIndex(curCol))+0.5);
				initialVal = *static_cast<int*>(buffer);
				intVal = static_cast<int>(fabs((float)static_cast<int>(initialVal / tenPow))+0.5);
				decimalVal = static_cast<int>(fabs((float)static_cast<int>(initialVal % tenPow))+0.5);
				sprintf(ptr, "\'%c%d.%0*d\'", (initialVal < 0)? '-' : ' ', intVal, row->getDataIndex(curCol), decimalVal);
				ptr += strlen(ptr);
				break;
			}
		}
	}
	strcpy(ptr, ");\n");

	fputs(query, outputFile);

	return 0;
}

bool SQLFileSource::hasNext() {
	return false;
}

} //namespace
