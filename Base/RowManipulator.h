#ifndef ROWMANIPULATOR_H
#define ROWMANIPULATOR_H

#include "DataType.h"
#include "ExportDLL.h"
#include "IRowManipulator.h"

#include <assert.h>
#include <vector>

namespace RappelzRDBBase {

class RowManipulator : public CImplement<IRowManipulator>
{
	public:
		RowManipulator(const DataDescriptor* columnsType, FieldOrder *order = 0, void **initValue = 0);
		~RowManipulator();

		int DLLCALLCONV allocRow();
		int DLLCALLCONV completeRowInit(); //complete initialization of ignored fields
		void DLLCALLCONV freeRow();
		void DLLCALLCONV freeValue(int colPos);

		//must be called before getValuePtr()
		int DLLCALLCONV initData(int colPos, unsigned int dataCount = 0);

		inline void * DLLCALLCONV getValuePtrByName(const char* columnName) { return getValuePtr(getColumnIndex(columnName)); }
		void * DLLCALLCONV getValuePtr(int colPos);

		int DLLCALLCONV getDataCount(int colPos);	//use the value saved in TYPE_VARCHAR_SIZE to get TYPE_VARCHAR_STR size else return the same as getMaxDataCount()
		void DLLCALLCONV setDataCount(int colPos, int dataCount);


		inline void ** DLLCALLCONV getCurrentRow() { return row; }
		inline void DLLCALLCONV setCurrentRow(void **row) { this->row = row; initRow(true); }

		void DLLCALLCONV setFieldOrder(FieldOrder *newOrder);
		inline FieldOrder* DLLCALLCONV getFieldOrder() { return order; }
		inline const DataDescriptor * DLLCALLCONV getColumnType() { return columnsType; }

		inline int DLLCALLCONV getMaxDataCount(int colPos) { return GET_VAR_LNGTH(columnsTypeOrdered.fieldsInfo[colPos].dataSize); }
		inline int DLLCALLCONV getDataIndex(int colPos) { return GET_VAR_INDEX(columnsTypeOrdered.fieldsInfo[colPos].dataSize); }
		inline int DLLCALLCONV getType(int colPos) { return GET_TYPE(columnsTypeOrdered.fieldsInfo[colPos].dataType); }
		inline int DLLCALLCONV getIgnoreType(int colPos) { return GET_IGNORETYPE(columnsTypeOrdered.fieldsInfo[colPos].dataType); }
		inline int DLLCALLCONV getFlags(int colPos) { return GET_FLAGS(columnsTypeOrdered.fieldsInfo[colPos].dataType); }

		inline int DLLCALLCONV getColumnCount() { return columnsTypeOrdered.numFields; }
		inline const char * DLLCALLCONV getColumnName(int colPos) { return columnsTypeOrdered.fieldsInfo[colPos].columnName; }
#ifndef NDEBUG
		inline int DLLCALLCONV getColumnOriginalIndex(int i) { if(!order) return i; if(!(i < order->size)) { *((char*)0) = 0; } assert(i < order->size); return order->order[i]; }
#else
		inline int DLLCALLCONV getColumnOriginalIndex(int i) { if(!order) return i; return order->order[i]; }
#endif
		int DLLCALLCONV getColumnIndex(const char* columnName);

		// Set a column value
		void DLLCALLCONV setDataBit(const char* columnName, char value); //value must be 1 for true or 0 for false
		void DLLCALLCONV setDataInt8(const char* columnName, char value);
		void DLLCALLCONV setDataInt16(const char* columnName, short value);
		void DLLCALLCONV setDataInt32(const char* columnName, int value);
		void DLLCALLCONV setDataInt64(const char* columnName, long long int value);
		void DLLCALLCONV setDataFloat32(const char* columnName, float value);
		void DLLCALLCONV setDataFloat64(const char* columnName, double value);
		void DLLCALLCONV setDataDecimal(const char* columnName, float value);

		// Set a column array data (size is number of element in the input array)
		void DLLCALLCONV setDataBitArray(const char* columnName, char* array, int size); //value can contains at most 8 bits (more is not supported atm)
		void DLLCALLCONV setDataInt8Array(const char* columnName, char* array, int size);
		void DLLCALLCONV setDataInt16Array(const char* columnName, short* array, int size);
		void DLLCALLCONV setDataInt32Array(const char* columnName, int* array, int size);
		void DLLCALLCONV setDataInt64Array(const char* columnName, long long int* array, int size);
		void DLLCALLCONV setDataFloat32Array(const char* columnName, float* array, int size);
		void DLLCALLCONV setDataFloat64Array(const char* columnName, double* array, int size);
		void DLLCALLCONV setDataDecimalArray(const char* columnName, float* array, int size);

		void DLLCALLCONV setDataString(const char* columnName, const char* value);

		// get a column value
		char DLLCALLCONV getDataBit(const char* columnName);
		char DLLCALLCONV getDataInt8(const char* columnName);
		short DLLCALLCONV getDataInt16(const char* columnName);
		int DLLCALLCONV getDataInt32(const char* columnName);
		long long int DLLCALLCONV getDataInt64(const char* columnName);
		float DLLCALLCONV getDataFloat32(const char* columnName);
		double DLLCALLCONV getDataFloat64(const char* columnName);
		float DLLCALLCONV getDataDecimal(const char* columnName);

		// get a column array value
		char DLLCALLCONV getDataBitArray(const char* columnName, int index);
		char DLLCALLCONV getDataInt8Array(const char* columnName, int index);
		short DLLCALLCONV getDataInt16Array(const char* columnName, int index);
		int DLLCALLCONV getDataInt32Array(const char* columnName, int index);
		long long int DLLCALLCONV getDataInt64Array(const char* columnName, int index);
		float DLLCALLCONV getDataFloat32Array(const char* columnName, int index);
		double DLLCALLCONV getDataFloat64Array(const char* columnName, int index);
		float DLLCALLCONV getDataDecimalArray(const char* columnName, int index);

		const char* DLLCALLCONV getDataString(const char* columnName);

	protected:
		void* checkAndGetColumnValuePtr(const char* columnName, int expectedType, int size = 1, int* columnIndex = 0);

	private:
		FieldOrder *savedOrder;
		std::vector<int*> indexedSizePtrs;
		bool *initializedFields;

		void **row;
		const DataDescriptor *columnsType;
		DataDescriptor columnsTypeOrdered;
		DataDescriptor savedColumnsTypeOrdered;
		FieldOrder *order;

		void initRow(bool initializedFields);

		void setOrderedMode();
		void setLinearMode();
};

} // namespace

#endif // ROWMANIPULATOR_H
