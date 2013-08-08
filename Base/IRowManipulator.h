#ifndef IROWMANIPULATOR_H
#define IROWMANIPULATOR_H

#include "DataType.h"
#include "ExportDLL.h"
#include "IObject.h"

class IRowManipulator;

#ifdef __cplusplus
extern "C" {
#endif

EBASEDLL IRowManipulator * DLLCALLCONV createRowManipulator(const DataDescriptor* columnsType, FieldOrder *order = 0, void **initValue = 0);

#ifdef __cplusplus
}
#endif

class IRowManipulator : public IObject
{
	public:
		virtual int DLLCALLCONV allocRow() = 0;
		virtual int DLLCALLCONV completeRowInit() = 0; //complete initialization of ignored fields
		virtual void DLLCALLCONV freeRow() = 0;
		virtual void DLLCALLCONV freeValue(int colPos) = 0;

		//must be called before getValuePtr()
		virtual int DLLCALLCONV initData(int colPos, unsigned int dataCount = 0) = 0;

		virtual void * DLLCALLCONV getValuePtr(int colPos) = 0;
		virtual void * DLLCALLCONV getValuePtr(const char* columnName) = 0;

		virtual int DLLCALLCONV getDataCount(int colPos) = 0;	//use the value saved in TYPE_VARCHAR_SIZE to get TYPE_VARCHAR_STR size else return the same as getMaxDataCount()
		virtual void DLLCALLCONV setDataCount(int colPos, int dataCount) = 0;


		virtual void ** DLLCALLCONV getCurrentRow() = 0;
		virtual void DLLCALLCONV setCurrentRow(void **row) = 0;

		virtual void DLLCALLCONV setFieldOrder(FieldOrder *newOrder) = 0;
		virtual FieldOrder* DLLCALLCONV getFieldOrder() = 0;
		virtual const DataDescriptor * DLLCALLCONV getColumnType() = 0;

		virtual int DLLCALLCONV getMaxDataCount(int colPos) = 0;
		virtual int DLLCALLCONV getDataIndex(int colPos) = 0;
		virtual int DLLCALLCONV getType(int colPos) = 0;
		virtual int DLLCALLCONV getIgnoreType(int colPos) = 0;
		virtual int DLLCALLCONV getFlags(int colPos) = 0;

		virtual int DLLCALLCONV getColumnCount() = 0;
		virtual const char * DLLCALLCONV getColumnName(int colPos) = 0;
		virtual int DLLCALLCONV getColumnOriginalIndex(int i) = 0;
		virtual int DLLCALLCONV getColumnIndex(const char* columnName) = 0;
};

#endif // IROWMANIPULATOR_H
