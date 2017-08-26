rdb.fields = {
	{"id", TYPE_INT32},
	{"text_id", TYPE_INT32},
	{"tooltip_id", TYPE_INT32},
	{"type", TYPE_INT32},
	{"local_flag", TYPE_INT32},
	{"casting_time", TYPE_INT32},
	{"use_count", TYPE_INT32},
	{"regen_time", TYPE_INT32},
	{"life_time", TYPE_INT32},
	{"casting_range", TYPE_FLOAT32},
	{"target_fx_size", TYPE_FLOAT32},
	{"limit_min_level", TYPE_INT32},
	{"limit_max_level", TYPE_INT32},
	{"limit_deva", TYPE_INT8},
	{"limit_asura", TYPE_INT8},
	{"limit_gaia", TYPE_INT8},
	{"limit_fighter", TYPE_INT8},
	{"limit_hunter", TYPE_INT8},
	{"limit_magician", TYPE_INT8},
	{"limit_summoner", TYPE_INT8},
	{"limit_job", TYPE_INT32},
	{"activation_condition", TYPE_INT32},
	{"activation_value1", TYPE_INT32},
	{"activation_value2", TYPE_INT32},
	{"activation2_condition", TYPE_INT32},
	{"activation2_value1", TYPE_INT32},
	{"activation2_value2", TYPE_INT32},
	{"activate_id", TYPE_INT32},
	{"drop_item_id_00", TYPE_INT32},
	{"drop_item_ratio_00", TYPE_INT32},
	{"drop_item_count_min_00", TYPE_INT32},
	{"drop_item_count_max_00", TYPE_INT32},
	{"drop_item_level_min_00", TYPE_INT32},
	{"drop_item_level_max_00", TYPE_INT32},
	{"drop_item_id_01", TYPE_INT32},
	{"drop_item_ratio_01", TYPE_INT32},
	{"drop_item_count_min_01", TYPE_INT32},
	{"drop_item_count_max_01", TYPE_INT32},
	{"drop_item_level_min_01", TYPE_INT32},
	{"drop_item_level_max_01", TYPE_INT32},
	{"script_text", TYPE_CHAR, 256},
	{"file_name", TYPE_CHAR, 256},
	{"terrain_check", TYPE_INT32},
	{"worldmap_check_type", TYPE_INT16},
}

rdb.defaultFileName = "db_fieldpropresource"
