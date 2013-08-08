#include "CSVSource.h"
#include "RowManipulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

//Fast conversion
//From http://code.google.com/p/stringencoders/source/browse/trunk/src/modp_numtoa.c

static void strreverse(char* begin, char* end)
{
	char aux;
	while (end > begin)
		aux = *end, *end-- = *begin, *begin++ = aux;
}

static size_t modp_itoa10(int64_t value, char* str)
{
	char* wstr=str;
	// Take care of sign
	uint64_t uvalue = (value < 0) ? (uint64_t)(-value) : (uint64_t)(value);
	// Conversion. Number is reversed.
	do *wstr++ = (char)(48 + (uvalue % 10)); while(uvalue /= 10);
	if (value < 0) *wstr++ = '-';
	*wstr='\0';

	// Reverse string
	strreverse(str,wstr-1);
	return (size_t)(wstr - str);
}

static size_t modp_itoa16(uint64_t uvalue, char* str)
{
	const char *digits = "0123456789ABCDEF";
	char* wstr=str;
	// Take care of sign
	// Conversion. Number is reversed.
	do *wstr++ = digits[uvalue & 0xF]; while(uvalue >> 4);
	*wstr='\0';

	// Reverse string
	strreverse(str,wstr-1);
	return (size_t)(wstr - str);
}

static void fputi(int64_t value, bool isHex, FILE *file) {
	int len;
	char buffer[32];

	if(isHex)
		len = modp_itoa16((uint64_t)value, buffer);
	else
		len = modp_itoa10(value, buffer);

	fwrite(buffer, 1, len, file);
}


namespace RappelzRDBBase {

int CSVSource::open(const char* source, eOpenMode openMode,  const char* location, const char* user, const char* password, const char* options) {
	IDataSource::open(source, openMode, location, user, password, options);
	if(!strcmp(source, "con") && openMode == OM_Read) {
		csvFile = stdin;
	} else if(!strcmp(source, "con") && openMode == OM_Write) {
		csvFile = stdout;
	} else if(openMode == OM_Read) {
		csvFile = fopen(source, "rb");
	} else {
		csvFile = fopen(source, "wb");
	}

	if(csvFile != stdin && csvFile != stdout) {
		io_buffer = malloc(65536);
		setvbuf(csvFile, (char*)io_buffer, _IOFBF, 65536);
	}

	if(!csvFile)
		return errno;

	return 0;
}

void CSVSource::close() {
	if(csvFile != stdin && csvFile != stdout)
		fclose(csvFile);
	free(io_buffer);
}

int CSVSource::prepareWrite(IRowManipulator *row, unsigned int rowNumber) {
	int i, firstColumn = 1, curCol;

	for(i=0; i<row->getColumnCount(); i++) {
		curCol = i;

		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_CSVIGNORE))
			continue;

		if(firstColumn == 1)
			firstColumn = 0;
		else fputc('\t', csvFile);
		fprintf(csvFile, "%s", row->getColumnName(curCol));
	}
	putc('\n', csvFile);
	return 0;
}

int CSVSource::prepareRead(IRowManipulator *row) {
	fgets(line, 4096, csvFile);	//discard columns names
	if(fgets(line, 4096, csvFile) == NULL) {	//preread lines
		eofState = ES_EOF;
		return 0;
	}
	setRowNumber(0);	//incremental row count

	return 0;
}

int CSVSource::readRow() {
	int i, curCol;
	char *readptr;
	void *buffer;
	char *tabpos;
	IRowManipulator *row = getRowManipulator();

	readptr = line;
	for(i=0; i<row->getColumnCount(); i++) {
		curCol = i;

		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_CSVIGNORE))
			continue;

		tabpos = strpbrk(readptr, "\t\n\r");
		if(!tabpos) {
			fprintf(stderr, "Can't find a field separator for next CSV column (\\t, \\r or \\n) at line %d, column missing ?\n", getRowNumber());
			break;
		}
		*tabpos = 0;

		if(row->getType(curCol) == TYPE_VARCHAR_STR)
			row->initData(curCol, strlen(readptr)+1);
		else row->initData(curCol);

		buffer = row->getValuePtr(curCol);

		switch(row->getType(curCol)) {
			case TYPE_INT8:
			case TYPE_INT16:
			case TYPE_INT32:
			case TYPE_BIT:
				//sscanf(readptr, "%d", (int*)buffer);  SLOW
				*(int*)buffer = atoi(readptr);
				break;

			case TYPE_DECIMAL:
				{
					float decimalTemp;
					sscanf(readptr, "%f", &decimalTemp);
					*(int*)buffer = (int)(decimalTemp*pow((float)10, row->getDataIndex(curCol)) + 0.5);
				}
				break;

			case TYPE_INT64:
				sscanf(readptr, "%I64d", (long long int*)buffer);
				break;

			case TYPE_CHAR:
				strcpy((char*)buffer, readptr);
				break;

			case TYPE_FLOAT32:
				sscanf(readptr, "%f", (float*)buffer);
				break;

			case TYPE_FLOAT64:
				sscanf(readptr, "%lf", (double*)buffer);
				break;

			case TYPE_VARCHAR_STR:
				strcpy((char*)buffer, readptr);
				break;
		}
		readptr = tabpos+1; //go to next field
	}

	setRowNumber(getRowNumber()+1);

	//read next line
	if(fgets(line, 4096, csvFile) == NULL) {
		eofState = ES_EOF;
		return 0;
	}
	return 0;
}

int CSVSource::writeRow() {
	int bitRead, i, curCol, count, firstColumn = 1;
	void *buffer;
	IRowManipulator *row = getRowManipulator();

	for(i=0; i<row->getColumnCount(); i++) {
		curCol = i;
		if(GET_FLAGBIT(row->getIgnoreType(curCol), TYPE_CSVIGNORE))
			continue;

		count = row->getDataCount(curCol);
		buffer = row->getValuePtr(curCol);

		if(firstColumn == 1)
			firstColumn = 0;
		else fputc('\t', csvFile);

		switch(row->getType(curCol)) {
			case TYPE_BIT:
				bitRead = 0;
				for(; count > 0; count--) {
					fprintf(csvFile, "%d", (*(char*)buffer >> bitRead) & 1);
					bitRead++;
					if(!(bitRead % 8)) {
						buffer = (char*)buffer + 1;
						bitRead = 0;
					}
				}
				break;

			case TYPE_CHAR:
				fwrite(buffer, 1, strlen((char*)buffer), csvFile);
				break;

			case TYPE_VARCHAR_STR:
				if(((char*)buffer)[count-1] == 0)
					fwrite(buffer, 1, count-1, csvFile);
				else fwrite(buffer, 1, count, csvFile);
				break;

			case TYPE_INT8:
				if(count > 1)
					for(int i=0; count > 0; count--, i++)
						fputi(((unsigned char*)buffer)[i], true, csvFile);
				else fputi(*(char*)buffer, false, csvFile);
				break;

			case TYPE_INT16:
				fputi(*(short*)buffer, false, csvFile);
				break;

			case TYPE_FLOAT32:
				fprintf(csvFile, "%f", *(float*)buffer);
				break;

			case TYPE_DECIMAL:
				fprintf(csvFile, "%f", (*(int*)buffer)/pow((float)10, row->getDataIndex(curCol)));
				break;

			case TYPE_INT32:
				//fprintf(csvFile, "%d", *(int*)buffer);  slow
				fputi(*(int*)buffer, false, csvFile);
				break;

			case TYPE_FLOAT64:
				fprintf(csvFile, "%lf", *(double*)buffer);
				break;

			case TYPE_INT64:
				fputi(*(long long int*)buffer, false, csvFile);
				break;
		}
	}
	fputc('\n', csvFile);

	return 0;
}

bool CSVSource::hasNext() {
	if(eofState == ES_EOF)
		return false;
	return true;
}

}//namespace
