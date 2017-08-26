rdb.fields = {
	{"id", TYPE_INT32},
	{"category", TYPE_INT32},
	{"value0", TYPE_INT32},
	{"value1", TYPE_INT32},
	{"value2", TYPE_INT32},
	{"is_set", TYPE_BIT | TYPE_RDBIGNORE},
	{"skip_db_update", TYPE_BIT | TYPE_RDBIGNORE},
}

rdb.defaultFileName = "db_titleconditiontyperesource"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		row.is_set = 0
		row.skip_db_update = 0
	end
end
