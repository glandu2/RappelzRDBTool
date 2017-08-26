rdb.fields = {
	{"id", TYPE_INT32},
	{"type", TYPE_INT32},
	{"name_id", TYPE_INT32},
	{"cage_id", TYPE_INT32},
	{"rate", TYPE_INT32 | TYPE_SQLIGNORE},
	{"size", TYPE_FLOAT32},
	{"scale", TYPE_FLOAT32},
	{"target_fx_size", TYPE_FLOAT32},
	{"walk_type", TYPE_INT32},
	{"slant_type", TYPE_INT32},
	{"camera_x", TYPE_INT32},
	{"camera_y", TYPE_INT32},
	{"camera_z", TYPE_INT32},
	{"target_x", TYPE_FLOAT32},
	{"target_y", TYPE_FLOAT32},
	{"target_z", TYPE_FLOAT32},
	{"model", TYPE_CHAR, 256},
	{"motion_file_id", TYPE_INT32},
	{"texture_group", TYPE_INT32},
	{"local_flag", TYPE_INT32},
}

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_SQL then
		local id = row.id;

		if id >= 500 and id < 600 then
			row.rate = 21
		else
			row.rate = 5
		end
	end
end
