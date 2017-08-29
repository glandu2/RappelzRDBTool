rdb.fields = {
	{"id", TYPE_INT32},
	{"name_id", TYPE_INT32},
	{"type", TYPE_INT32},
	{"group", TYPE_INT32},
	{"class", TYPE_INT32},
	{"set_id", TYPE_INT32},
	{"set_part_flag", TYPE_INT32},
	{"rank", TYPE_INT32},
	{"level", TYPE_INT32},
	{"enhance", TYPE_INT32},
	{"socket", TYPE_INT32},
	{"status_flag", TYPE_INT32},
	{"use_min_level", TYPE_INT32},
	{"use_max_level", TYPE_INT32},
	{"target_min_level", TYPE_INT32},
	{"target_max_level", TYPE_INT32},
	{"range", TYPE_DECIMAL, MAKEINDEXEDVAR(2, 1)},
	{"weight", TYPE_FLOAT32},
	{"price", TYPE_INT32},
	{"huntaholic_point", TYPE_INT32},
	{"endurance", TYPE_INT32},
	{"wear_type", TYPE_INT32},
	{"nv0", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv1", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"limit_deva", TYPE_BIT},
	{"limit_asura", TYPE_BIT},
	{"limit_gaia", TYPE_BIT},
	{"nv2", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv3", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv4", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv5", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv6", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"limit_fighter", TYPE_BIT},
	{"limit_hunter", TYPE_BIT},
	{"limit_magician", TYPE_BIT},
	{"limit_summoner", TYPE_BIT},
	{"nv7", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv8", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv9", TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"flag_cashitem", TYPE_BIT},
	{"flag_wear", TYPE_BIT},
	{"flag_storage", TYPE_BIT},
	{"flag_enhance", TYPE_BIT},
	{"flag_use", TYPE_BIT},
	{"nv10", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv11", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"flag_duplicate", TYPE_BIT},
	{"flag_target_use", TYPE_BIT},
	{"nv12", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"flag_trade", TYPE_BIT},
	{"flag_sell", TYPE_BIT},
	{"flag_quest", TYPE_BIT},
	{"flag_overweight", TYPE_BIT},
	{"flag_riding", TYPE_BIT},
	{"flag_drop", TYPE_BIT},
	{"flag_move", TYPE_BIT},
	{"flag_sit", TYPE_BIT},
	{"flag_raid", TYPE_BIT},
	{"flag_secroute", TYPE_BIT},
	{"flag_eventmap", TYPE_BIT},
	{"flag_huntaholic", TYPE_BIT},
	{"nv13", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv14", TYPE_BIT | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"nv15", TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"material", TYPE_INT32},
	{"summon_id", TYPE_INT32},
	{"throw_range", TYPE_INT32},
	{"distribute_type", TYPE_BIT},
	{"base_type_0", TYPE_INT16},
	{"base_type_1", TYPE_INT16},
	{"base_type_2", TYPE_INT16},
	{"base_type_3", TYPE_INT16},
	{"base_var1_0", TYPE_FLOAT64},
	{"base_var1_1", TYPE_FLOAT64},
	{"base_var1_2", TYPE_FLOAT64},
	{"base_var1_3", TYPE_FLOAT64},
	{"base_var2_0", TYPE_FLOAT64},
	{"base_var2_1", TYPE_FLOAT64},
	{"base_var2_2", TYPE_FLOAT64},
	{"base_var2_3", TYPE_FLOAT64},
	{"opt_type_0", TYPE_INT16},
	{"opt_type_1", TYPE_INT16},
	{"opt_type_2", TYPE_INT16},
	{"opt_type_3", TYPE_INT16},
	{"opt_var1_0", TYPE_FLOAT64},
	{"opt_var1_1", TYPE_FLOAT64},
	{"opt_var1_2", TYPE_FLOAT64},
	{"opt_var1_3", TYPE_FLOAT64},
	{"opt_var2_0", TYPE_FLOAT64},
	{"opt_var2_1", TYPE_FLOAT64},
	{"opt_var2_2", TYPE_FLOAT64},
	{"opt_var2_3", TYPE_FLOAT64},
	{"enhance_0_id", TYPE_INT16},
	{"enhance_1_id", TYPE_INT16},
	{"enhance_0_01", TYPE_FLOAT32},
	{"enhance_0_02", TYPE_FLOAT32},
	{"enhance_0_03", TYPE_FLOAT32},
	{"enhance_0_04", TYPE_FLOAT32},
	{"enhance_1_01", TYPE_FLOAT32},
	{"enhance_1_02", TYPE_FLOAT32},
	{"enhance_1_03", TYPE_FLOAT32},
	{"enhance_1_04", TYPE_FLOAT32},
	{"skill_id", TYPE_INT32},
	{"state_id", TYPE_INT32},
	{"state_level", TYPE_INT32},
	{"state_time", TYPE_INT32},
	{"state_type", TYPE_INT8},
	{"cool_time", TYPE_INT32},
	{"cool_time_group", TYPE_INT16},
	{"available_period", TYPE_INT32},
	{"decrease_type", TYPE_INT8},
	{"unknown3", TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"unknown4", TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE},
	{"model_type_dem", TYPE_INT32},
	{"model_type_def", TYPE_INT32},
	{"model_type_asm", TYPE_INT32},
	{"model_type_asf", TYPE_INT32},
	{"model_type_gam", TYPE_INT32},
	{"model_type_gaf", TYPE_INT32},
	{"deco_model_change", TYPE_INT8},
	{"model_00", TYPE_CHAR, 256},
	{"model_01", TYPE_CHAR, 256},
	{"model_02", TYPE_CHAR, 256},
	{"model_03", TYPE_CHAR, 256},
	{"model_04", TYPE_CHAR, 256},
	{"model_05", TYPE_CHAR, 256},
	{"model_06", TYPE_CHAR, 256},
	{"model_07", TYPE_CHAR, 256},
	{"model_08", TYPE_CHAR, 256},
	{"model_09", TYPE_CHAR, 256},
	{"model_10", TYPE_CHAR, 256},
	{"model_11", TYPE_CHAR, 256},
	{"model_12", TYPE_CHAR, 256},
	{"model_13", TYPE_CHAR, 256},
	{"model_14", TYPE_CHAR, 256},
	{"model_15", TYPE_CHAR, 256},
	{"model_16", TYPE_CHAR, 256},
	{"model_17", TYPE_CHAR, 256},
	{"drop_type", TYPE_CHAR, 256},
	{"unkCatValue", TYPE_CHAR | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, 3},
	{"icon_id", TYPE_INT32},
	{"icon_file_name", TYPE_CHAR, 256},
	{"script_text", TYPE_CHAR, 512},
	{"tooltip_id", TYPE_INT32},
}

rdb.sqlColumnOrder = table.concat({
	"id\0",
	"name_id\0",
	"tooltip_id\0",
	"type\0",
	"group\0",
	"class\0",
	"wear_type\0",
	"set_id\0",
	"set_part_flag\0",
	"rank\0",
	"level\0",
	"enhance\0",
	"socket\0",
	"status_flag\0",
	"limit_deva\0",
	"limit_asura\0",
	"limit_gaia\0",
	"limit_fighter\0",
	"limit_hunter\0",
	"limit_magician\0",
	"limit_summoner\0",
	"use_min_level\0",
	"use_max_level\0",
	"target_min_level\0",
	"target_max_level\0",
	"range\0",
	"weight\0",
	"price\0",
	"huntaholic_point\0",
	"endurance\0",
	"material\0",
	"summon_id\0",
	"flag_cashitem\0",
	"flag_wear\0",
	"flag_use\0",
	"flag_target_use\0",
	"flag_duplicate\0",
	"flag_drop\0",
	"flag_trade\0",
	"flag_sell\0",
	"flag_storage\0",
	"flag_overweight\0",
	"flag_riding\0",
	"flag_move\0",
	"flag_sit\0",
	"flag_enhance\0",
	"flag_quest\0",
	"flag_raid\0",
	"flag_secroute\0",
	"flag_eventmap\0",
	"flag_huntaholic\0",
	"available_period\0",
	"decrease_type\0",
	"throw_range\0",
	"distribute_type\0",
	"base_type_0\0",
	"base_var1_0\0",
	"base_var2_0\0",
	"base_type_1\0",
	"base_var1_1\0",
	"base_var2_1\0",
	"base_type_2\0",
	"base_var1_2\0",
	"base_var2_2\0",
	"base_type_3\0",
	"base_var1_3\0",
	"base_var2_3\0",
	"opt_type_0\0",
	"opt_var1_0\0",
	"opt_var2_0\0",
	"opt_type_1\0",
	"opt_var1_1\0",
	"opt_var2_1\0",
	"opt_type_2\0",
	"opt_var1_2\0",
	"opt_var2_2\0",
	"opt_type_3\0",
	"opt_var1_3\0",
	"opt_var2_3\0",
	"enhance_0_id\0",
	"enhance_0_01\0",
	"enhance_0_02\0",
	"enhance_0_03\0",
	"enhance_0_04\0",
	"enhance_1_id\0",
	"enhance_1_01\0",
	"enhance_1_02\0",
	"enhance_1_03\0",
	"enhance_1_04\0",
	"skill_id\0",
	"state_id\0",
	"state_level\0",
	"state_time\0",
	"state_type\0",
	"cool_time\0",
	"cool_time_group\0",
	"model_type_dem\0",
	"model_type_def\0",
	"model_type_asm\0",
	"model_type_asf\0",
	"model_type_gam\0",
	"model_type_gaf\0",
	"deco_model_change\0",
	"model_00\0",
	"model_01\0",
	"model_02\0",
	"model_03\0",
	"model_04\0",
	"model_05\0",
	"model_06\0",
	"model_07\0",
	"model_08\0",
	"model_09\0",
	"model_10\0",
	"model_11\0",
	"model_12\0",
	"model_13\0",
	"model_14\0",
	"model_15\0",
	"model_16\0",
	"model_17\0",
	"drop_type\0",
	"icon_id\0",
	"icon_file_name\0",
	"script_text\0",
})

local set_id_table = {
	[105701] = 1111,
	[107703] = 1113,
	[107723] = 1113,
	[110701] = 1111,
	[110721] = 1111,
	[110702] = 1112,
	[110722] = 1112,
	[110703] = 1113,
	[110723] = 1113,
	[111701] = 1111,
	[271722] = 1112,
	[271703] = 1113,
	[271723] = 1113,
	[105721] = 1111,
	[111721] = 1111,
	[272701] = 1111,
	[272721] = 1111,
	[272702] = 1112,
	[272722] = 1112,
	[103701] = 1111,
	[103721] = 1111,
	[105702] = 1112,
	[105722] = 1112,
	[105703] = 1113,
	[105723] = 1113,
	[111702] = 1112,
	[111722] = 1112,
	[111703] = 1113,
	[111723] = 1113,
	[103702] = 1112,
	[272703] = 1113,
	[700257] = 1,
	[272723] = 1113,
	[101701] = 1111,
	[101721] = 1111,
	[103722] = 1112,
	[103703] = 1113,
	[103723] = 1113,
	[108701] = 1111,
	[108721] = 1111,
	[108702] = 1112,
	[101702] = 1112,
	[108722] = 1112,
	[302730] = 1112,
	[101722] = 1112,
	[101703] = 1113,
	[101723] = 1113,
	[108703] = 1113,
	[108723] = 1113,
	[112701] = 1111,
	[112721] = 1111,
	[273701] = 1111,
	[273721] = 1111,
	[273702] = 1112,
	[273722] = 1112,
	[273703] = 1113,
	[273723] = 1113,
	[302731] = 1112,
	[302732] = 1112,
	[302733] = 1113,
	[302734] = 1113,
	[302735] = 1113,
	[302736] = 1113,
	[302737] = 1113,
	[112702] = 1112,
	[302738] = 1113,
	[106701] = 1111,
	[106721] = 1111,
	[106702] = 1112,
	[106722] = 1112,
	[106703] = 1113,
	[106723] = 1113,
	[112722] = 1112,
	[112703] = 1113,
	[112723] = 1113,
	[274701] = 1111,
	[302701] = 1111,
	[302702] = 1111,
	[302703] = 1111,
	[302704] = 1111,
	[302705] = 1111,
	[302706] = 1111,
	[302739] = 1113,
	[302740] = 1113,
	[302741] = 1113,
	[302742] = 1113,
	[302743] = 1113,
	[302744] = 1113,
	[302745] = 1113,
	[274721] = 1111,
	[302707] = 1111,
	[302746] = 1113,
	[302747] = 1113,
	[302748] = 1113,
	[104701] = 1111,
	[104721] = 1111,
	[104702] = 1112,
	[104722] = 1112,
	[109701] = 1111,
	[109721] = 1111,
	[109702] = 1112,
	[109722] = 1112,
	[109703] = 1113,
	[109723] = 1113,
	[274702] = 1112,
	[274722] = 1112,
	[274703] = 1113,
	[274723] = 1113,
	[302708] = 1111,
	[302709] = 1111,
	[302710] = 1111,
	[302711] = 1111,
	[302712] = 1111,
	[302713] = 1111,
	[104703] = 1113,
	[302714] = 1111,
	[102701] = 1111,
	[102721] = 1111,
	[102702] = 1112,
	[102722] = 1112,
	[102703] = 1113,
	[102723] = 1113,
	[104723] = 1113,
	[302715] = 1111,
	[302716] = 1111,
	[302717] = 1112,
	[302718] = 1112,
	[302719] = 1112,
	[302720] = 1112,
	[302721] = 1112,
	[302722] = 1112,
	[107701] = 1111,
	[107721] = 1111,
	[107702] = 1112,
	[107722] = 1112,
	[271701] = 1111,
	[271721] = 1111,
	[271702] = 1112,
	[302723] = 1112,
	[302724] = 1112,
	[302725] = 1112,
	[302726] = 1112,
	[302727] = 1112,
	[302728] = 1112,
	[302729] = 1112,
}

rdb.convertData = function (dst, mode, row, rowNum)
	if mode == DCT_Read and dst ~= DF_RDB then
		row.unkCatValue = "y I"
		row.unknown3 = 0
		row.unknown4 = 0

		for i = 0,15 do
			row[string.format("nv%d", i)] = 0
		end
	end

	if dst == DF_RDB then
		if mode == DCT_Read then
			row.set_part_flag = row.set_part_flag / 2
			if row.script_text == "" then
				row.script_text = "0"
			end

			if set_id_table[row.id] then
				row.set_id = set_id_table[row.id]
			else
				row.set_id = 0
			end
		else
			if row.script_text == "0" then
				row.script_text = ""
			end
		end
	end

	if mode == DCT_Read and dst == DF_SQL then
		row.model_type_dem = row.model_type_dem ~= 0
		row.model_type_def = row.model_type_def ~= 0
		row.model_type_asm = row.model_type_asm ~= 0
		row.model_type_asf = row.model_type_asf ~= 0
		row.model_type_gam = row.model_type_gam ~= 0
		row.model_type_gaf = row.model_type_gaf ~= 0
	end
end
