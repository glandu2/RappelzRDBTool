#ifndef DATABASE_H
#define DATABASE_H

#include "IDatabase.h"
#include "DataType.h"

#include <vector>

class IDatabaseDescription;
class IDataSource;

namespace RappelzRDBBase {

class RowManipulator;

class Database : public CImplement<IDatabase>
{
	public:
		Database(IDatabaseDescription *databaseDescription);
		virtual ~Database();

		virtual const FieldOrder * DLLCALLCONV getFieldOrder(eDataSourceType type);

		virtual int DLLCALLCONV readData(eDataSourceType type, const char* source = 0, void (DLLCALLCONV *progressCallBack)(void*, int, int) = 0, void *arg = 0, const char* location = 0, const char* user = 0, const char* password = 0, const char* options = 0);
		virtual int DLLCALLCONV writeData(eDataSourceType type, const char* source = 0, void (DLLCALLCONV *progressCallBack)(void*, int, int) = 0, void *arg = 0, const char* location = 0, const char* user = 0, const char* password = 0, const char* options = 0);

		virtual void DLLCALLCONV close(void (DLLCALLCONV *progressCallBack)(void*, int, int), void *arg = 0);

		virtual IRowManipulator * DLLCALLCONV getRowManipulator();
		const std::vector<void**> * getRows();

		virtual inline int DLLCALLCONV getRowCount() { return dataList->size(); }
		virtual inline void** DLLCALLCONV getRowAt(int i) { return dataList->at(i); }

		virtual void DLLCALLCONV setUserData(void* data);
		virtual void* DLLCALLCONV getUserData();

		virtual inline IDatabaseDescription* DLLCALLCONV getDatabaseDescription() { return databaseDescription; }

		virtual unsigned long long int DLLCALLCONV getDate() { return date; }

	protected:
		void getDataSourceInfo(eDataSourceType type, IDataSource **ds, FieldOrder **associatedOrder, eDataFormat *associatedFormat);
		static FieldOrder getFieldOrderFromColumnName(DataDescriptor *dataDescription, const char *columns);

	private:
		std::vector<void**> *dataList;
		DataDescriptor dataDescription;
		RowManipulator *rowManipulator;
		FieldOrder sqlOrder;
		FieldOrder csvOrder;
		IDatabaseDescription *databaseDescription;
		unsigned long long int date;
		void *userData;
};

} //namespace

#endif // DATABASE_H
