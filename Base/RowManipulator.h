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

		void * DLLCALLCONV getValuePtr(int colPos);
		inline void * DLLCALLCONV getValuePtr(const char* columnName) { return getValuePtr(getColumnIndex(columnName)); }

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
