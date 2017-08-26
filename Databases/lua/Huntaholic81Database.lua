rdb.fields = {
	{"id", TYPE_INT32},
	{"name_id", TYPE_INT32},
	{"local_flag", TYPE_INT32},
	{"hunting_period", TYPE_INT32},
	{"objective_point", TYPE_INT32},
	{"max_point", TYPE_INT32 | TYPE_RDBIGNORE},
	{"lobby_channel_id", TYPE_INT32 | TYPE_RDBIGNORE},
	{"lobby_x", TYPE_INT32 | TYPE_RDBIGNORE},
	{"lobby_y", TYPE_INT32 | TYPE_RDBIGNORE},
	{"dungeon_channel_id", TYPE_INT32 | TYPE_RDBIGNORE},
	{"dungeon_x", TYPE_INT32 | TYPE_RDBIGNORE},
	{"dungeon_y", TYPE_INT32 | TYPE_RDBIGNORE},
}

rdb.defaultFileName = "db_huntaholicresource"

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		row.max_point = 6000
		row.lobby_channel_id = 300011
		row.lobby_x = 83719
		row.lobby_y = 2544
		row.dungeon_channel_id = 300000
		row.dungeon_x = 88935
		row.dungeon_y = 8622
	end
end
