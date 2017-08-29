rdb.fields = {
	{"id", TYPE_INT32},
	{"model_id", TYPE_INT32},
	{"name_id", TYPE_INT32},
	{"type", TYPE_INT32},
	{"affiliation", TYPE_INT32},
	{"species_id", TYPE_INT32},
	{"color", TYPE_INT32},
	{"magic_type", TYPE_INT32},
	{"rate", TYPE_INT8},
	{"stat_id", TYPE_INT32},
	{"size", TYPE_FLOAT32},
	{"scale", TYPE_FLOAT32},
	{"target_fx_size", TYPE_FLOAT32},
	{"standard_walk_speed", TYPE_INT32},
	{"standard_run_speed", TYPE_INT32},
	{"riding_speed", TYPE_INT32},
	{"run_speed", TYPE_INT32},
	{"is_riding_only", TYPE_INT8},
	{"riding_motion_type", TYPE_INT32},
	{"attack_range", TYPE_DECIMAL, MAKEINDEXEDVAR(2, 1)},
	{"walk_type", TYPE_INT32},
	{"slant_type", TYPE_INT32},
	{"material", TYPE_INT32},
	{"weapon_type", TYPE_INT32},
	{"attack_motion_speed", TYPE_INT32},
	{"form", TYPE_INT32},
	{"evolve_target", TYPE_INT32},
	{"camera_x", TYPE_INT32},
	{"camera_y", TYPE_INT32},
	{"camera_z", TYPE_INT32},
	{"target_x", TYPE_FLOAT32},
	{"target_y", TYPE_FLOAT32},
	{"target_z", TYPE_FLOAT32},
	{"model", TYPE_CHAR, 256},
	{"motion_file_id", TYPE_INT32},
	{"face_id", TYPE_INT32},
	{"face_file_name", TYPE_CHAR, 256},
	{"card_id", TYPE_INT32},
	{"script_text", TYPE_CHAR | TYPE_RDBIGNORE, 256},
	{"illust_file_name", TYPE_CHAR, 256},
	{"text_feature_id", TYPE_INT32},
	{"text_name_id", TYPE_INT32},
	{"skill_tree1_id", TYPE_INT32},
	{"skill_tree2_id", TYPE_INT32},
	{"skill_tree3_id", TYPE_INT32},
	{"skill_tree4_id", TYPE_INT32},
	{"skill_tree5_id", TYPE_INT32},
	{"texture_group", TYPE_INT32},
	{"local_flag", TYPE_INT32},
}

rdb.defaultFileName = "db_creature"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		local id = row.id

		if id == 540017 or id == 540021 then
			row.script_text = "creature_learn_riding()"
		else
			row.script_text = "0"
		end
	end
end
