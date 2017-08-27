#ifndef CSVSOURCE_H
#define CSVSOURCE_H

#include "IDataSource.h"
#include <stdio.h>
#include <locale.h>

#define CSVSOURCE_LINESIZE 8192

namespace RappelzRDBBase {

class CSVSource : public IDataSource
{
	public:
		CSVSource() { eofState = ES_NotEOF; setlocale(LC_NUMERIC, "C"); }

		virtual int open(const char* source, eOpenMode openMode, const char *location = 0, const char* options = 0);
		virtual void close();

		virtual unsigned long long int getDate() { return 0; }

		virtual int prepareRead(IRowManipulator *row);
		virtual int prepareWrite(IRowManipulator *row, unsigned int rowCount);
		virtual int readRow();
		virtual int writeRow();

		virtual bool hasNext();

	private:
		enum eEOFState {
			ES_NotEOF,
			ES_EOF
		} eofState;
		FILE *csvFile;

		char line[CSVSOURCE_LINESIZE];
		void *io_buffer;
};

} //namespace

#endif // CSVSOURCE_H
