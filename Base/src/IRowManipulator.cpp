#include "IRowManipulator.h"
#include "RowManipulator.h"

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:createRowManipulator=_createRowManipulator@12")
#endif
IRowManipulator * DLLCALLCONV createRowManipulator(const DataDescriptor* columnsType, FieldOrder *order, void **initValue) {
	try {
		return new RappelzRDBBase::RowManipulator(columnsType, order, initValue);
	} catch(...) {
		return 0;
	}
}

