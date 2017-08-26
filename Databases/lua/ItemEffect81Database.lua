rdb.fields = {
	{"id", TYPE_INT32},
	{"ordinal_id", TYPE_INT32},
	{"tooltip_id", TYPE_INT32},
	{"min_level", TYPE_INT32 | TYPE_RDBIGNORE},
	{"max_level", TYPE_INT32 | TYPE_RDBIGNORE},
	{"effect_type", TYPE_INT8},
	{"effect_id", TYPE_INT16},
	{"effect_level", TYPE_INT16},
	{"value_0", TYPE_FLOAT64},
	{"value_1", TYPE_FLOAT64},
	{"value_2", TYPE_FLOAT64},
	{"value_3", TYPE_FLOAT64},
	{"value_4", TYPE_FLOAT64},
	{"value_5", TYPE_FLOAT64},
	{"value_6", TYPE_FLOAT64},
	{"value_7", TYPE_FLOAT64},
	{"value_8", TYPE_FLOAT64},
	{"value_9", TYPE_FLOAT64},
	{"value_10", TYPE_FLOAT64},
	{"value_11", TYPE_FLOAT64},
	{"value_12", TYPE_FLOAT64},
	{"value_13", TYPE_FLOAT64},
	{"value_14", TYPE_FLOAT64},
	{"value_15", TYPE_FLOAT64},
	{"value_16", TYPE_FLOAT64},
	{"value_17", TYPE_FLOAT64},
	{"value_18", TYPE_FLOAT64},
	{"value_19", TYPE_FLOAT64},
	{"unknown", TYPE_INT32 | TYPE_SQLIGNORE},
}

rdb.defaultFileName = "db_itemeffectresource"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		row.min_level = 0
		row.max_level = 0
	elseif mode == DCT_Read and dst == DF_SQL then
		row.unknown = 0
	end
end
