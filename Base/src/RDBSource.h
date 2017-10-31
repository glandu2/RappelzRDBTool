#ifndef RDBSOURCE_H
#define RDBSOURCE_H

#include "IDataSource.h"
#include <stdio.h>

namespace RappelzRDBBase {

class RDBSource : public IDataSource {
public:
	RDBSource();

	virtual int open(const char* source, eOpenMode openMode, const char* location = 0, const char* options = 0);
	virtual void close();

	virtual unsigned long long int getDate() { return date; }

	virtual int prepareRead(IRowManipulator* row);
	virtual int prepareWrite(IRowManipulator* row, unsigned int rowCount);
	virtual int readRow();
	virtual int writeRow();

	virtual bool hasNext();

protected:
	FILE* rdbFile;
	bool refFile;
	unsigned int defaultStringSize;
	unsigned char currentByte;
	unsigned char bitAvailable;
	unsigned int rowRead;
	void* io_buffer;
	unsigned long long int date;
};

}  // namespace RappelzRDBBase

#endif  // RDBSOURCE_H
