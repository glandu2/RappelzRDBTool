rdb.fields = {
	{"id", TYPE_INT32},
	{"text_id", TYPE_INT32},
	{"stati_id", TYPE_INT32},
	{"skill_tree_id", TYPE_INT32},
	{"job_class", TYPE_INT8},
	{"job_depth", TYPE_INT8},
	{"up_lv", TYPE_INT16},
	{"up_jlv", TYPE_INT16},
	{"avable_job_0", TYPE_INT16},
	{"avable_job_1", TYPE_INT16},
	{"avable_job_2", TYPE_INT16},
	{"avable_job_3", TYPE_INT16},
	{"unknown_value", TYPE_INT16 | TYPE_SQLIGNORE},
	{"icon_id", TYPE_INT32},
	{"icon_file_name", TYPE_CHAR, 256},
}

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_SQL then
		row.unknown_value = 163
	end
end
