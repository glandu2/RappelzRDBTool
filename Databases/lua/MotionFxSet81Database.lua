rdb.fields = {
	{"id", TYPE_INT32},
	{"fx_type", TYPE_INT32},
	{"add_time", TYPE_INT32},
	{"following", TYPE_INT32},
	{"graphic_effect_start_file_ID", TYPE_INT32},
	{"graphic_effect_middle_file_ID", TYPE_INT32},
	{"graphic_effect_end_file_ID", TYPE_INT32},
	{"graphic_effect_direction", TYPE_INT32},
	{"is_effect0", TYPE_INT8 | TYPE_SQLIGNORE},
	{"is_effect1", TYPE_INT8 | TYPE_SQLIGNORE},
	{"is_effect2", TYPE_INT8 | TYPE_SQLIGNORE},
	{"unknown0", TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE},
	{"sound_start_file_ID", TYPE_INT32},
	{"sound_middle_file_ID", TYPE_INT32},
	{"sound_end_file_ID", TYPE_INT32},
	{"sound_play_probability", TYPE_INT32},
	{"sound_volumn", TYPE_INT32},
	{"is_sound0", TYPE_INT8 | TYPE_SQLIGNORE},
	{"is_sound1", TYPE_INT8 | TYPE_SQLIGNORE},
	{"is_sound2", TYPE_INT8 | TYPE_SQLIGNORE},
	{"sound_reverb_apply", TYPE_INT8},
	{"sound_filter_apply", TYPE_INT8},
	{"sound_stereo_apply", TYPE_INT8},
	{"sound_option0", TYPE_INT8 | TYPE_SQLIGNORE},
	{"sound_option1", TYPE_INT8 | TYPE_SQLIGNORE},
	{"sound_option2", TYPE_INT8 | TYPE_SQLIGNORE},
	{"kagValue", TYPE_CHAR | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, 3},
}

rdb.defaultTableName = "MotionFXSet"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst ~= DF_RDB then
		row.unknown0 = 1
		row.kagValue = "kag"

		if dst == DF_SQL then
			row.is_effect0 = row.graphic_effect_start_file_ID ~= 0
			row.is_effect1 = row.graphic_effect_middle_file_ID ~= 0
			row.is_effect2 = row.graphic_effect_end_file_ID ~= 0

			row.is_sound0 = row.sound_start_file_ID ~= 0
			row.is_sound1 = row.sound_middle_file_ID ~= 0
			row.is_sound2 = row.sound_end_file_ID ~= 0

			row.sound_option0 = 0
			row.sound_option1 = 0
			row.sound_option2 = 0
		end
	end
end
