#include "RowManipulator.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <vector>

#include "Log.h"

namespace RappelzRDBBase {

//linear
RowManipulator::RowManipulator(const DataDescriptor* columnsType, FieldOrder *order, void **initValue) {
	int i;

	assert(columnsType != 0);

	this->row = initValue;
	this->columnsType = columnsType;
	this->columnsTypeOrdered.fieldsInfo = 0;
	this->columnsTypeOrdered.numFields = 0;
	this->order = 0;

	setFieldOrder(order);
	this->initializedFields = new bool[columnsType->numFields];

	for(i=0; i < columnsType->numFields; i++) {
		if(GET_TYPE(columnsType->fieldsInfo[i].dataType) == TYPE_VARCHAR_SIZE)
			this->columnsType->fieldsInfo[i].dataSize = MAKEINDEXEDVAR(GET_VAR_INDEX(columnsType->fieldsInfo[i].dataSize), 1);
	}

	if(this->row) initRow(true);
}

RowManipulator::~RowManipulator() {
	delete[] initializedFields;
}

//linear
void RowManipulator::initRow(bool initializedFields) {
	int i, columnCount;

	setLinearMode();

	columnCount = getColumnCount();
	for(i=0; i<columnCount; i++) {
		if(getType(i) == TYPE_VARCHAR_SIZE) {
			const unsigned int index = getDataIndex(i);
			if(this->indexedSizePtrs.size() <= index) this->indexedSizePtrs.resize(index+1, 0);
			this->indexedSizePtrs[index] = (int*)getValuePtr(i);
			if(initializedFields == false)
				*this->indexedSizePtrs[index] = 1; //minimal size
		}
		this->initializedFields[i] = initializedFields;
	}
	setOrderedMode();
}

//linear
int RowManipulator::allocRow() {
	row = (void**) malloc(columnsType->numFields*sizeof(void*));
	if(!row) return ENOMEM;

	initRow(false);
	return 0;
}

//ordered
int RowManipulator::initData(int colPos, unsigned int dataCount) {
	if(dataCount == 0)
		dataCount = getDataCount(colPos);
	setDataCount(colPos, dataCount);

	const unsigned int maxDataCount = getMaxDataCount(colPos);

	assert(dataCount != 0); //disallow 0 data field, if this happen, there is a conception error

	//To verify if malloc failed, set to 1 for no error when we don't use malloc,
	//&result is purely arbitrary, the value has to be non-zero
	void *result = &result;

	switch(getType(colPos)) {
		case TYPE_BIT:
			if(maxDataCount > (sizeof(void*)*8))
				result = row[getColumnOriginalIndex(colPos)] = malloc((dataCount+7)/8);
			break;

		case TYPE_NVARCHAR_STR:
		case TYPE_VARCHAR_STR:
		case TYPE_CHAR:
			if((maxDataCount+1) > (sizeof(void*)/sizeof(char))) {
				result = row[getColumnOriginalIndex(colPos)] = malloc((dataCount+1) * sizeof(char));
				if(result) memset(static_cast<char*>(result), 0, dataCount+1);	//fill with '\0' to prevent bugs when reading this field as a nul-terminated string
			}
			break;

		case TYPE_INT8:
			if(maxDataCount > (sizeof(void*)/sizeof(char)))
				result = row[getColumnOriginalIndex(colPos)] = malloc(dataCount * sizeof(char));
			break;

		case TYPE_INT16:
			if(maxDataCount > (sizeof(void*)/sizeof(short)))
				result = row[getColumnOriginalIndex(colPos)] = malloc(dataCount * sizeof(short));
			break;

		case TYPE_FLOAT32:
			if(maxDataCount > (sizeof(void*)/sizeof(float)))
				result = row[getColumnOriginalIndex(colPos)] = malloc(dataCount * sizeof(float));
			break;

		//size = register size = sizeof(int) so never allocate memory
		//case TYPE_VARCHAR_SIZE:
		//	break;

		case TYPE_DECIMAL:
			//if(maxDataCount > (sizeof(void*)/sizeof(int)))
			//	row[i] = malloc(dataCount * sizeof(int));
			//break;
			//same as TYPE_INT32 -> no "break;"

		case TYPE_INT32:
			if(maxDataCount > (sizeof(void*)/sizeof(int)))
				result = row[getColumnOriginalIndex(colPos)] = malloc(dataCount * sizeof(int));
			break;

		case TYPE_FLOAT64:
			if(maxDataCount > (sizeof(void*)/sizeof(double)))
				result = row[getColumnOriginalIndex(colPos)] = malloc(dataCount * sizeof(double));
			break;

		case TYPE_INT64:
			if(maxDataCount > (sizeof(void*)/sizeof(long long int)))
				result = row[getColumnOriginalIndex(colPos)] = malloc(dataCount * sizeof(long long int));
			break;
	}
	if(!result)
		return ENOMEM;

	this->initializedFields[getColumnOriginalIndex(colPos)] = true;

	return 0;
}

//linear
int RowManipulator::completeRowInit() {
	int i, result;
	setLinearMode();
	for(i=0; i<columnsType->numFields; i++) {
		if(this->initializedFields[i] == false) {
			result = initData(i);
			if(result) {
				setOrderedMode();
				return result;
			}
		}
	}
	setOrderedMode();
	return 0;
}

//linear
void RowManipulator::freeRow() {
	int colPos;

	setLinearMode();
	for(colPos=0; colPos<columnsType->numFields; colPos++) {
		freeValue(colPos);
	}
	setOrderedMode();
	free(row);
}

//ordered
void RowManipulator::freeValue(int colPos) {
	const unsigned int dataCount = getDataCount(colPos);

	colPos = getColumnOriginalIndex(colPos);

	if(this->initializedFields[colPos] == false) return;

	switch(GET_TYPE(columnsType->fieldsInfo[colPos].dataType)) {
		case TYPE_BIT:
			if(dataCount > (sizeof(void*)*8))
				free(row[colPos]);
			break;

		case TYPE_NVARCHAR_STR:
		case TYPE_VARCHAR_STR:
		case TYPE_INT8:
		case TYPE_CHAR:
			if(dataCount > (sizeof(void*)/sizeof(char)))
				free(row[colPos]);
			break;

		case TYPE_INT16:
			if(dataCount > (sizeof(void*)/sizeof(short)))
				free(row[colPos]);
			break;

		case TYPE_FLOAT32:
			if(dataCount > (sizeof(void*)/sizeof(float)))
				free(row[colPos]);
			break;

		//size = register size = sizeof(int) so never allocate memory
		//case TYPE_VARCHAR_SIZE:
		//	break;

		case TYPE_DECIMAL:
			//if(dataCount > (sizeof(void*)/sizeof(int)))
			//	free(row[colPos]);
			//break;
			//same as TYPE_INT32 -> no "break;"

		case TYPE_INT32:
			if(dataCount > (sizeof(void*)/sizeof(int)))
				free(row[colPos]);
			break;

		case TYPE_FLOAT64:
			if(dataCount > (sizeof(void*)/sizeof(double)))
				free(row[colPos]);
			break;

		case TYPE_INT64:
			if(dataCount > (sizeof(void*)/sizeof(long long int)))
				free(row[colPos]);
			break;
	}
	this->initializedFields[colPos] = false;
}


//ordered
void *RowManipulator::getValuePtr(int colPos) {
	if(colPos < 0 || colPos >= getColumnCount()) {
		getLogger()->log(ILog::LL_Error, "RowManipulator: Invalid column index: %d. Only has %d cols (index begin at 0 for first)", colPos, getColumnCount());
		return 0;
	}

	colPos = getColumnOriginalIndex(colPos);

	const unsigned int dataCount = GET_VAR_LNGTH(columnsType->fieldsInfo[colPos].dataSize);

	switch(GET_TYPE(columnsType->fieldsInfo[colPos].dataType)) {
		case TYPE_BIT:
			if(dataCount > (sizeof(void*)*8))
				return row[colPos];
			return &row[colPos];

		case TYPE_NVARCHAR_STR:
		case TYPE_VARCHAR_STR:
		case TYPE_INT8:
		case TYPE_CHAR:
			if(dataCount > (sizeof(void*)/sizeof(char)))
				return row[colPos];
			return &row[colPos];

		case TYPE_INT16:
			if(dataCount > (sizeof(void*)/sizeof(short)))
				return row[colPos];
			return &row[colPos];

		case TYPE_FLOAT32:
			if(dataCount > (sizeof(void*)/sizeof(float)))
				return row[colPos];
			return &row[colPos];

		case TYPE_VARCHAR_SIZE:
			return &row[colPos];

		case TYPE_DECIMAL:
			//if(dataCount > (sizeof(void*)/sizeof(int)))
			//	row[i] = malloc(dataCount * sizeof(int));
			//break;
			//same as TYPE_INT32 -> no "break;"

		case TYPE_INT32:
			if(dataCount > (sizeof(void*)/sizeof(int)))
				return row[colPos];
			return &row[colPos];

		case TYPE_FLOAT64:
			if(dataCount > (sizeof(void*)/sizeof(double)))
				return row[colPos];
			return &row[colPos];

		case TYPE_INT64:
			if(dataCount > (sizeof(void*)/sizeof(long long int)))
				return row[colPos];
			return &row[colPos];
	}

	return 0;
}

//ordered
int RowManipulator::getDataCount(int colPos) {
	if((getType(colPos) == TYPE_VARCHAR_STR || getType(colPos) == TYPE_NVARCHAR_STR) && *this->indexedSizePtrs[getDataIndex(colPos)])
		return *this->indexedSizePtrs[getDataIndex(colPos)];
	return getMaxDataCount(colPos);
}

//ordered
//If the value was already allocated, freeing and reinit is needed after that
void RowManipulator::setDataCount(int colPos, int dataCount) {
	if(getType(colPos) == TYPE_VARCHAR_STR || getType(colPos) == TYPE_NVARCHAR_STR)
		*this->indexedSizePtrs[getDataIndex(colPos)] = dataCount;
}

//ordered
//should be recoded to be more efficient, this version is somewhat slow
int RowManipulator::getColumnIndex(const char* columnName) {
	int index = -1;

	if(columnIndexByName.empty()) {
		for(int i=0; i < getColumnCount(); i++) {
			if(!strcmp(getColumnName(i), columnName))
				index = i;
			columnIndexByName.insert(make_pair(std::string(getColumnName(i)), i));
		}
	} else {
		auto it = columnIndexByName.find(std::string(columnName));
		if(it != columnIndexByName.end())
			index = it->second;
	}

	if(index == -1)
		getLogger()->log(ILog::LL_Error, "RowManipulator: Column named \"%s\" not found\n", columnName);
	return index;
}

void RowManipulator::setFieldOrder(FieldOrder *newOrder) {
	if(order)
		delete[] columnsTypeOrdered.fieldsInfo;

	order = newOrder;
	savedOrder = newOrder;

	if(newOrder) {
		columnsTypeOrdered.numFields = newOrder->size;
		columnsTypeOrdered.fieldsInfo = new FieldDescriptor[columnsTypeOrdered.numFields];
		for(int i = 0; i < newOrder->size; i++) {
			columnsTypeOrdered.fieldsInfo[i] = columnsType->fieldsInfo[getColumnOriginalIndex(i)];
		}
	} else {
		columnsTypeOrdered.fieldsInfo = columnsType->fieldsInfo;
		columnsTypeOrdered.numFields = columnsType->numFields;
	}

	savedColumnsTypeOrdered = columnsTypeOrdered;
}

void RowManipulator::setOrderedMode() {
	order = this->savedOrder;
	columnsTypeOrdered = this->savedColumnsTypeOrdered;
}

void RowManipulator::setLinearMode() {
	this->savedOrder = order;
	this->savedColumnsTypeOrdered = columnsTypeOrdered;
	order = 0;
	columnsTypeOrdered.fieldsInfo = columnsType->fieldsInfo;
	columnsTypeOrdered.numFields = columnsType->numFields;
}

static const char* getTypeName(int type) {
	switch(type) {
		case TYPE_BIT: return "TYPE_BIT";
		case TYPE_INT8: return "TYPE_INT8";
		case TYPE_INT16: return "TYPE_INT16";
		case TYPE_INT32: return "TYPE_INT32";
		case TYPE_INT64: return "TYPE_INT64";
		case TYPE_FLOAT32: return "TYPE_FLOAT32";
		case TYPE_FLOAT64: return "TYPE_FLOAT64";
		case TYPE_CHAR: return "TYPE_CHAR";
		case TYPE_VARCHAR_STR: return "TYPE_VARCHAR_STR";
		case TYPE_DECIMAL: return "TYPE_DECIMAL";
		case TYPE_NVARCHAR_STR: return "TYPE_NVARCHAR_STR";
		default: return "unknown";
	}
}

void* RowManipulator::checkAndGetColumnValuePtr(const char *columnName, int type, int size, int *columnIdx) {
	int columnIndex = getColumnIndex(columnName);
	if(columnIndex < 0 || columnIndex > getColumnCount()) {
		return 0;
	}
	bool isTypeOk = getType(columnIndex) == type ||
			((getType(columnIndex) == TYPE_VARCHAR_STR || getType(columnIndex) == TYPE_NVARCHAR_STR || getType(columnIndex) == TYPE_CHAR) &&
			(type == TYPE_VARCHAR_STR || type == TYPE_NVARCHAR_STR || type == TYPE_CHAR)) ||
			(getType(columnIndex) == TYPE_CHAR && type == TYPE_INT8);
	if(!isTypeOk) {
		getLogger()->log(ILog::LL_Error, "RowManipulator: Attempt to set a wrong type value into column %s, column is of type %s but value is of type %s\n", columnName, getTypeName(getType(columnIndex)), getTypeName(type));
		return 0;
	}
	if(getMaxDataCount(columnIndex) < size) {
		getLogger()->log(ILog::LL_Error, "RowManipulator: Attempt to set a too long value into column %s, column has length of %d but value has length of %d\n", columnName, getMaxDataCount(columnIndex), size);
		return 0;
	}
	if(columnIdx)
		*columnIdx = columnIndex;
	return getValuePtr(columnIndex);
}

// Set a column value
void RowManipulator::setDataBit(const char* columnName, char value) {
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_BIT));
	if(!buffer)
		return;
	*buffer = value != 0;
}

void RowManipulator::setDataInt8(const char* columnName, char value) {
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_INT8));
	if(!buffer)
		return;
	*buffer = value;
}

void RowManipulator::setDataInt16(const char* columnName, short value) {
	short* buffer = static_cast<short*>(checkAndGetColumnValuePtr(columnName, TYPE_INT16));
	if(!buffer)
		return;
	*buffer = value;
}

void RowManipulator::setDataInt32(const char* columnName, int value) {
	int* buffer = static_cast<int*>(checkAndGetColumnValuePtr(columnName, TYPE_INT32));
	if(!buffer)
		return;
	*buffer = value;
}

void RowManipulator::setDataInt64(const char* columnName, long long int value) {
	long long int* buffer = static_cast<long long int*>(checkAndGetColumnValuePtr(columnName, TYPE_INT64));
	if(!buffer)
		return;
	*buffer = value;
}

void RowManipulator::setDataFloat32(const char* columnName, float value) {
	float* buffer = static_cast<float*>(checkAndGetColumnValuePtr(columnName, TYPE_FLOAT32));
	if(!buffer)
		return;
	*buffer = value;
}

void RowManipulator::setDataFloat64(const char* columnName, double value) {
	double* buffer = static_cast<double*>(checkAndGetColumnValuePtr(columnName, TYPE_FLOAT64));
	if(!buffer)
		return;
	*buffer = value;
}

void RowManipulator::setDataDecimal(const char* columnName, float value) {
	int columnIndex = 0;
	int* buffer = static_cast<int*>(checkAndGetColumnValuePtr(columnName, TYPE_DECIMAL, 1, &columnIndex));
	if(!buffer)
		return;
	*buffer = (int)(value*pow((float)10, getDataIndex(columnIndex)));
}

// Set a column array data (size is number of element in the input array)
void RowManipulator::setDataBitArray(const char* columnName, char* array, int size) {
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_BIT, size));
	if(!buffer)
		return;
	*buffer = *array;
}

void RowManipulator::setDataInt8Array(const char* columnName, char* array, int size) {
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_INT8, size));
	if(!buffer)
		return;
	memcpy(buffer, array, size * sizeof(buffer));
}

void RowManipulator::setDataInt16Array(const char* columnName, short* array, int size) {
	short* buffer = static_cast<short*>(checkAndGetColumnValuePtr(columnName, TYPE_INT16, size));
	if(!buffer)
		return;
	memcpy(buffer, array, size * sizeof(buffer));
}

void RowManipulator::setDataInt32Array(const char* columnName, int* array, int size) {
	int* buffer = static_cast<int*>(checkAndGetColumnValuePtr(columnName, TYPE_INT32, size));
	if(!buffer)
		return;
	memcpy(buffer, array, size * sizeof(buffer));
}

void RowManipulator::setDataInt64Array(const char* columnName, long long int* array, int size) {
	long long int* buffer = static_cast<long long int*>(checkAndGetColumnValuePtr(columnName, TYPE_INT64, size));
	if(!buffer)
		return;
	memcpy(buffer, array, size * sizeof(buffer));
}

void RowManipulator::setDataFloat32Array(const char* columnName, float* array, int size) {
	float* buffer = static_cast<float*>(checkAndGetColumnValuePtr(columnName, TYPE_FLOAT32, size));
	if(!buffer)
		return;
	memcpy(buffer, array, size * sizeof(buffer));
}

void RowManipulator::setDataFloat64Array(const char* columnName, double* array, int size) {
	double* buffer = static_cast<double*>(checkAndGetColumnValuePtr(columnName, TYPE_FLOAT64, size));
	if(!buffer)
		return;
	memcpy(buffer, array, size * sizeof(buffer));
}

void RowManipulator::setDataDecimalArray(const char* columnName, float* array, int size) {
	int columnIndex = 0;
	int* buffer = static_cast<int*>(checkAndGetColumnValuePtr(columnName, TYPE_DECIMAL, size, &columnIndex));
	if(!buffer)
		return;
	float mult = pow((float)10, getDataIndex(columnIndex));
	for(int i = 0; i < size; i++) {
		buffer[i] = (int)(array[i]*mult);
	}
}

void RowManipulator::setDataString(const char* columnName, const char* value) {
	int columnIndex = 0;
	int size = strlen(value);
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_VARCHAR_STR, size, &columnIndex));
	if(!buffer)
		return;
	if(getType(columnIndex) != TYPE_CHAR) {
		freeValue(columnIndex);
		initData(columnIndex, size);
	}
	buffer = static_cast<char*>(getValuePtr(columnIndex));
	memcpy(buffer, value, size);
	if(getType(columnIndex) == TYPE_CHAR && getDataCount(columnIndex) > size)
		buffer[size] = 0;
}

// get a column value
char RowManipulator::getDataBit(const char* columnName) {
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_BIT));
	if(!buffer)
		return 0;
	return *buffer != 0;
}

char RowManipulator::getDataInt8(const char* columnName) {
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_INT8));
	if(!buffer)
		return 0;
	return *buffer;
}

short RowManipulator::getDataInt16(const char* columnName) {
	short* buffer = static_cast<short*>(checkAndGetColumnValuePtr(columnName, TYPE_INT16));
	if(!buffer)
		return 0;
	return *buffer;
}

int RowManipulator::getDataInt32(const char* columnName) {
	int* buffer = static_cast<int*>(checkAndGetColumnValuePtr(columnName, TYPE_INT32));
	if(!buffer)
		return 0;
	return *buffer;
}

long long int RowManipulator::getDataInt64(const char* columnName) {
	long long int* buffer = static_cast<long long int*>(checkAndGetColumnValuePtr(columnName, TYPE_INT64));
	if(!buffer)
		return 0;
	return *buffer;
}

float RowManipulator::getDataFloat32(const char* columnName) {
	float* buffer = static_cast<float*>(checkAndGetColumnValuePtr(columnName, TYPE_FLOAT32));
	if(!buffer)
		return 0;
	return *buffer;
}

double RowManipulator::getDataFloat64(const char* columnName) {
	double* buffer = static_cast<double*>(checkAndGetColumnValuePtr(columnName, TYPE_FLOAT64));
	if(!buffer)
		return 0;
	return *buffer;
}

float RowManipulator::getDataDecimal(const char* columnName) {
	int columnIndex = 0;
	int* buffer = static_cast<int*>(checkAndGetColumnValuePtr(columnName, TYPE_DECIMAL, 1, &columnIndex));
	if(!buffer)
		return 0;
	return *buffer/pow((float)10, getDataIndex(columnIndex));
}

// get a column array value
char RowManipulator::getDataBitArray(const char* columnName, int index) {
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_BIT, index+1));
	if(!buffer)
		return 0;
	return (*buffer >> index) & 1;
}

char RowManipulator::getDataInt8Array(const char* columnName, int index) {
	char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_INT8, index+1));
	if(!buffer)
		return 0;
	return buffer[index];
}

short RowManipulator::getDataInt16Array(const char* columnName, int index) {
	short* buffer = static_cast<short*>(checkAndGetColumnValuePtr(columnName, TYPE_INT16, index+1));
	if(!buffer)
		return 0;
	return buffer[index];
}

int RowManipulator::getDataInt32Array(const char* columnName, int index) {
	int* buffer = static_cast<int*>(checkAndGetColumnValuePtr(columnName, TYPE_INT32, index+1));
	if(!buffer)
		return 0;
	return buffer[index];
}

long long int RowManipulator::getDataInt64Array(const char* columnName, int index) {
	long long int* buffer = static_cast<long long int*>(checkAndGetColumnValuePtr(columnName, TYPE_INT64, index+1));
	if(!buffer)
		return 0;
	return buffer[index];
}

float RowManipulator::getDataFloat32Array(const char* columnName, int index) {
	float* buffer = static_cast<float*>(checkAndGetColumnValuePtr(columnName, TYPE_FLOAT32, index+1));
	if(!buffer)
		return 0;
	return buffer[index];
}

double RowManipulator::getDataFloat64Array(const char* columnName, int index) {
	double* buffer = static_cast<double*>(checkAndGetColumnValuePtr(columnName, TYPE_FLOAT64, index+1));
	if(!buffer)
		return 0;
	return buffer[index];
}

float RowManipulator::getDataDecimalArray(const char* columnName, int index) {
	int columnIndex = 0;
	int* buffer = static_cast<int*>(checkAndGetColumnValuePtr(columnName, TYPE_DECIMAL, index+1, &columnIndex));
	if(!buffer)
		return 0;
	return buffer[index]/pow((float)10, getDataIndex(columnIndex));
}

const char* RowManipulator::getDataString(const char* columnName) {
	const char* buffer = static_cast<char*>(checkAndGetColumnValuePtr(columnName, TYPE_VARCHAR_STR, 0));
	if(!buffer)
		return "";
	return buffer;
}

} //namespace
