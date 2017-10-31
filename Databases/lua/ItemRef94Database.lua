-- By Gangor
-- https://www.elitepvpers.com/forum/rappelz-private-server/2769975-release-rappelz-rdb-tool-tabs-21.html#post36438074

rdb.fields = {
	{"id", TYPE_INT16 | TYPE_RDBIGNORE | TYPE_FLAG_SORT},
	{"string", TYPE_CHAR, 52},
}

rdb.defaultFileName = "db_item.ref"

rdb.defaultTableName = "ItemReferenceResource"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		row.id = rowNum
	end
end
