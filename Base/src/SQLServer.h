#ifndef SQLSERVER_H
#define SQLSERVER_H

#include "SQLLanguage.h"

namespace RappelzRDBBase {

class SQLServer : public SQLLanguage {
public:
	virtual void getTypeName(char* buffer, int type, int count = 1, int index = 0);
};

}  // namespace RappelzRDBBase

#endif  // SQLSERVER_H
