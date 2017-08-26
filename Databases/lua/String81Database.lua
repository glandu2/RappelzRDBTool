rdb.fields = {
	{"", TYPE_VARCHAR_SIZE | TYPE_SQLIGNORE, MAKEINDEXEDVAR(0, 1)},
	{"", TYPE_VARCHAR_SIZE | TYPE_SQLIGNORE, MAKEINDEXEDVAR(1, 1)},
	{"name", TYPE_VARCHAR_STR, MAKEINDEXEDVAR(0, 64)},
	{"value", TYPE_NVARCHAR_STR, MAKEINDEXEDVAR(1, 3999)},
	{"code", TYPE_INT32},
	{"group_id", TYPE_INT32},
	{"unknownValue0", TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE},
	{"unknownValue1", TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE},
	{"unknownValue2", TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE},
	{"unknownValue3", TYPE_INT32 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE},
}

rdb.sqlColumnOrder = table.concat({
	"name\0",
	"group_id\0",
	"code\0",
	"value\0",
})

rdb.csvColumnOrder = table.concat({
	"code\0",
	"group_id\0",
	"name\0",
	"value\0",
})

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst ~= DF_RDB then
		row.unknownValue0 = 0
		row.unknownValue1 = 0
		row.unknownValue2 = 0
		row.unknownValue3 = 0
	end
end
