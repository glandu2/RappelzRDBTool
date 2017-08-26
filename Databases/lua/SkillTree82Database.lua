rdb.fields = {
	{"job_id", TYPE_INT32 | TYPE_FLAG_SORT},
	{"skill_id", TYPE_INT32},
	{"skill_group_id", TYPE_INT32},
	{"min_skill_lv", TYPE_INT32},
	{"max_skill_lv", TYPE_INT32},
	{"lv", TYPE_INT32},
	{"job_lv", TYPE_INT32},
	{"jp_ratio", TYPE_FLOAT32},
	{"need_skill_id_1", TYPE_INT32},
	{"need_skill_id_2", TYPE_INT32},
	{"need_skill_id_3", TYPE_INT32},
	{"need_skill_lv_1", TYPE_INT32},
	{"need_skill_lv_2", TYPE_INT32},
	{"need_skill_lv_3", TYPE_INT32},
	{"cenhance_min", TYPE_INT32},
	{"cenhance_max", TYPE_INT32},
}

rdb.sqlColumnOrder = table.concat({
	"job_id\0",
	"skill_id\0",
	"skill_group_id\0",
	"min_skill_lv\0",
	"max_skill_lv\0",
	"lv\0",
	"job_lv\0",
	"jp_ratio\0",
	"need_skill_id_1\0",
	"need_skill_lv_1\0",
	"need_skill_id_2\0",
	"need_skill_lv_2\0",
	"need_skill_id_3\0",
	"need_skill_lv_3\0",
	"cenhance_min\0",
	"cenhance_max\0",
})

rdb.specialCaseId = SPECIALCASE_DOUBLEFORRDB

