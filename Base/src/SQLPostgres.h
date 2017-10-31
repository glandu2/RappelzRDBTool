#ifndef SQLPOSTGRES_H
#define SQLPOSTGRES_H

#include "SQLLanguage.h"

namespace RappelzRDBBase {

class SQLPostgres : public SQLLanguage {
public:
	virtual void getTypeName(char* buffer, int type, int count = 1, int index = 0);
};

}  // namespace RappelzRDBBase

#endif  // SQLPOSTGRES_H
