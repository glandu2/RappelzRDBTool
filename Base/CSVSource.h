#ifndef CSVSOURCE_H
#define CSVSOURCE_H

#include "IDataSource.h"
#include <stdio.h>
#include <locale.h>

namespace RappelzRDBBase {

class CSVSource : public IDataSource
{
	public:
		CSVSource() { eofState = ES_NotEOF; setlocale(LC_ALL, "C"); }

		virtual int open(const char* source, eOpenMode openMode,  const char* location = 0, const char* user = 0, const char* password = 0, const char* options = 0);
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

		char line[4096];
		void *io_buffer;
};

} //namespace

#endif // CSVSOURCE_H
