#include "SQLServer.h"
#include "DataType.h"
#include <stdio.h>

namespace RappelzRDBBase {

void SQLServer::getTypeName(char *buffer, int type, int count, int index) {
	switch(type) {
		case TYPE_INT8:
			sprintf(buffer, "tinyint");
			break;

		case TYPE_INT16:
			sprintf(buffer, "smallint");
			break;

		case TYPE_INT32:
			sprintf(buffer, "int");
			break;

		case TYPE_DECIMAL:
			sprintf(buffer, "decimal(10,%d)", index);
			break;

		case TYPE_BIT:
			sprintf(buffer, "tinyint");
			break;

		case TYPE_INT64:
			sprintf(buffer, "bigint");
			break;

		case TYPE_FLOAT32:
			sprintf(buffer, "real");
			break;

		case TYPE_FLOAT64:
			sprintf(buffer, "float(53)");
			break;

		case TYPE_CHAR:
		case TYPE_VARCHAR_STR:
			sprintf(buffer, "varchar(%d)", count);
			break;
	}
}

} //namespace
