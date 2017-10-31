rdb.fields = {
	{"id", TYPE_INT16 | TYPE_RDBIGNORE | TYPE_FLAG_SORT},
	{"string", TYPE_CHAR, 52},
}

rdb.defaultFileName = "db_item.ref"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		row.id = rowNum + 1
	end
end
