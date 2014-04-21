#include "RowManipulator.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <vector>

#include "DebugOutput.h"

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
		debugoutput("Invalid column index: %d. Only has %d cols (index begin at 0 for first)", colPos, getColumnCount());
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
	int i;
	for(i=0; i<getColumnCount(); i++) {
		if(!strcmp(getColumnName(i), columnName))
			return i;
	}

	debugoutput("Column named \"%s\" not found", columnName);
	return -1;
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

} //namespace
