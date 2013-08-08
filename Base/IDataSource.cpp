#include "IDataSource.h"

int IDataSource::open(const char* source, eOpenMode openMode, const char* location, const char* user, const char* password, const char* options) {
	this->openMode = openMode;
	return 0;
}

int IDataSource::prepare(IRowManipulator *row, unsigned int rowCount) {
	this->row = row;
	this->rowNumber = rowCount;

	if(openMode == OM_Read)
		return prepareRead(row);
	return prepareWrite(row, rowNumber);
}
