rdb.fields = {
	{"job_id", TYPE_INT32 | TYPE_FLAG_SORT},
	{"skill_id", TYPE_INT32},
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
}

rdb.specialCaseId = SPECIALCASE_DOUBLEFORRDB

