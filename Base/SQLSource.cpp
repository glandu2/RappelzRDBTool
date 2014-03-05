#include "SQLSource.h"
#include "RowManipulator.h"
#include "SQLLanguage.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "ICharsetConverter.h"

void printOdbcStatus(SQLSMALLINT type, HSTMT hstmt) {
	SQLCHAR     buffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLCHAR     sqlstate[SQL_SQLSTATE_SIZE + 1];
	SQLINTEGER  sqlcode;
	SQLSMALLINT length;
	int i=1;
	while ( SQLGetDiagRec( type, hstmt, i, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length) == SQL_SUCCESS ) {
		printf("SQLSTATE: %s\n", sqlstate);
		printf("Native Error Code: %ld\n", sqlcode);
		printf("buffer: %s \n\n", buffer);
		i++;
	}
}

namespace RappelzRDBBase {

char *SQLSource::strreplace(const char *input, char c, const char *rep) {
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

SQLSource::SQLSource(SQLLanguage *language) {
	henv = 0;
	hdbc = 0;
	hstmt = 0;
	tableName = 0;
	sqlLanguage = language;
	outputFile = 0;
	fileOutputMode = false;
	utf16To8bits = 0;
	setlocale(LC_ALL, "C");
}

SQLSource::~SQLSource() {
	close();
	if(tableName) free(tableName);
	delete sqlLanguage;
}

int SQLSource::open(const char* source, eOpenMode openMode,  const char* location, const char* user, const char* password, const char* options) {
	IDataSource::open(source, openMode, location, user, password);


	if(options && strstr(options, "output-file")) {
		if(openMode != OM_Write)	//only write sql file supported
			return ENOSYS;

		fileOutputMode = true;
		outputFile = fopen(source, "w");
		if(!outputFile)
			return errno;
	} else {						//directly send instructions to database via odbc
		SQLRETURN result;

		fileOutputMode = false;

		if(options) {
			const char *p = strstr(options, "charset=");
			if(p) {
				char targetCharset[32] = {0};
				p += 8;
				const char *end = strchr(p, ';');
				if(!end || end - p > 31) {
					strncpy(targetCharset, p, 31);
				} else {
					strncpy(targetCharset, p, end - p);
				}
				targetCharset[31] = 0;

				fprintf(stderr, "Using target charset \"%s\"\n", targetCharset);
				utf16To8bits = createCharsetConverter(targetCharset);
			}
		}
		if(utf16To8bits == 0)
			utf16To8bits = createCharsetConverter("");
		if(utf16To8bits == 0)
			utf16To8bits = createCharsetConverter("CP1252");

		result = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
		if(!SQL_SUCCEEDED(result)) return ENOSYS;

		result = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
		if(!SQL_SUCCEEDED(result)) {
			SQLFreeHandle(SQL_HANDLE_ENV, henv);
			henv = 0;
			return ENOSYS;
		}

		SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
		result = SQLConnect(hdbc, (UCHAR*)location, SQL_NTS, (UCHAR*)user, SQL_NTS, (UCHAR*)password, SQL_NTS);
		if(!SQL_SUCCEEDED(result)) {
			printOdbcStatus(SQL_HANDLE_DBC, hdbc);
			printf("%d\n", result);
			SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, henv);
			hdbc = 0;
			henv = 0;
			return ENOENT;
		}

		SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		if(openMode == OM_Write) {
			SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
			commitTransaction = true;
		} else SQLSetConnectAttr(hdbc, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TXN_SERIALIZABLE, SQL_IS_UINTEGER);	//consitent results with select count(*) and select *
	}

	tableName = strdup(source);

	return 0;
}

void SQLSource::close() {
	if(fileOutputMode) {
		if(outputFile) fclose(outputFile);
		outputFile = 0;
	} else {
		if(getOpenMode() == OM_Write && hdbc) {
			if(commitTransaction)
				SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
			else SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
		}

		if(hstmt) SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		if(hdbc) SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		if(henv) SQLFreeHandle(SQL_HANDLE_ENV, henv);
		hstmt = 0;
		hdbc = 0;
		henv = 0;
	}

	if(utf16To8bits)
		delete utf16To8bits;
	utf16To8bits = 0;
	if(tableName) free(tableName);
	tableName = 0;
}

int SQLSource::prepareRead(IRowManipulator *row) {
	int rowCount;

	//retreive row count
	sprintf(query, "SELECT COUNT(*) FROM %s;", tableName);
	SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if(!SQL_SUCCEEDED(SQLFetch(hstmt))) {
		printOdbcStatus(SQL_HANDLE_STMT, hstmt);
		return EINVAL;
	}
	SQLGetData(hstmt, 1, SQL_C_LONG, &rowCount, sizeof(int), NULL);
	setRowNumber(rowCount);

	SQLCloseCursor(hstmt);

	prepareReadQuery();

	printf("Quering: \"%s\"\n", query);
	if(!SQL_SUCCEEDED(SQLPrepare(hstmt, (SQLCHAR*)query, SQL_NTS))) {
		printOdbcStatus(SQL_HANDLE_STMT, hstmt);
		return ENOEXEC;
	}
	if(!SQL_SUCCEEDED(SQLExecute(hstmt))) {
		printOdbcStatus(SQL_HANDLE_STMT, hstmt);
		return ENOEXEC;
	}


	//rows are pre-fetched
	if(!SQL_SUCCEEDED(SQLFetch(hstmt))) {
		endOfRecordSet = true;
	} else endOfRecordSet = false;

	return 0;
}

int SQLSource::prepareWrite(IRowManipulator *row, unsigned int rowCount) {
	char *ptr = query;
	bool isFirstColumn = true;

	sprintf(query, "DROP TABLE %s;", tableName);
	if(fileOutputMode) {
		fputs(query, outputFile);
	} else {
		SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
	}

	if(createSQLTable(hstmt, tableName))
		return EACCES;

	if(!fileOutputMode)
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

	prepareWriteQuery();

	return 0;
}

int SQLSource::createSQLTable(SQLHSTMT hstmt, const char *table) {
	char query[8192];
	char *p;
	int isFirstColumn = 1, curCol;
	IRowManipulator *row = getRowManipulator();

	p = query;
	sprintf(p, "CREATE TABLE %s (", table);
	p += strlen(p);

	for(curCol=0; curCol<row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE)) continue;

		if(isFirstColumn)
			isFirstColumn = 0;
		else {
			strcpy(p, ",");
			p += strlen(p);
		}

		sprintf(p, "%s ", row->getColumnName(curCol));
		p += strlen(p);

		sqlLanguage->getTypeName(p, row->getType(curCol), row->getMaxDataCount(curCol), row->getDataIndex(curCol));
		p += strlen(p);

		strcpy(p, " NOT NULL");
		p += strlen(p);

		if(GET_FLAGBIT(row->getFlags(curCol),TYPE_FLAG_KEY)) {
			strcpy(p, " PRIMARY KEY");
			p += strlen(p);
		}
	}
	strcpy(p, ");");

	if(fileOutputMode) {
		fputs(query, outputFile);
		return ferror(outputFile) == 0;
	} else {
		int result = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
		if(!SQL_SUCCEEDED(result) && result != SQL_NO_DATA) {
			fprintf(stderr, "SQL Query: %s\n", query);
			printOdbcStatus(SQL_HANDLE_STMT, hstmt);
			commitTransaction = false;
			return EILSEQ;
		}
		return 0;
	}
}

int SQLSource::readRow() {
	prepareReadRowQuery(hstmt);

	if(!SQL_SUCCEEDED(SQLFetch(hstmt)))
		endOfRecordSet = true;
	else endOfRecordSet = false;

	return 0;
}

int SQLSource::writeRow() {
	int result;

	result = prepareWriteRowQuery();
	if(result != 0) {
		commitTransaction = false;
		return result;
	}

	if(fileOutputMode) {
		fputs(query, outputFile);
	} else {
		int result = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
		if(!SQL_SUCCEEDED(result) && result != SQL_NO_DATA) {
			fprintf(stderr, "SQL Query: %s\n", query);
			printOdbcStatus(SQL_HANDLE_STMT, hstmt);
			commitTransaction = false;
			return EILSEQ;
		}
	}

	return 0;
}

int SQLSource::prepareReadQuery() {
	char *p = query;
	bool isFirstColumn = true;
	int i;
	IRowManipulator *row = getRowManipulator();

	strcpy(p, "SELECT ");
	p += strlen(p);

	for(i=0; i<row->getColumnCount(); i++) {
		if(GET_FLAGBIT(row->getIgnoreType(i), TYPE_SQLIGNORE)) continue;

		if(isFirstColumn)
			isFirstColumn = false;
		else {
			strcpy(p, ", ");
			p += strlen(p);
		}
		sprintf(p, "%s", row->getColumnName(i));
		p += strlen(p);
	}
	sprintf(p, " FROM %s;", tableName);

	return 0;
}

int SQLSource::prepareWriteQuery() {
	char *ptr = query;
	int curCol;
	bool isFirstColumn = true;
	IRowManipulator *row = getRowManipulator();

	sprintf(ptr, "INSERT INTO %s (", tableName);
	ptr += strlen(ptr);

	for(curCol=0; curCol<row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE)) continue;

		if(isFirstColumn)
			isFirstColumn = false;
		else {
			strcpy(ptr, ",");
			ptr += strlen(ptr);
		}
//		strcpy(ptr, "\"");
//		ptr++;
		strcpy(ptr, row->getColumnName(curCol));
		ptr += strlen(ptr);
//		strcpy(ptr, "\"");
//		ptr++;
	}
	strcpy(ptr, ") VALUES (");
	ptr += strlen(ptr);

	endOfHeader = ptr;
	return 0;
}

int SQLSource::prepareWriteRowQuery() {
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
	strcpy(ptr, ");");

	return 0;
}

int SQLSource::prepareReadRowQuery(SQLHSTMT hstmt) {
	IRowManipulator *row = getRowManipulator();
	void *buffer;
	int curCol, columnIndex, dummy;
	SQLLEN dataSize;
	SQLLEN isDataNull;

	for(curCol=0, columnIndex=0; curCol<row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE)) continue;
		columnIndex++;	//do not count ignored columns

		//Special handling for varchar
		if(row->getType(curCol) != TYPE_VARCHAR_STR)
			row->initData(curCol);

		buffer = row->getValuePtr(curCol);

		switch(row->getType(curCol)) {
			case TYPE_BIT:
				SQLGetData(hstmt, columnIndex, SQL_C_DEFAULT, buffer, sizeof(char), &isDataNull);
				if(isDataNull == SQL_NULL_DATA) *static_cast<char*>(buffer) = 0;
				break;

			case TYPE_CHAR:
				SQLGetData(hstmt, columnIndex, SQL_C_CHAR, buffer, sizeof(char)*row->getDataCount(curCol), &isDataNull);
				if(isDataNull == SQL_NULL_DATA)
					*static_cast<char*>(buffer) = '\0';
				break;

			case TYPE_INT8:
				SQLGetData(hstmt, columnIndex, SQL_C_TINYINT, buffer, sizeof(char), &isDataNull);
				if(isDataNull == SQL_NULL_DATA) *static_cast<char*>(buffer) = 0;
				break;

			case TYPE_INT16:
				SQLGetData(hstmt, columnIndex, SQL_C_SHORT, buffer, sizeof(short), &isDataNull);
				if(isDataNull == SQL_NULL_DATA) *static_cast<short*>(buffer) = 0;
				break;

			case TYPE_INT32:
				SQLGetData(hstmt, columnIndex, SQL_C_LONG, buffer, sizeof(int), &isDataNull);
				if(isDataNull == SQL_NULL_DATA) *static_cast<int*>(buffer) = 0;
				break;

			case TYPE_DECIMAL:
			{
				char decimalTemp[14];
				SQLGetData(hstmt, columnIndex, SQL_C_CHAR, decimalTemp, 13, &isDataNull);
				if(isDataNull == SQL_NULL_DATA) *static_cast<int*>(buffer) = 0;
				else {
					float decimalValue;
					sscanf(decimalTemp, "%f", &decimalValue);
					*static_cast<int*>(buffer) = (int)(decimalValue*pow((float)10, row->getDataIndex(curCol))+0.5);
				}
				break;
			}

			case TYPE_INT64:
				SQLGetData(hstmt, columnIndex, SQL_C_DEFAULT, buffer, sizeof(long long int), &isDataNull);	//using SQL_C_DEFAULT as SQL_C_SBIGINT has some problem with some odbc implementation
				if(isDataNull == SQL_NULL_DATA)
					*static_cast<long long int*>(buffer) = 0;
				break;

			case TYPE_FLOAT32:
				SQLGetData(hstmt, columnIndex, SQL_C_FLOAT, buffer, sizeof(float), &isDataNull);
				if(isDataNull == SQL_NULL_DATA)
					*static_cast<float*>(buffer) = 0;
				break;

			case TYPE_FLOAT64:
				SQLGetData(hstmt, columnIndex, SQL_C_DOUBLE, buffer, sizeof(double), &isDataNull);
				if(isDataNull == SQL_NULL_DATA)
					*static_cast<double*>(buffer) = 0;
				break;

			case TYPE_VARCHAR_SIZE:
				break;

			case TYPE_VARCHAR_STR: {
				char *unicodeBuffer;
				int bufferSize;
				int bytesRead = 0;

				SQLGetData(hstmt, columnIndex, SQL_C_BINARY, &dummy, 0, &dataSize);
				if(dataSize % 2 == 0)
					bufferSize = dataSize+2;
				else
					bufferSize = dataSize+1;
				unicodeBuffer = (char*)malloc(bufferSize);

				int ret;
				while((ret = SQLGetData(hstmt, columnIndex, SQL_C_WCHAR, unicodeBuffer + bytesRead, bufferSize - bytesRead, &isDataNull)) == SQL_SUCCESS_WITH_INFO) {
					if(isDataNull == SQL_NULL_DATA)
						break;

					bytesRead = bufferSize-2; //dont keep null terminator
					bufferSize *= 2;
					unicodeBuffer = (char*) realloc(unicodeBuffer, bufferSize);
				}
				if(ret == SQL_SUCCESS)
					bytesRead += isDataNull;

				if(isDataNull == SQL_NULL_DATA || bytesRead == 0) {
					row->initData(curCol, 1);
					*static_cast<char*>(row->getValuePtr(curCol)) = 0;
				} else {

					ICharsetConverter::ConvertedString in, out;
					in.data = unicodeBuffer;
					in.size = bytesRead;
					utf16To8bits->convertFromUtf16(in, &out);

					if(out.size > 0 && out.data[out.size-1] == 0)
						row->initData(curCol, out.size);
					else
						row->initData(curCol, out.size+1);
					char *outBuffer = static_cast<char*>(row->getValuePtr(curCol));
					memcpy(outBuffer, out.data, out.size);
					free(out.data);
				}

				free(unicodeBuffer);
				break;
			}
		}
	}
	return 0;
}

bool SQLSource::hasNext() {
	return !endOfRecordSet;
}

} //namespace
