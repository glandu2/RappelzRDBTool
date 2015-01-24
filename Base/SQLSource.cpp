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
#include "ILog.h"

static void printOdbcStatus(ILog::Level errorLevel, SQLSMALLINT type, SQLHANDLE handle) {
	SQLSMALLINT i = 0, len;
	SQLINTEGER native;
	SQLCHAR state[7];
	SQLCHAR text[256];
	SQLRETURN ret;

	do {
		ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len);
		if (SQL_SUCCEEDED(ret))
			getLogger()->log(errorLevel, "SQL: %s:%d:%ld:%s\n", state, i, (long)native, text);
	} while(ret == SQL_SUCCESS);
}


namespace RappelzRDBBase {

int SQLSource::checkSqlResult(int result, const char* functionName, bool ignoreError) {
	if(result != SQL_SUCCESS && result != SQL_NO_DATA && result != SQL_NEED_DATA) {
		ILog::Level level = result == SQL_SUCCESS_WITH_INFO? ILog::LL_Info : ILog::LL_Error;

		getLogger()->log(level, "SQL: %s %s(%d):\n", functionName, result == SQL_SUCCESS_WITH_INFO? "additional info" : "error", result);
		getLogger()->log(level, "SQL: query: %s\n", query);
		printOdbcStatus(level, SQL_HANDLE_STMT, hstmt);
		printOdbcStatus(level, SQL_HANDLE_DBC, hdbc);
		printOdbcStatus(level, SQL_HANDLE_ENV, henv);
		if(!SQL_SUCCEEDED(result)) {
			if(!ignoreError)
				commitTransaction = false;
			return ENOEXEC;
		}
	}

	return 0;
}

SQLSource::SQLSource(SQLLanguage *language) {
	henv = 0;
	hdbc = 0;
	hstmt = 0;
	tableName = 0;
	sqlLanguage = language;
	utf16To8bits = 0;
	reuseTableSchema = false;
	query[0] = 0;
}

SQLSource::~SQLSource() {
	close();
	if(tableName) free(tableName);
	delete sqlLanguage;
}

int SQLSource::open(const char* source, eOpenMode openMode, const char *location, const char* options) {
	IDataSource::open(source, openMode, location, options);

	SQLRETURN result;
	const char* connectionString = location;

	if(options) {
		char key[20];
		char value[80];
		const char* p = options;

		while((p = getNextOption(p, key, value))) {
			if(!strcmp(key, "charset")) {
				getLogger()->log(ILog::LL_Debug, "SQLSource: Using target charset \"%s\"\n", value);
				utf16To8bits = createCharsetConverter(value);
			} else if(!strcmp(key, "reusetable")) {
				reuseTableSchema = true;
			}
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
	if(checkSqlResult(SQLDriverConnect(hdbc, 0, (UCHAR*)connectionString, SQL_NTS, 0, 0, 0, 0), "SQLDriverConnect")) {
		getLogger()->log(ILog::LL_Error, "Connection string: \"%s\"\n", connectionString);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		hdbc = 0;
		henv = 0;
		return ENXIO;
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
	int rowCount = 0;

	//retreive row count
	sprintf(query, "SELECT COUNT(*) FROM %s;", tableName);
	if(checkSqlResult(SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS), "SQLExecDirect"))
		return ENOENT;

	if(checkSqlResult(SQLGetData(hstmt, 1, SQL_C_LONG, &rowCount, sizeof(int), NULL), "SQLGetData"))
		return ENOENT;
	setRowNumber(rowCount);

	SQLCloseCursor(hstmt);

	columnsToProcess.clear();
	for(int curCol=0; curCol<row->getColumnCount(); curCol++) {
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE) || row->getType(curCol) == TYPE_VARCHAR_SIZE)
			continue;

		columnsToProcess.push_back(curCol);
	}

	prepareReadQuery();

	if(checkSqlResult(SQLPrepare(hstmt, (SQLCHAR*)query, SQL_NTS), "SQLPrepare"))
		return ENOEXEC;

	if(checkSqlResult(SQLExecute(hstmt), "SQLExecute"))
		return EINVAL;


	//rows are pre-fetched
	if(!SQL_SUCCEEDED(SQLFetch(hstmt))) {
		endOfRecordSet = true;
	} else endOfRecordSet = false;

	return 0;
}

int SQLSource::prepareWrite(IRowManipulator *row, unsigned int rowCount) {
	clearBoundParameters(row->getColumnCount());

	if(reuseTableSchema) {
		SQLRETURN result;

		sprintf(query, "DELETE FROM %s;", tableName);
		if(checkSqlResult(SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS), "SQLExecDirect")) {
			SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
			return ENOENT;
		}

		std::vector<char*> dotPositions;
		char columnName[128];
		SQLLEN cbColumnName;
		char *p = tableName;
		char *tableNamePart, *endOfDatabasePart;

		while(p) {
			p = strchr(p, '.');
			if(p) {
				dotPositions.push_back(p);
				p++;
			}
		}


		if(dotPositions.size() >= 1)
			tableNamePart = dotPositions[dotPositions.size()-1] + 1;
		else
			tableNamePart = tableName;


		if(dotPositions.size() >= 2)
			endOfDatabasePart = dotPositions[dotPositions.size()-2] + 1;
		else
			endOfDatabasePart = tableName;

		sprintf(query, "SELECT COLUMN_NAME FROM %.*sINFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = \'%s\' ORDER BY ORDINAL_POSITION ASC;", endOfDatabasePart - tableName, tableName, tableNamePart);
		if(checkSqlResult(SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS), "SQLExecDirect")) {
			SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
			return ENOEXEC;
		}

		result = SQLBindCol(hstmt, 1, SQL_C_CHAR, &columnName, sizeof(columnName), &cbColumnName);
		columnsToProcess.clear();
		while(SQL_SUCCEEDED(result = SQLFetch(hstmt))) {
			if(cbColumnName != SQL_NO_TOTAL && cbColumnName != SQL_NULL_DATA) {
				int index = row->getColumnIndex(columnName);
				if(index >= 0) {
					columnsToProcess.push_back(index);
				} else {
					getLogger()->log(ILog::LL_Error, "Unknown column name: %s\n", columnName);
					SQLBindCol(hstmt, 1, SQL_C_CHAR, NULL, 0, NULL);
					SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
					return ESPIPE;
				}
			}
		}
		SQLBindCol(hstmt, 1, SQL_C_CHAR, NULL, 0, NULL);

		if(columnsToProcess.size() == 0) {
			getLogger()->log(ILog::LL_Error, "No columns ! Query: %s\n", query);
			SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
			return ENOEXEC;
		}

		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
	} else {
		sprintf(query, "DROP TABLE %s;", tableName);
		SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);

		columnsToProcess.clear();
		for(int curCol=0; curCol<row->getColumnCount(); curCol++) {
			if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_SQLIGNORE) || row->getType(curCol) == TYPE_VARCHAR_SIZE)
				continue;

			columnsToProcess.push_back(curCol);
		}

		if(createSQLTable(hstmt, tableName)) {
			SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
			return ENOENT;
		}

		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
	}

	prepareWriteQuery();

	int result = SQLPrepare(hstmt, (SQLCHAR*)query, SQL_NTS);
	if(checkSqlResult(result, "SQLPrepare"))
		return ENOEXEC;

	return 0;
}

int SQLSource::createSQLTable(SQLHSTMT hstmt, const char *table) {
	char *p;
	int isFirstColumn = 1, curCol;
	IRowManipulator *row = getRowManipulator();
	size_t i;

	p = query;
	sprintf(p, "CREATE TABLE %s (", table);
	p += strlen(p);

	for(i = 0; i < columnsToProcess.size(); i++) {
		curCol = columnsToProcess[i];

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
	if(checkSqlResult(result, "SQLExecDirect"))
		return ENOENT;

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
	if(checkSqlResult(result, "SQLExecute")) {
		return EILSEQ;
	}

	return 0;
}

char* SQLSource::appendColumnNames(char *p) {
	bool isFirstColumn = true;
	size_t i;
	IRowManipulator *row = getRowManipulator();

	for(i = 0; i < columnsToProcess.size(); i++) {
		if(isFirstColumn) {
			isFirstColumn = false;
		} else {
			strcpy(p, ", ");
			p += strlen(p);
		}
		p += sprintf(p, "\"%s\"", row->getColumnName(columnsToProcess[i]));
	}

	return p;
}

int SQLSource::prepareReadQuery() {
	char *p = query;

	strcpy(p, "SELECT ");
	p += strlen(p);

	p = appendColumnNames(p);

	sprintf(p, " FROM %s", tableName);
	p += strlen(p);

	size_t i;
	IRowManipulator *row = getRowManipulator();

	for(i = 0; i < columnsToProcess.size(); i++) {
		if(row->getFlags(columnsToProcess[i]) & TYPE_FLAG_SORT) {
			sprintf(p, " ORDER BY \"%s\" ASC", row->getColumnName(columnsToProcess[i]));
			p += strlen(p);
			break;
		}
	}

	sprintf(p, ";");

	return 0;
}

int SQLSource::prepareWriteQuery() {
	char *p = query;
	int curCol;
	size_t i;
	bool isFirstColumn = true;
	IRowManipulator *row = getRowManipulator();

	p += sprintf(p, "INSERT INTO %s (", tableName);

	p = appendColumnNames(p);

	strcpy(p, ") VALUES (");
	p += strlen(p);

	for(i = 0; i < columnsToProcess.size(); i++) {
		curCol = columnsToProcess[i];

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
				dbType = SQL_WVARCHAR;
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
				dbType = SQL_WVARCHAR;
				columnSize = row->getMaxDataCount(curCol);
				bufferSize = columnSize*4; //worst case: each UTF16 character is 4 bytes wide
				break;
		}
		ParameterInfo* paramInfo = initParameter(curCol, bufferSize);
		int result = SQLBindParameter(hstmt, i+1, SQL_PARAM_INPUT, columnType, dbType, columnSize, precision, paramInfo->data, 0, &paramInfo->dataSizeOrInd);
		if(checkSqlResult(result, "SQLBindParameter")) {
			getLogger()->log(ILog::LL_Error, "Error while binding column \"%s\"\n", row->getColumnName(curCol));
			return ENOEXEC;
		}
	}

	strcpy(p, ");");
	p += strlen(p);

	return 0;
}

int SQLSource::prepareWriteRowQuery() {
	IRowManipulator *row = getRowManipulator();
	void *buffer;
	int curCol, count;
	size_t i;

	for(i = 0; i < columnsToProcess.size(); i++) {
		curCol = columnsToProcess[i];

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
	size_t i;
	SQLLEN dataSize;
	SQLLEN isDataNull;

	for(i = 0; i < columnsToProcess.size(); i++) {
		curCol = columnsToProcess[i];
		columnIndex = i + 1;

		//Special handling for varchar
		if(row->getType(curCol) != TYPE_VARCHAR_STR && row->getType(curCol) != TYPE_NVARCHAR_STR)
			row->initData(curCol);

		buffer = row->getValuePtr(curCol);

		switch(row->getType(curCol)) {
			case TYPE_BIT:
				SQLGetData(hstmt, columnIndex, SQL_C_BIT, buffer, sizeof(char), &isDataNull);
				if(isDataNull == SQL_NULL_DATA) *static_cast<char*>(buffer) = 0;
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
				char decimalTemp[20];
				SQLGetData(hstmt, columnIndex, SQL_C_CHAR, decimalTemp, 19, &isDataNull);
				if(isDataNull == SQL_NULL_DATA) *static_cast<int*>(buffer) = 0;
				else {
					float decimalValue;
					sscanf(decimalTemp, "%f", &decimalValue);
					decimalValue = decimalValue*pow((float)10, row->getDataIndex(curCol));
					if(decimalValue < 0)
						decimalValue -= 0.5;
					else
						decimalValue += 0.5;
					*static_cast<int*>(buffer) = (int)decimalValue;
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

			case TYPE_CHAR:
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
					if(row->getType(curCol) != TYPE_CHAR)
						row->initData(curCol, 1);
					*static_cast<char*>(row->getValuePtr(curCol)) = 0;
				} else {
					char *outBuffer;
					ICharsetConverter::ConvertedString in, out;
					in.data = unicodeBuffer;
					in.size = bytesRead;
					utf16To8bits->convertFromUtf16(in, &out);

					if(row->getType(curCol) == TYPE_CHAR) {
						//Cap max size
						if(out.size > row->getDataCount(curCol))
							out.size = row->getDataCount(curCol);
					} else { //Varchar is autosized
						if(out.size > 0 && out.data[out.size-1] == 0)
							row->initData(curCol, out.size);
						else
							row->initData(curCol, out.size+1);
					}
					outBuffer = static_cast<char*>(row->getValuePtr(curCol));
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
	for(size_t i = 0; i < boundBuffers.size(); i++) {
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
