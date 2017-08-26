rdb.fields = {
	{"level", TYPE_INT32 | TYPE_RDBIGNORE | TYPE_FLAG_SORT},
	{"normal_exp", TYPE_INT64},
	{"growth_exp", TYPE_INT64},
	{"evolve_exp", TYPE_INT64},
}

rdb.defaultTableName = "SummonLevelResource"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		row.level = rowNum + 1
	end
end
