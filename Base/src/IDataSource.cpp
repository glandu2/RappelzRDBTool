#include "IDataSource.h"
#include "string.h"

int IDataSource::open(const char* source, eOpenMode openMode, const char* location, const char* options) {
	this->openMode = openMode;
	return 0;
}

const char* IDataSource::getNextOption(const char* options, char* key, char* value) {
	if(options == 0 || options[0] == '\0')
		return 0;

	const char* valueStart = strchr(options, '=');
	if(!valueStart)
		return 0;
	valueStart++;

	const char* valueEnd = strchr(valueStart, ';');
	if(!valueEnd)
		valueEnd = valueStart + strlen(valueStart);

	memcpy(key, options, valueStart - 1 - options);
	key[valueStart - 1 - options] = 0;

	memcpy(value, valueStart, valueEnd - valueStart);
	value[valueEnd - valueStart] = 0;

	return valueEnd[0] == '\0' ? 0 : (valueEnd + 1);
}

int IDataSource::prepare(IRowManipulator* row, unsigned int rowCount) {
	this->row = row;
	this->rowNumber = rowCount;

	if(openMode == OM_Read)
		return prepareRead(row);
	return prepareWrite(row, rowNumber);
}
