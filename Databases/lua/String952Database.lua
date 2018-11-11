-- db_string.rdb with version 9.5.2 given by Ghost Informatics to be able to get its format

rdb.fields = {
	{"code", TYPE_INT32},
	{"group_id", TYPE_INT32},
	{"", TYPE_VARCHAR_SIZE | TYPE_SQLIGNORE, MAKEINDEXEDVAR(0, 1)},
	{"value", TYPE_NVARCHAR_STR, MAKEINDEXEDVAR(0, 3999)},
}