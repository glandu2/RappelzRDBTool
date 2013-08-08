#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "DataType.h"

class IRowManipulator;

class IDataSource
{
	public:
		enum eOpenMode {OM_Read, OM_Write };

		virtual int open(const char* source, eOpenMode openMode, const char* location = 0, const char* user = 0, const char* password = 0, const char* options = 0);
		virtual void close() {}

		int prepare(IRowManipulator *row, unsigned int rowCount = 0);
		inline int processRow() { if(openMode == OM_Read) return readRow(); return writeRow(); }
		virtual bool hasNext() = 0;

		inline unsigned int getRowNumber() { return rowNumber; }
		inline IRowManipulator *getRowManipulator() { return row; }
		inline eOpenMode getOpenMode() { return openMode; }

	protected:
		virtual int prepareRead(IRowManipulator *row) = 0;
		virtual int prepareWrite(IRowManipulator *row, unsigned int rowCount) = 0;
		virtual int readRow() = 0;
		virtual int writeRow() = 0;

		inline void setRowNumber(unsigned int rowNum) { rowNumber = rowNum; }

	private:
		eOpenMode openMode;
		IRowManipulator *row;
		unsigned int rowNumber;
};

#endif // DATASOURCE_H
