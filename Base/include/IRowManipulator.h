#ifndef IROWMANIPULATOR_H
#define IROWMANIPULATOR_H

#include "DataType.h"
#include "ExportDLL.h"
#include "IObject.h"

class IRowManipulator;

#ifdef __cplusplus
extern "C" {
#endif

EBASEDLL IRowManipulator* DLLCALLCONV createRowManipulator(const DataDescriptor* columnsType,
                                                           FieldOrder* order = 0,
                                                           void** initValue = 0);

#ifdef __cplusplus
}
#endif

class IRowManipulator : public IObject {
public:
	virtual int DLLCALLCONV allocRow() = 0;
	virtual int DLLCALLCONV completeRowInit() = 0;  // complete initialization of ignored fields
	virtual void DLLCALLCONV freeRow() = 0;
	virtual void DLLCALLCONV freeValue(int colPos) = 0;

	// must be called before getValuePtr()
	virtual int DLLCALLCONV initData(int colPos, unsigned int dataCount = 0) = 0;

	virtual void* DLLCALLCONV getValuePtrByName(const char* columnName) = 0;
	virtual void* DLLCALLCONV getValuePtr(int colPos) = 0;

	virtual int DLLCALLCONV getDataCount(int colPos) = 0;  // use the value saved in TYPE_VARCHAR_SIZE to get
	                                                       // TYPE_VARCHAR_STR size else return the same as
	                                                       // getMaxDataCount()
	virtual void DLLCALLCONV setDataCount(int colPos, int dataCount) = 0;

	virtual void** DLLCALLCONV getCurrentRow() = 0;
	virtual void DLLCALLCONV setCurrentRow(void** row) = 0;

	virtual void DLLCALLCONV setFieldOrder(FieldOrder* newOrder) = 0;
	virtual FieldOrder* DLLCALLCONV getFieldOrder() = 0;
	virtual const DataDescriptor* DLLCALLCONV getColumnType() = 0;

	virtual int DLLCALLCONV getMaxDataCount(int colPos) = 0;
	virtual int DLLCALLCONV getDataIndex(int colPos) = 0;
	virtual int DLLCALLCONV getType(int colPos) = 0;
	virtual int DLLCALLCONV getIgnoreType(int colPos) = 0;
	virtual int DLLCALLCONV getFlags(int colPos) = 0;

	virtual int DLLCALLCONV getColumnCount() = 0;
	virtual const char* DLLCALLCONV getColumnName(int colPos) = 0;
	virtual int DLLCALLCONV getColumnOriginalIndex(int i) = 0;
	virtual int DLLCALLCONV getColumnIndex(const char* columnName) = 0;

	// Set a column value
	virtual void DLLCALLCONV setDataBit(const char* columnName, char value) = 0;
	virtual void DLLCALLCONV setDataInt8(const char* columnName, char value) = 0;
	virtual void DLLCALLCONV setDataInt16(const char* columnName, short value) = 0;
	virtual void DLLCALLCONV setDataInt32(const char* columnName, int value) = 0;
	virtual void DLLCALLCONV setDataInt64(const char* columnName, long long int value) = 0;
	virtual void DLLCALLCONV setDataFloat32(const char* columnName, float value) = 0;
	virtual void DLLCALLCONV setDataFloat64(const char* columnName, double value) = 0;
	virtual void DLLCALLCONV setDataDecimal(const char* columnName, float value) = 0;

	// Set a column array data (size is number of element in the input array)
	virtual void DLLCALLCONV setDataBitArray(const char* columnName, char* array, int size) = 0;
	virtual void DLLCALLCONV setDataInt8Array(const char* columnName, char* array, int size) = 0;
	virtual void DLLCALLCONV setDataInt16Array(const char* columnName, short* array, int size) = 0;
	virtual void DLLCALLCONV setDataInt32Array(const char* columnName, int* array, int size) = 0;
	virtual void DLLCALLCONV setDataInt64Array(const char* columnName, long long int* array, int size) = 0;
	virtual void DLLCALLCONV setDataFloat32Array(const char* columnName, float* array, int size) = 0;
	virtual void DLLCALLCONV setDataFloat64Array(const char* columnName, double* array, int size) = 0;
	virtual void DLLCALLCONV setDataDecimalArray(const char* columnName, float* array, int size) = 0;

	virtual void DLLCALLCONV setDataString(const char* columnName, const char* value) = 0;

	// get a column value
	virtual char DLLCALLCONV getDataBit(const char* columnName) = 0;
	virtual char DLLCALLCONV getDataInt8(const char* columnName) = 0;
	virtual short DLLCALLCONV getDataInt16(const char* columnName) = 0;
	virtual int DLLCALLCONV getDataInt32(const char* columnName) = 0;
	virtual long long int DLLCALLCONV getDataInt64(const char* columnName) = 0;
	virtual float DLLCALLCONV getDataFloat32(const char* columnName) = 0;
	virtual double DLLCALLCONV getDataFloat64(const char* columnName) = 0;
	virtual float DLLCALLCONV getDataDecimal(const char* columnName) = 0;

	// get a column array value
	virtual char DLLCALLCONV getDataBitArray(const char* columnName, int index) = 0;
	virtual char DLLCALLCONV getDataInt8Array(const char* columnName, int index) = 0;
	virtual short DLLCALLCONV getDataInt16Array(const char* columnName, int index) = 0;
	virtual int DLLCALLCONV getDataInt32Array(const char* columnName, int index) = 0;
	virtual long long int DLLCALLCONV getDataInt64Array(const char* columnName, int index) = 0;
	virtual float DLLCALLCONV getDataFloat32Array(const char* columnName, int index) = 0;
	virtual double DLLCALLCONV getDataFloat64Array(const char* columnName, int index) = 0;
	virtual float DLLCALLCONV getDataDecimalArray(const char* columnName, int index) = 0;

	virtual const char* DLLCALLCONV getDataString(const char* columnName) = 0;

	// Explicitly set string data size
	virtual void DLLCALLCONV setDataStringSize(const char* columnName, int size) = 0;
};

#endif  // IROWMANIPULATOR_H
