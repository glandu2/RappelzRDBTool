#ifndef IDATABASE_H
#define IDATABASE_H

#include "IObject.h"
#include "DataType.h"
#include "ExportDLL.h"

class IRowManipulator;
class IDataSource;
class IDatabaseDescription;

class IDatabase : public IObject
{
	public:
		virtual const FieldOrder * DLLCALLCONV getFieldOrder(eDataSourceType type) = 0;

		virtual int DLLCALLCONV readData(eDataSourceType type, const char* source = 0, void (DLLCALLCONV *progressCallBack)(void*, int, int) = 0, void *arg = 0, const char *location = 0, const char* options = 0) = 0;
		virtual int DLLCALLCONV writeData(eDataSourceType type, const char* source = 0, void (DLLCALLCONV *progressCallBack)(void*, int, int) = 0, void *arg = 0, const char *location = 0, const char* options = 0) = 0;

		virtual void DLLCALLCONV close(void (DLLCALLCONV *progressCallBack)(void*, int, int), void *arg = 0) = 0;

		virtual IRowManipulator * DLLCALLCONV getRowManipulator() = 0;

		virtual int DLLCALLCONV getRowCount() = 0;
		virtual void** DLLCALLCONV getRowAt(int i) = 0;

		virtual void DLLCALLCONV setUserData(void* data) = 0;
		virtual void* DLLCALLCONV getUserData() = 0;

		virtual IDatabaseDescription* DLLCALLCONV getDatabaseDescription() = 0;

		virtual unsigned long long int DLLCALLCONV getDate() = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

EBASEDLL IDatabase * DLLCALLCONV createDatabase(IDatabaseDescription *databaseDescription);

#ifdef __cplusplus
}
#endif

#endif // IDATABASE_H
