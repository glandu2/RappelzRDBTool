rdb.fields = {
	{"id", TYPE_INT32},
	{"local_flag", TYPE_INT32},
	{"unknown1", TYPE_INT32 | TYPE_SQLIGNORE},
	{"dungeon_level", TYPE_INT32},
	{"raid_start_pos_x", TYPE_INT32},
	{"raid_start_pos_y", TYPE_INT32},
	{"defence_pos_x", TYPE_INT32},
	{"defence_pos_y", TYPE_INT32},
	{"connector_id", TYPE_INT32},
	{"connector_pos_x", TYPE_INT32},
	{"connector_pos_y", TYPE_INT32},
	{"connector_direction", TYPE_INT32},
	{"core_id", TYPE_INT32},
	{"core_pos_x", TYPE_INT32},
	{"core_pos_y", TYPE_INT32},
	{"core_offset_x", TYPE_FLOAT32},
	{"core_offset_y", TYPE_FLOAT32},
	{"core_offset_z", TYPE_FLOAT32},
	{"core_around_x", TYPE_FLOAT32},
	{"core_around_y", TYPE_FLOAT32},
	{"core_around_z", TYPE_FLOAT32},
	{"scale_x", TYPE_FLOAT32},
	{"scale_y", TYPE_FLOAT32},
	{"scale_z", TYPE_FLOAT32},
	{"is_height_locked", TYPE_INT8},
	{"lock_height", TYPE_FLOAT32},
	{"boss01_id", TYPE_INT32},
	{"boss02_id", TYPE_INT32},
	{"raid_opening_time", TYPE_INT32},
	{"raid_closing_time", TYPE_INT32},
	{"siege_opening_time", TYPE_INT32},
	{"siege_closing_time", TYPE_INT32},
	{"seamless_x", TYPE_INT32},
	{"seamless_y", TYPE_INT32},
	{"num_party_guild", TYPE_INT32},
	{"num_party_mercenary", TYPE_INT32},
	{"num_party_raid", TYPE_INT32 | TYPE_RDBIGNORE},
}

rdb.defaultFileName = "db_dungeonresource"

rdb.convertData = function (dst, mode, row, rowNum)
	if dst == DF_SQL and mode == DCT_Read then
		row.unknown1 = row.connector_id == 0
	elseif dst == DF_RDB and mode == DCT_Read then
		row.num_party_raid = row.num_party_guild / 2
	end
end
