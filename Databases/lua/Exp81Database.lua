rdb.fields = {
	{"level", TYPE_INT32 | TYPE_RDBIGNORE | TYPE_FLAG_SORT},
	{"exp", TYPE_INT64},
	{"jp_0", TYPE_INT32},
	{"jp_1", TYPE_INT32},
	{"jp_2", TYPE_INT32},
	{"jp_3", TYPE_INT32},
}

rdb.defaultTableName = "LevelResource"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		row.level = rowNum + 1
	end
end
