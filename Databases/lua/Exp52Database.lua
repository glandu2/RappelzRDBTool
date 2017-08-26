rdb.fields = {
	{"level", TYPE_INT32 | TYPE_RDBIGNORE | TYPE_FLAG_SORT},
	{"normal_exp", TYPE_INT64},
	{"jl1", TYPE_INT32},
	{"jl2", TYPE_INT32},
	{"jl3", TYPE_INT32},
	{"jl4", TYPE_INT32},
}

rdb.defaultTableName = "LevelResource"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		row.level = rowNum + 1
	end
end
