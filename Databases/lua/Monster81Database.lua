rdb.fields = {
	{"id", TYPE_INT32},
	{"monster_group", TYPE_INT32},
	{"monster_type", TYPE_INT32},
	{"name_id", TYPE_INT32},
	{"location_id", TYPE_INT32},
	{"race", TYPE_INT32},
	{"grp", TYPE_INT32},
	{"transform_level", TYPE_INT32},
	{"level", TYPE_INT32},
	{"size", TYPE_FLOAT32},
	{"scale", TYPE_FLOAT32},
	{"target_fx_size", TYPE_FLOAT32},
	{"walk_type", TYPE_INT32},
	{"slant_type", TYPE_INT32},
	{"visible_range", TYPE_INT32},
	{"chase_range", TYPE_INT32},
	{"magic_type", TYPE_INT32},
	{"f_fisrt_attack", TYPE_BIT},
	{"f_group_first_attack", TYPE_BIT},
	{"f_response_casting", TYPE_BIT},
	{"f_response_race", TYPE_BIT},
	{"f_response_battle", TYPE_BIT},
	{"padding0", TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE},
	{"padding1", TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE},
	{"hp", TYPE_INT32},
	{"mp", TYPE_INT32},
	{"attack_point", TYPE_INT32},
	{"magic_point", TYPE_INT32},
	{"defence", TYPE_INT32},
	{"magic_defence", TYPE_INT32},
	{"attack_speed", TYPE_INT32},
	{"attack_speed_type", TYPE_INT32 | TYPE_SQLIGNORE},
	{"magic_speed", TYPE_INT32},
	{"accuracy", TYPE_INT32},
	{"avoid", TYPE_INT32},
	{"magic_accuracy", TYPE_INT32},
	{"magic_avoid", TYPE_INT32},
	{"taming_id", TYPE_INT32},
	{"taming_percentage", TYPE_FLOAT32},
	{"taming_exp_mod", TYPE_FLOAT32},
	{"exp", TYPE_INT32},
	{"jp", TYPE_INT32},
	{"monster_skill_link_id", TYPE_INT32},
	{"stat_id", TYPE_INT32},
	{"ability", TYPE_INT32},
	{"standard_walk_spped", TYPE_INT32},
	{"standard_run_spped", TYPE_INT32},
	{"walk_speed", TYPE_INT32},
	{"run_speed", TYPE_INT32},
	{"attack_range", TYPE_DECIMAL, MAKEINDEXEDVAR(2, 1)},
	{"hidesense_range", TYPE_DECIMAL, MAKEINDEXEDVAR(2, 1)},
	{"gold_drop_percentage", TYPE_INT32},
	{"gold_min", TYPE_INT32},
	{"gold_max", TYPE_INT32},
	{"chaos_drop_percentage", TYPE_INT32},
	{"chaos_min", TYPE_INT32},
	{"chaos_max", TYPE_INT32},
	{"exp_2", TYPE_INT32},
	{"jp_2", TYPE_INT32},
	{"gold_min_2", TYPE_INT32},
	{"gold_max_2", TYPE_INT32},
	{"chaos_min_2", TYPE_INT32},
	{"chaos_max_2", TYPE_INT32},
	{"drop_table_link_id", TYPE_INT32},
	{"texture_group", TYPE_INT32},
	{"local_flag", TYPE_INT32},
	{"model", TYPE_CHAR, 256},
	{"motion_file_id", TYPE_INT32},
	{"weapon_type", TYPE_INT32},
	{"camera_x", TYPE_INT32},
	{"camera_y", TYPE_INT32},
	{"camera_z", TYPE_INT32},
	{"target_x", TYPE_FLOAT32},
	{"target_y", TYPE_FLOAT32},
	{"target_z", TYPE_FLOAT32},
	{"material", TYPE_INT32},
	{"attack_motion_speed", TYPE_INT32},
	{"fight_type", TYPE_INT32},
	{"script_on_dead", TYPE_CHAR | TYPE_RDBIGNORE},
}

rdb.sqlColumnOrder = table.concat({
	"id\0",
	"monster_group\0",
	"name_id\0",
	"location_id\0",
	"model\0",
	"motion_file_id\0",
	"transform_level\0",
	"walk_type\0",
	"slant_type\0",
	"size\0",
	"scale\0",
	"target_fx_size\0",
	"camera_x\0",
	"camera_y\0",
	"camera_z\0",
	"target_x\0",
	"target_y\0",
	"target_z\0",
	"level\0",
	"grp\0",
	"magic_type\0",
	"race\0",
	"visible_range\0",
	"chase_range\0",
	"f_fisrt_attack\0",
	"f_group_first_attack\0",
	"f_response_casting\0",
	"f_response_race\0",
	"f_response_battle\0",
	"monster_type\0",
	"stat_id\0",
	"fight_type\0",
	"monster_skill_link_id\0",
	"material\0",
	"weapon_type\0",
	"attack_motion_speed\0",
	"ability\0",
	"standard_walk_spped\0",
	"standard_run_spped\0",
	"walk_speed\0",
	"run_speed\0",
	"attack_range\0",
	"hidesense_range\0",
	"hp\0",
	"mp\0",
	"attack_point\0",
	"magic_point\0",
	"defence\0",
	"magic_defence\0",
	"attack_speed\0",
	"magic_speed\0",
	"accuracy\0",
	"avoid\0",
	"magic_accuracy\0",
	"magic_avoid\0",
	"taming_id\0",
	"taming_percentage\0",
	"taming_exp_mod\0",
	"exp\0",
	"jp\0",
	"gold_drop_percentage\0",
	"gold_min\0",
	"gold_max\0",
	"chaos_drop_percentage\0",
	"chaos_min\0",
	"chaos_max\0",
	"exp_2\0",
	"jp_2\0",
	"gold_min_2\0",
	"gold_max_2\0",
	"chaos_min_2\0",
	"chaos_max_2\0",
	"drop_table_link_id\0",
	"texture_group\0",
	"local_flag\0",
	"script_on_dead\0",
})

rdb.csvColumnOrder = table.concat({
	"id\0",
	"monster_group\0",
	"name_id\0",
	"location_id\0",
	"model\0",
	"motion_file_id\0",
	"transform_level\0",
	"walk_type\0",
	"slant_type\0",
	"size\0",
	"scale\0",
	"target_fx_size\0",
	"camera_x\0",
	"camera_y\0",
	"camera_z\0",
	"target_x\0",
	"target_y\0",
	"target_z\0",
	"level\0",
	"grp\0",
	"magic_type\0",
	"race\0",
	"visible_range\0",
	"chase_range\0",
	"f_fisrt_attack\0",
	"f_group_first_attack\0",
	"f_response_casting\0",
	"f_response_race\0",
	"f_response_battle\0",
	"monster_type\0",
	"stat_id\0",
	"fight_type\0",
	"monster_skill_link_id\0",
	"material\0",
	"weapon_type\0",
	"attack_motion_speed\0",
	"ability\0",
	"standard_walk_spped\0",
	"standard_run_spped\0",
	"walk_speed\0",
	"run_speed\0",
	"attack_range\0",
	"hidesense_range\0",
	"hp\0",
	"mp\0",
	"attack_point\0",
	"magic_point\0",
	"defence\0",
	"magic_defence\0",
	"attack_speed\0",
	"magic_speed\0",
	"accuracy\0",
	"avoid\0",
	"magic_accuracy\0",
	"magic_avoid\0",
	"taming_id\0",
	"taming_percentage\0",
	"taming_exp_mod\0",
	"exp\0",
	"jp\0",
	"gold_drop_percentage\0",
	"gold_min\0",
	"gold_max\0",
	"chaos_drop_percentage\0",
	"chaos_min\0",
	"chaos_max\0",
	"exp_2\0",
	"jp_2\0",
	"gold_min_2\0",
	"gold_max_2\0",
	"chaos_min_2\0",
	"chaos_max_2\0",
	"drop_table_link_id\0",
	"texture_group\0",
	"local_flag\0",
	"script_on_dead\0",
})

local encodeMap = {}
local decodeMap = {}

local function compute_bit_swapping()
	local j, oldValue

	for i = 0,31 do
		encodeMap[i] = i;
	end

	j = 3
	for i = 0,31 do
		oldValue = encodeMap[i]
		encodeMap[i] = encodeMap[j]
		encodeMap[j] = oldValue
		j = (j + i + 3) % 32
	end

	for i = 0,31 do
		decodeMap[encodeMap[i]] = i
	end
end

compute_bit_swapping()

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst == DF_RDB then
		local value = row.id
		local result = 0
		for i = 0,31 do
			if (1 << i) & value ~= 0 then
				result = result | (1 << decodeMap[i])
			end
		end
		row.id = result
		row.script_on_dead = ""
	elseif mode == DCT_Write and dst == DF_RDB then
		local value = row.id
		local result = 0
		for i = 0,31 do
			if (1 << i) & value ~= 0 then
				result = result | (1 << encodeMap[i])
			end
		end
		row.id = result
		row.padding0 = 0
		row.padding1 = 0
	elseif mode == DCT_Read and DF_SQL then
		row.attack_speed_type = 15
	end
end
