#include "RDBSource.h"
#include "ILog.h"
#include "RowManipulator.h"
#include <cmath>
#include <errno.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace RappelzRDBBase {

RDBSource::RDBSource() {
	date = 0;
	defaultStringSize = 52;
}

static inline bool endsWith(const char* str, const char* suffix) {
	if(!str || !suffix)
		return false;
	size_t lenstr = strlen(str);
	size_t lensuffix = strlen(suffix);
	if(lensuffix > lenstr)
		return false;
	return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int RDBSource::open(const char* source, eOpenMode openMode, const char* location, const char* options) {
	IDataSource::open(source, openMode, location, options);

	const char* fopenOpenMode;
	if(!source)
		return EINVAL;

	if(openMode == OM_Read)
		fopenOpenMode = "rb";
	else
		fopenOpenMode = "wb";

	rdbFile = fopen(source, fopenOpenMode);
	rowRead = 0;
	refFile = endsWith(source, ".ref");

	if(!rdbFile) {
		getLogger()->log(ILog::LL_Error, "RDBSource: Can't open file \"%s\": %s\n", source, strerror(errno));
		if(errno == ENOENT)
			return ENOENT;
		else
			return EIO;
	}

	io_buffer = malloc(65536);
	setvbuf(rdbFile, (char*) io_buffer, _IOFBF, 65536);

	return 0;
}

void RDBSource::close() {
	fclose(rdbFile);
	rdbFile = 0;
	free(io_buffer);
	io_buffer = 0;
}

static void copyWithNOT(char* buffer, const char* src) {
	while(*src) {
		*buffer = ~*src;
		buffer++;
		src++;
	}
}

int RDBSource::prepareWrite(IRowManipulator* row, unsigned int rowCount) {
	if(!refFile) {
		char header[0x80];
		time_t rawtime;

		memset(header, 0, 0x80);
		time(&rawtime);
		strftime(header, 9, "%Y%m%d", localtime(&rawtime));
		copyWithNOT(
		    header + 16,
		    "\xad\xbb\xbd\xdf\xbc\x8d\x9a\x9e\x8b\x9a\x9b\xdf\x9d\x86\xdf\xad\x9e\x8f\x8f\x9a\x93\x85\xad\xbb\xbd"
		    "\xab\x90\x90\x93\xdf\x99\x8d\x90\x92\xdf\xb8\x93\x9e\x91\x9b\x8a\xcd");
		fwrite(header, 1, 0x80, rdbFile);
	}
	fwrite(&rowCount, 4, 1, rdbFile);
	if(refFile) {
		defaultStringSize = 52;
		for(int i = 0; i < row->getColumnCount(); i++) {
			if(GET_FLAGBIT(row->getIgnoreType(i), TYPE_RDBIGNORE))
				continue;

			if(row->getType(i) == TYPE_CHAR) {
				defaultStringSize = row->getDataCount(i);
				getLogger()->log(ILog::LL_Info,
				                 "Writing ref file with string length %d from field %s\n",
				                 defaultStringSize,
				                 row->getColumnName(i));
				break;
			}
		}

		fwrite(&defaultStringSize, 4, 1, rdbFile);
	}

	bitAvailable = 8;

	return 0;
}

int RDBSource::prepareRead(IRowManipulator* row) {
	int rowNumber;

	if(!refFile) {
		char dateBuffer[9];
		fread(dateBuffer, 1, 8, rdbFile);
		dateBuffer[8] = 0;

		struct tm fileTime = {0};
		sscanf(dateBuffer, "%4d%2d%2d", &fileTime.tm_year, &fileTime.tm_mon, &fileTime.tm_mday);
		fileTime.tm_year -= 1900;
		fileTime.tm_mon--;
		date = mktime(&fileTime);

		fseek(rdbFile, 0x80, SEEK_SET);
	} else {
		fseek(rdbFile, 0x00, SEEK_SET);
	}
	fread(&rowNumber, 4, 1, rdbFile);
	if(refFile) {
		fread(&defaultStringSize, 4, 1, rdbFile);
	}
	setRowNumber(rowNumber);
	bitAvailable = 0;

	return 0;
}

int RDBSource::readRow() {
	int bitRead, i, curCol, count;
	void* buffer;
	IRowManipulator* row = getRowManipulator();

	for(i = 0; i < row->getColumnCount(); i++) {
		curCol = i;
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_RDBIGNORE))
			continue;

		count = row->getDataCount(curCol);
		row->initData(curCol);
		buffer = row->getValuePtr(curCol);

		switch(row->getType(curCol)) {
			case TYPE_BIT:
				*(char*) buffer = 0;
				bitRead = 0;
				for(; count > 0; count--) {
					if(bitAvailable == 0) {
						if(fread(&currentByte, 1, 1, rdbFile) != 1) {
							getLogger()->log(
							    ILog::LL_Error, "RDBSource: Error while reading record data: %s\n", strerror(errno));
							if(errno)
								return EIO;
							else
								return EINVAL;
						}
						bitAvailable = 8;
					}
					bitAvailable--;
					*(char*) buffer |= (currentByte & 1) << bitRead;
					currentByte >>= 1;
					bitRead++;
					if(!(bitRead % 8)) {
						buffer = (char*) buffer + 1;
						*(char*) buffer = 0;
						bitRead = 0;
					}
				}
				break;

			case TYPE_CHAR: {
				int realCount = count;
				if(refFile) {
					if((unsigned int) count < defaultStringSize) {
						getLogger()->log(
						    ILog::LL_Error,
						    "RDBSource: Ref file string length is %d which is larger than string field size %d: %s\n",
						    defaultStringSize,
						    count,
						    strerror(errno));
						return EINVAL;
					}
					realCount = defaultStringSize;
				}
				if(fread(buffer, 1, realCount, rdbFile) != realCount) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while reading record data: %s\n", strerror(errno));
					if(errno)
						return EIO;
					else
						return EINVAL;
				}
				break;
			}
			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR:
			case TYPE_INT8:
				if(fread(buffer, 1, count, rdbFile) != count) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while reading record data: %s\n", strerror(errno));
					if(errno)
						return EIO;
					else
						return EINVAL;
				}
				break;

			case TYPE_INT16:
				if(fread(buffer, 2, count, rdbFile) != count) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while reading record data: %s\n", strerror(errno));
					if(errno)
						return EIO;
					else
						return EINVAL;
				}
				break;

			case TYPE_VARCHAR_SIZE:
			case TYPE_FLOAT32:
			case TYPE_DECIMAL:
			case TYPE_INT32:
				if(fread(buffer, 4, count, rdbFile) != count) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while reading record data: %s\n", strerror(errno));
					if(errno)
						return EIO;
					else
						return EINVAL;
				}
				break;

			case TYPE_FLOAT64:
			case TYPE_INT64:
				if(fread(buffer, 8, count, rdbFile) != count) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while reading record data: %s\n", strerror(errno));
					if(errno)
						return EIO;
					else
						return EINVAL;
				}
				break;
		}

		// Protect from negative size in TYPE_VARCHAR_SIZE
		if(row->getType(curCol) == TYPE_VARCHAR_SIZE) {
			if(*reinterpret_cast<int*>(buffer) < 0) {
				*reinterpret_cast<int*>(buffer) = 0;
			}
		}

#ifndef _MSC_VER
		if(row->getType(curCol) == TYPE_FLOAT32 && buffer) {
			float* val = (float*) buffer;
			switch(std::fpclassify(*val)) {
				case FP_INFINITE:
					if(*val < 0)
						*val = -FLT_MAX;
					else
						*val = FLT_MAX;
					break;

				case FP_NAN:
				case FP_SUBNORMAL:
					*val = 0;
					break;
			}
		} else if(row->getType(curCol) == TYPE_FLOAT64 && buffer) {
			double* val = (double*) buffer;
			switch(std::fpclassify(*val)) {
				case FP_INFINITE:
					if(*val < 0)
						*val = -DBL_MAX;
					else
						*val = DBL_MAX;
					break;

				case FP_NAN:
				case FP_SUBNORMAL:
					*val = 0;
					break;
			}
		}
#else
		if(row->getType(curCol) == TYPE_FLOAT32 && buffer) {
			float* val = (float*) buffer;

			switch(_fpclass(*val)) {
				case _FPCLASS_NINF:
					*val = -FLT_MAX;
					break;

				case _FPCLASS_PINF:
					*val = FLT_MAX;
					break;

				case _FPCLASS_QNAN:
				case _FPCLASS_SNAN:
				case _FPCLASS_ND:
				case _FPCLASS_PD:
					*val = 0;
					break;
			}

			// Denormalized numbers are not correctly detected on msvc2010
			if((*val > 0 && *val < FLT_MIN) || (*val < 0 && *val > -FLT_MIN))
				*val = 0;
		} else if(row->getType(curCol) == TYPE_FLOAT64 && buffer) {
			double* val = (double*) buffer;

			switch(_fpclass(*val)) {
				case _FPCLASS_NINF:
					*val = -DBL_MAX;
					break;

				case _FPCLASS_PINF:
					*val = DBL_MAX;
					break;

				case _FPCLASS_QNAN:
				case _FPCLASS_SNAN:
				case _FPCLASS_ND:
				case _FPCLASS_PD:
					*val = 0;
					break;
			}
		}
#endif

		if(row->getType(curCol) != TYPE_BIT) {
			bitAvailable = 0;
		}
	}

	rowRead++;

	return 0;
}

int RDBSource::writeRow() {
	int bitRead, i, curCol, count;
	void* buffer;
	IRowManipulator* row = getRowManipulator();

	for(i = 0; i < row->getColumnCount(); i++) {
		curCol = i;
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_RDBIGNORE))
			continue;

		count = row->getDataCount(curCol);
		buffer = row->getValuePtr(curCol);

		if(row->getType(curCol) != TYPE_BIT) {
			if(bitAvailable != 8)
				if(fwrite(&currentByte, 1, 1, rdbFile) != 1) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while writing record data: %s\n", strerror(errno));
					return EIO;
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
							getLogger()->log(
							    ILog::LL_Error, "RDBSource: Error while writing record data: %s\n", strerror(errno));
							return EIO;
						}
						bitAvailable = 8;
						currentByte = 0;
					}
					currentByte |= ((*(char*) buffer >> bitRead) & 1) << (8 - bitAvailable);
					bitAvailable--;
					bitRead++;
					if(!(bitRead % 8)) {
						buffer = (char*) buffer + 1;
						bitRead = 0;
					}
				}
				break;

			case TYPE_CHAR: {
				int realCount = count;
				if(refFile) {
					if((unsigned int) count < defaultStringSize) {
						getLogger()->log(ILog::LL_Error,
						                 "RDBSource: Writing ref file string length is %d which is larger than string "
						                 "field size %d: %s\n",
						                 defaultStringSize,
						                 count,
						                 strerror(errno));
						return EINVAL;
					}
					if((unsigned int) count != defaultStringSize) {
						getLogger()->log(
						    ILog::LL_Warning,
						    "RDBSource: Writing ref file string length is %d which is different than string "
						    "field size %d: %s\n",
						    defaultStringSize,
						    count,
						    strerror(errno));
					}
					realCount = defaultStringSize;
				}
				if(fwrite(buffer, 1, realCount, rdbFile) != realCount) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while writing record data: %s\n", strerror(errno));
					return EIO;
				}
				break;
			}

			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR:
			case TYPE_INT8:
				if(fwrite(buffer, 1, count, rdbFile) != count) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while writing record data: %s\n", strerror(errno));
					return EIO;
				}
				break;

			case TYPE_INT16:
				if(fwrite(buffer, 2, count, rdbFile) != count) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while writing record data: %s\n", strerror(errno));
					return EIO;
				}
				break;

			case TYPE_FLOAT32:
			case TYPE_VARCHAR_SIZE:
			case TYPE_DECIMAL:
			case TYPE_INT32:
				if(fwrite(buffer, 4, count, rdbFile) != count) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while writing record data: %s\n", strerror(errno));
					return EIO;
				}
				break;

			case TYPE_FLOAT64:
			case TYPE_INT64:
				if(fwrite(buffer, 8, count, rdbFile) != count) {
					getLogger()->log(
					    ILog::LL_Error, "RDBSource: Error while writing record data: %s\n", strerror(errno));
					return EIO;
				}
				break;
		}
	}

	return 0;
}

bool RDBSource::hasNext() {
	return !feof(rdbFile) && rowRead < getRowNumber();
}

}  // namespace RappelzRDBBase
