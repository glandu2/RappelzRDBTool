#include "RDBSource.h"
#include "RowManipulator.h"
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

namespace RappelzRDBBase {

RDBSource::RDBSource() {
	date = 0;
}

int RDBSource::open(const char* source, eOpenMode openMode,  const char* location, const char* user, const char* password, const char* options) {
	IDataSource::open(source, openMode, location, user, password, options);

	const char* fopenOpenMode;
	if(!source) return EINVAL;

	if(openMode == OM_Read)
		fopenOpenMode = "rb";
	else fopenOpenMode = "wb";

	rdbFile = fopen(source, fopenOpenMode);
	rowRead = 0;

	if(!rdbFile) {
		if(errno == ENOENT)
			return ENOENT;
		else
			return EIO;
	}

	io_buffer = malloc(65536);
	setvbuf(rdbFile, (char*)io_buffer, _IOFBF, 65536);

	return 0;
}

void RDBSource::close() {
	fclose(rdbFile);
	rdbFile = 0;
	free(io_buffer);
	io_buffer = 0;
}

void copyWithNOT(char *buffer, const char *src) {
	while(*src) {
		*buffer = ~*src;
		buffer++;
		src++;
	}
}

int RDBSource::prepareWrite(IRowManipulator *row, unsigned int rowCount) {
	char header[0x80];
	time_t rawtime;

	memset(header, 0, 0x80);
	time(&rawtime);
	strftime(header, 9, "%Y%m%d", localtime(&rawtime));
	copyWithNOT(header + 16, "\xad\xbb\xbd\xdf\xbc\x8d\x9a\x9e\x8b\x9a\x9b\xdf\x9d\x86\xdf\xad\x9e\x8f\x8f\x9a\x93\x85\xad\xbb\xbd\xab\x90\x90\x93\xdf\x99\x8d\x90\x92\xdf\xb8\x93\x9e\x91\x9b\x8a\xcd");
	fwrite(header, 1, 0x80, rdbFile);
	fwrite(&rowCount, 4, 1, rdbFile);

	bitAvailable = 8;

	return 0;
}

int RDBSource::prepareRead(IRowManipulator *row) {
	int rowNumber;
	char dateBuffer[9];
	fread(dateBuffer, 1, 8, rdbFile);
	dateBuffer[8] = 0;

	struct tm fileTime = {0};
	sscanf(dateBuffer, "%4d%2d%2d", &fileTime.tm_year, &fileTime.tm_mon, &fileTime.tm_mday);
	fileTime.tm_year -= 1900;
	fileTime.tm_mon--;
	date = mktime(&fileTime);

	fseek(rdbFile, 0x80, SEEK_SET);
	fread(&rowNumber, 4, 1, rdbFile);

	setRowNumber(rowNumber);
	bitAvailable = 0;

	return 0;
}

int RDBSource::readRow() {
	int bitRead, i, curCol, count;
	void *buffer;
	IRowManipulator *row = getRowManipulator();

	for(i=0; i<row->getColumnCount(); i++) {
		curCol = i;
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_RDBIGNORE))
			continue;

		count = row->getDataCount(curCol);
		row->initData(curCol);
		buffer = row->getValuePtr(curCol);

		switch(row->getType(curCol)) {
			case TYPE_BIT:
				*(char*)buffer = 0;
				bitRead = 0;
				for(; count > 0; count--) {
					if(bitAvailable == 0) {
						if(fread(&currentByte, 1, 1, rdbFile) != 1) {
							if(errno) return EIO;
							else return EOF;
						}
						bitAvailable = 8;
					}
					bitAvailable--;
					*(char*)buffer |= (currentByte & 1) << bitRead;
					currentByte >>= 1;
					bitRead++;
					if(!(bitRead % 8)) {
						buffer = (char*)buffer + 1;
						*(char*)buffer = 0;
						bitRead = 0;
					}
				}
				break;

			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR:
			case TYPE_CHAR:
			case TYPE_INT8:
				if(fread(buffer, 1, count, rdbFile) != count) {
					if(errno) return EIO;
					else return EOF;
				}
				break;

			case TYPE_INT16:
				if(fread(buffer, 2, count, rdbFile) != count) {
					if(errno) return EIO;
					else return EOF;
				}
				break;

			case TYPE_VARCHAR_SIZE:
			case TYPE_FLOAT32:
			case TYPE_DECIMAL:
			case TYPE_INT32:
				if(fread(buffer, 4, count, rdbFile) != count) {
					if(errno) return EIO;
					else return EOF;
				}
				break;

			case TYPE_FLOAT64:
			case TYPE_INT64:
				if(fread(buffer, 8, count, rdbFile) != count) {
					if(errno) return EIO;
					else return EOF;
				}
				break;
		}
		if(row->getType(curCol) != TYPE_BIT) {
			bitAvailable = 0;
		}
	}

	rowRead++;

	return 0;
}

int RDBSource::writeRow() {
	int bitRead, i, curCol, count;
	void *buffer;
	IRowManipulator *row = getRowManipulator();

	for(i=0; i<row->getColumnCount(); i++) {
		curCol = i;
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_RDBIGNORE))
			continue;

		count = row->getDataCount(curCol);
		buffer = row->getValuePtr(curCol);

		if(row->getType(curCol) != TYPE_BIT) {
			if(bitAvailable != 8)
				if(fwrite(&currentByte, 1, 1, rdbFile) != 1) {
					if(errno) return EIO;
					else return EOF;
				}
			bitAvailable = 8;
			currentByte = 0;
		}

		switch(row->getType(curCol)) {
			case TYPE_BIT:
				bitRead = 0;
				for(; count > 0; count--) {
					if(bitAvailable == 0) {
						if(fwrite(&currentByte, 1, 1, rdbFile) != 1) {
							if(errno) return EIO;
							else return EOF;
						}
						bitAvailable = 8;
						currentByte = 0;
					}
					currentByte |= ((*(char*)buffer >> bitRead) & 1) << (8-bitAvailable);
					bitAvailable--;
					bitRead++;
					if(!(bitRead % 8)) {
						buffer = (char*)buffer + 1;
						bitRead = 0;
					}
				}
				break;

			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR:
			case TYPE_CHAR:
			case TYPE_INT8:
				if(fwrite(buffer, 1, count, rdbFile) != count) {
					if(errno) return EIO;
					else return EOF;
				}
				break;

			case TYPE_INT16:
				if(fwrite(buffer, 2, count, rdbFile) != count) {
					if(errno) return EIO;
					else return EOF;
				}
				break;

			case TYPE_FLOAT32:
			case TYPE_VARCHAR_SIZE:
			case TYPE_DECIMAL:
			case TYPE_INT32:
				if(fwrite(buffer, 4, count, rdbFile) != count) {
					if(errno) return EIO;
					else return EOF;
				}
				break;

			case TYPE_FLOAT64:
			case TYPE_INT64:
				if(fwrite(buffer, 8, count, rdbFile) != count) {
					if(errno) return EIO;
					else return EOF;
				}
				break;
		}
	}

	return 0;
}

bool RDBSource::hasNext() {
	return !feof(rdbFile) && rowRead < getRowNumber();
}

} //namespace
