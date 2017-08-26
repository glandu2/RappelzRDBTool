rdb.fields = {
	{"id", TYPE_INT32},
	{"text_id_quest", TYPE_INT32},
	{"text_id_summary", TYPE_INT32},
	{"text_id_status", TYPE_INT32},
	{"limit_begin_time", TYPE_INT32},
	{"limit_end_time", TYPE_INT32},
	{"limit_level", TYPE_INT32},
	{"limit_job_level", TYPE_INT32},
	{"limit_max_level", TYPE_INT32},
	{"limit_max_joblevel", TYPE_INT32},
	{"nv", TYPE_BIT | TYPE_SQLIGNORE},
	{"limit_deva", TYPE_BIT},
	{"limit_asura", TYPE_BIT},
	{"limit_gaia", TYPE_BIT},
	{"limit_fighter", TYPE_BIT},
	{"limit_hunter", TYPE_BIT},
	{"limit_magician", TYPE_BIT},
	{"limit_summoner", TYPE_BIT},
	{"unknown0", TYPE_INT8},
	{"unknown1", TYPE_INT8},
	{"unknown2", TYPE_INT8},
	{"limit_job", TYPE_INT32},
	{"limit_favor_group_id", TYPE_INT32 | TYPE_RDBIGNORE},
	{"limit_favor", TYPE_INT32},
	{"repeatable", TYPE_INT32},
	{"invoke_condition", TYPE_INT32},
	{"invoke_value", TYPE_INT32},
	{"time_limit_type", TYPE_INT16},
	{"limit_job_depth", TYPE_INT16},
	{"time_limit", TYPE_INT32},
	{"type", TYPE_INT32},
	{"value1", TYPE_INT32},
	{"value2", TYPE_INT32},
	{"value3", TYPE_INT32},
	{"value4", TYPE_INT32},
	{"value5", TYPE_INT32},
	{"value6", TYPE_INT32},
	{"value7", TYPE_INT32},
	{"value8", TYPE_INT32},
	{"value9", TYPE_INT32},
	{"value10", TYPE_INT32},
	{"value11", TYPE_INT32},
	{"value12", TYPE_INT32},
	{"drop_group_id", TYPE_INT32},
	{"quest_difficulty", TYPE_INT32},
	{"favor_group_id", TYPE_INT32 | TYPE_RDBIGNORE},
	{"hate_group_id", TYPE_INT32 | TYPE_RDBIGNORE},
	{"favor", TYPE_INT32},
	{"exp", TYPE_INT32},
	{"jp", TYPE_INT32},
	{"holicpoint", TYPE_INT32},
	{"gold", TYPE_INT32},
	{"default_reward_id", TYPE_INT32},
	{"default_reward_level", TYPE_INT32},
	{"default_reward_quantity", TYPE_INT32},
	{"optional_reward_id1", TYPE_INT32},
	{"optional_reward_level1", TYPE_INT32},
	{"optional_reward_quantity1", TYPE_INT32},
	{"optional_reward_id2", TYPE_INT32},
	{"optional_reward_level2", TYPE_INT32},
	{"optional_reward_quantity2", TYPE_INT32},
	{"optional_reward_id3", TYPE_INT32},
	{"optional_reward_level3", TYPE_INT32},
	{"optional_reward_quantity3", TYPE_INT32},
	{"optional_reward_id4", TYPE_INT32},
	{"optional_reward_level4", TYPE_INT32},
	{"optional_reward_quantity4", TYPE_INT32},
	{"optional_reward_id5", TYPE_INT32},
	{"optional_reward_level5", TYPE_INT32},
	{"optional_reward_quantity5", TYPE_INT32},
	{"optional_reward_id6", TYPE_INT32},
	{"optional_reward_level6", TYPE_INT32},
	{"optional_reward_quantity6", TYPE_INT32},
	{"forequest1", TYPE_INT32},
	{"forequest2", TYPE_INT32},
	{"forequest3", TYPE_INT32},
	{"or_flag", TYPE_INT8},
	{"unknownFlag", TYPE_INT16 | TYPE_SQLIGNORE},
	{"unknownPadding", TYPE_INT8 | TYPE_SQLIGNORE},
}

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst ~= DF_SQL then
		local id = row.id;

		if id >= 3600 and id <= 3611 and id ~= 3610 then
			row.hate_group_id = 1
		else
			row.hate_group_id = 0
		end

		row.nv = 0
		row.unknown0 = 0
		row.unknown1 = 0
		row.unknown2 = 0
		row.unknownPadding = 0
		row.limit_favor_group_id = 0
		row.favor_group_id = 999
	end
end
