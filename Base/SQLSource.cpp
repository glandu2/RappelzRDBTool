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

static void printOdbcStatus(SQLSMALLINT type, HSTMT hstmt) {
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

SQLSource::SQLSource(SQLLanguage *language) {
	henv = 0;
	hdbc = 0;
	hstmt = 0;
	tableName = 0;
	sqlLanguage = language;
	utf16To8bits = 0;
	setlocale(LC_ALL, "C");
}

SQLSource::~SQLSource() {
	close();
	if(tableName) free(tableName);
	delete sqlLanguage;
}

int SQLSource::checkSqlResult(int result, const char* functionName) {
	if(!SQL_SUCCEEDED(result) && result != SQL_NO_DATA && result != SQL_NEED_DATA) {
		fprintf(stderr, "%s: SQL Error %d\n", functionName, result);
		fprintf(stderr, "SQL Query: %s\n", query);
		printOdbcStatus(SQL_HANDLE_STMT, hstmt);
		printOdbcStatus(SQL_HANDLE_DBC, hdbc);
		printOdbcStatus(SQL_HANDLE_ENV, henv);
		commitTransaction = false;
		return EILSEQ;
	}

    return 0;
}

int SQLSource::open(const char* source, eOpenMode openMode,  const char* location, const char* user, const char* password, const char* options) {
	IDataSource::open(source, openMode, location, user, password);

	SQLRETURN result;

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

	tableName = strdup(source);

	return 0;
}

void SQLSource::close() {
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

	clearBoundParameters(0);

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
	if(checkSqlResult(SQLFetch(hstmt), "prepareRead.SQLFetch"))
		return EINVAL;

	SQLGetData(hstmt, 1, SQL_C_LONG, &rowCount, sizeof(int), NULL);
	setRowNumber(rowCount);

	SQLCloseCursor(hstmt);

	prepareReadQuery();

	printf("Quering: \"%s\"\n", query);
	if(checkSqlResult(SQLPrepare(hstmt, (SQLCHAR*)query, SQL_NTS), "prepareRead.SQLPrepare"))
		return ENOEXEC;

	if(checkSqlResult(SQLExecute(hstmt), "prepareRead.SQLExecute"))
		return ENOEXEC;


	//rows are pre-fetched
	if(!SQL_SUCCEEDED(SQLFetch(hstmt))) {
		endOfRecordSet = true;
	} else endOfRecordSet = false;

	return 0;
}

int SQLSource::prepareWrite(IRowManipulator *row, unsigned int rowCount) {
	clearBoundParameters(row->getColumnCount());

	sprintf(query, "DROP TABLE %s;", tableName);
	SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

	if(createSQLTable(hstmt, tableName))
		return EACCES;

	SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

	prepareWriteQuery();

	int result = SQLPrepare(hstmt, (SQLCHAR*)query, SQL_NTS);
	if(checkSqlResult(result, "writeRow.SQLExecute"))
		return EILSEQ;

	return 0;
}

int SQLSource::createSQLTable(SQLHSTMT hstmt, const char *table) {
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

		sprintf(p, "\"%s\" ", row->getColumnName(curCol));
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

	int result = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if(checkSqlResult(result, "createSQLTable.SQLExecDirect"))
		return EILSEQ;

	return 0;
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

	prepareWriteRowQuery();

	result = SQLExecute(hstmt);
	if(checkSqlResult(result, "writeRow.SQLExecute"))
		return EILSEQ;

	return 0;
}

char* SQLSource::appendColumnNames(char *p) {
	bool isFirstColumn = true;
	int i;
	IRowManipulator *row = getRowManipulator();

	for(i=0; i<row->getColumnCount(); i++) {
		if(GET_FLAGBIT(row->getIgnoreType(i), TYPE_SQLIGNORE))
			continue;

		if(isFirstColumn) {
			isFirstColumn = false;
		} else {
			strcpy(p, ", ");
			p += strlen(p);
		}
		p += sprintf(p, "\"%s\"", row->getColumnName(i));
	}

	return p;
}

int SQLSource::prepareReadQuery() {
	char *p = query;

	strcpy(p, "SELECT ");
	p += strlen(p);

	p = appendColumnNames(p);

	sprintf(p, " FROM %s;", tableName);

	return 0;
}

int SQLSource::prepareWriteQuery() {
	char *p = query;
	int i, curCol;
	bool isFirstColumn = true;
	IRowManipulator *row = getRowManipulator();

	p += sprintf(p, "INSERT INTO %s (", tableName);

	p = appendColumnNames(p);

	strcpy(p, ") VALUES (");
	p += strlen(p);

	for(i=1, curCol=0; curCol<row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE) || row->getType(curCol) == TYPE_VARCHAR_SIZE)
			continue;

		if(isFirstColumn) {
			isFirstColumn = false;
		} else {
			strcpy(p, ", ");
			p += 2;
		}
		strcpy(p, "?");
		p++;

		SQLSMALLINT columnType = SQL_C_DEFAULT, dbType = SQL_UNKNOWN_TYPE, precision = 0;
		SQLULEN columnSize = 1;
		SQLLEN bufferSize = 0;

		switch(row->getType(curCol)) {
			case TYPE_BIT:
				columnType = SQL_C_DEFAULT;
				dbType = SQL_BIT;
				bufferSize = 1;
				break;

			case TYPE_CHAR:
				columnType = SQL_C_WCHAR;
				dbType = SQL_WLONGVARCHAR;
				columnSize = row->getMaxDataCount(curCol);
				bufferSize = columnSize*4; //worst case: each UTF16 character is 4 bytes wide
				break;

			case TYPE_INT8:
				columnType = SQL_C_TINYINT;
				dbType = SQL_TINYINT;
				columnSize = 1;
				bufferSize = 1;
				break;

			case TYPE_INT16:
				columnType = SQL_C_SHORT;
				dbType = SQL_SMALLINT;
				bufferSize = 2;
				break;

			case TYPE_INT32:
				columnType = SQL_C_LONG;
				dbType = SQL_INTEGER;
				bufferSize = 4;
				break;

			case TYPE_DECIMAL:
				columnType = SQL_C_DOUBLE;
				dbType = SQL_DECIMAL;
				columnSize = 10;
				precision = row->getDataIndex(curCol);
				bufferSize = 8;
				break;

			case TYPE_INT64:
				columnType = SQL_C_SBIGINT;
				dbType = SQL_BIGINT;
				bufferSize = 8;
				break;

			case TYPE_FLOAT32:
				columnType = SQL_C_FLOAT;
				dbType = SQL_REAL;
				bufferSize = 4;
				break;

			case TYPE_FLOAT64:
				columnType = SQL_C_DOUBLE;
				dbType = SQL_DOUBLE;
				bufferSize = 8;
				break;

			case TYPE_VARCHAR_SIZE:
				break;

			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR:
				columnType = SQL_C_WCHAR;
				dbType = SQL_WLONGVARCHAR;
				columnSize = row->getMaxDataCount(curCol);
				bufferSize = columnSize*4; //worst case: each UTF16 character is 4 bytes wide
				break;
		}
		ParameterInfo* paramInfo = initParameter(curCol, bufferSize);
		int result = SQLBindParameter(hstmt, i, SQL_PARAM_INPUT, columnType, dbType, columnSize, precision, paramInfo->data, 0, &paramInfo->dataSizeOrInd);
		if(checkSqlResult(result, "prepareWriteQuery.SQLBindParameter"))
			return EILSEQ;
		i++;
	}

	strcpy(p, ");");
	p += strlen(p);

	return 0;
}

int SQLSource::prepareWriteRowQuery() {
	IRowManipulator *row = getRowManipulator();
	void *buffer;
	int curCol, count;

	for(curCol = 0; curCol < row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE) || row->getType(curCol) == TYPE_VARCHAR_SIZE)
			continue;

		count = row->getDataCount(curCol);
		buffer = row->getValuePtr(curCol);

		switch(row->getType(curCol)) {
			case TYPE_BIT:
			case TYPE_INT8:
				setParameterData(curCol, buffer, 1);
				break;

			case TYPE_INT16:
				setParameterData(curCol, buffer, 2);
				break;

			case TYPE_INT32:
				setParameterData(curCol, buffer, 4);
				break;

			case TYPE_INT64:
				setParameterData(curCol, buffer, 8);
				break;

			case TYPE_FLOAT32:
				setParameterData(curCol, buffer, 4);
				break;

			case TYPE_FLOAT64:
				setParameterData(curCol, buffer, 8);
				break;

			case TYPE_CHAR:
			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR: {
				ICharsetConverter::ConvertedString in, out;
				in.data = (char*)buffer;
				in.size = strlen((char*)buffer);
				utf16To8bits->convertToUtf16(in, &out);

				setParameterData(curCol, out.data, out.size);
				free(out.data);
				break;
			}

			case TYPE_DECIMAL: {
				double initialVal, val, tenPow;
				tenPow = pow(10.0, row->getDataIndex(curCol));
				initialVal = *static_cast<int*>(buffer);
				val = initialVal / tenPow;
				setParameterData(curCol, &val, 8);
				break;
			}
		}
	}


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
		if(row->getType(curCol) != TYPE_VARCHAR_STR && row->getType(curCol) != TYPE_NVARCHAR_STR)
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

			case TYPE_NVARCHAR_STR:
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


/***********************************/
/*** Parameter binding functions ***/
/***********************************/

void SQLSource::clearBoundParameters(int newCount) {
	if(hstmt)
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
	for(int i = 0; i < boundBuffers.size(); i++) {
		if(boundBuffers[i].data)
			free(boundBuffers[i].data);
		boundBuffers[i].data = 0;
	}
	boundBuffers.resize(newCount);
}

SQLSource::ParameterInfo* SQLSource::initParameter(int index, SQLLEN dataSizeOrInd) {
	if(boundBuffers[index].data != 0)
		free(boundBuffers[index].data);

	if(dataSizeOrInd > 0) {
		boundBuffers[index].data = malloc(dataSizeOrInd);
		boundBuffers[index].dataSizeOrInd = 0;
	} else
		boundBuffers[index].dataSizeOrInd = dataSizeOrInd;

	return &boundBuffers[index];
}

void* SQLSource::getParameter(int index) {
	return boundBuffers[index].data;
}

SQLLEN* SQLSource::getParameterSizeOrInd(int index) {
	return &boundBuffers[index].dataSizeOrInd;
}

void SQLSource::setParameterData(int index, void *data, SQLLEN size) {
	memcpy(boundBuffers[index].data, data, size);
	boundBuffers[index].dataSizeOrInd = size;
}

} //namespace
