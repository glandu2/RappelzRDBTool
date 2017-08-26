rdb.fields = {
	{"motion_id", TYPE_INT32},
	{"race_id", TYPE_INT32},
	{"sex", TYPE_INT32},
	{"file_name", TYPE_CHAR, 256},
	{"frame_length", TYPE_INT32},
	{"play_speed", TYPE_INT32},
	{"graphic_effect_file_ID", TYPE_INT32},
	{"graphic_effect_speed_type", TYPE_INT32},
	{"graphic_effect_position", TYPE_INT32},
	{"graphic_effect_follow", TYPE_INT8},
	{"graphic_effect_disappear", TYPE_INT8},
	{"padding0", TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"sound_file_ID", TYPE_INT32},
	{"sound_volumn", TYPE_INT32},
	{"sound_play_probability", TYPE_INT32},
	{"sound_timing", TYPE_INT32},
	{"sound_reverb_apply", TYPE_INT8},
	{"sound_filter_apply", TYPE_INT8},
	{"sound_stereo_apply", TYPE_INT8},
	{"padding1", TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"fx_id", TYPE_INT32},
	{"fx_timing", TYPE_INT32},
	{"fx_position", TYPE_INT32},
	{"fx_play_speed", TYPE_INT32},
	{"event01_id", TYPE_INT32},
	{"event01_timing", TYPE_INT32},
	{"event02_id", TYPE_INT32},
	{"event02_timing", TYPE_INT32},
	{"event03_id", TYPE_INT32},
	{"event03_timing", TYPE_INT32},
	{"event04_id", TYPE_INT32},
	{"event04_timing", TYPE_INT32},
	{"event05_id", TYPE_INT32},
	{"event05_timing", TYPE_INT32},
	{"event06_id", TYPE_INT32},
	{"event06_timing", TYPE_INT32},
	{"event07_id", TYPE_INT32},
	{"event07_timing", TYPE_INT32},
	{"event08_id", TYPE_INT32},
	{"event08_timing", TYPE_INT32},
	{"event09_id", TYPE_INT32},
	{"event09_timing", TYPE_INT32},
	{"event10_id", TYPE_INT32},
	{"event10_timing", TYPE_INT32},
}

rdb.defaultTableName = "CharacterMotion"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and (dst == DF_CSV or dst == DF_SQL) then
		row.padding0 = 103
		row.padding1 = 0
	end
end
