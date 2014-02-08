#include "DataType.h"
#include "ExportDLL.h"
#include "IRowManipulator.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "text_id"},
	 {1, TYPE_INT32 | TYPE_RDBIGNORE, "desc_id"},
	 {1, TYPE_INT8, "is_valid"},
	 {1, TYPE_INT8, "elemental"},
	 {1, TYPE_INT8, "is_passive"},
	 {1, TYPE_INT8, "is_physical_act"},
	 {1, TYPE_INT8, "is_harmful"},
	 {1, TYPE_INT8, "is_need_target"},
	 {1, TYPE_INT8, "is_corpse"},
	 {1, TYPE_INT8, "is_toggle"},
	 {1, TYPE_INT8, "casting_type"},
	 {1, TYPE_INT8, "casting_level"},
	 {1, TYPE_INT32, "toggle_group"},
	 {1, TYPE_INT32, "cast_range"},
	 {1, TYPE_INT32, "valid_range"},
	 {1, TYPE_INT32, "cost_hp"},
	 {1, TYPE_INT32, "cost_hp_per_skl"},
	 {1, TYPE_INT32, "cost_mp"},
	 {1, TYPE_INT32, "cost_mp_per_skl"},
	 {1, TYPE_INT32, "cost_mp_per_enhance"},
	 {1, TYPE_FLOAT32, "cost_hp_per"},
	 {1, TYPE_FLOAT32, "cost_hp_per_skl_per"},
	 {1, TYPE_FLOAT32, "cost_mp_per"},
	 {1, TYPE_FLOAT32, "cost_mp_per_skl_per"},
	 {1, TYPE_INT32, "cost_havoc"},
	 {1, TYPE_INT32, "cost_havoc_per_skl"},
	 {1, TYPE_INT32, "cost_energy"},
	 {1, TYPE_INT32, "cost_energy_per_skl"},
	 {1, TYPE_INT32, "cost_exp"},
	 {1, TYPE_INT32, "cost_exp_per_enhance"},
	 {1, TYPE_INT32, "cost_jp"},
	 {1, TYPE_INT32, "cost_jp_per_enhance"},
	 {1, TYPE_INT32, "cost_item"},
	 {1, TYPE_INT32, "cost_item_count"},
	 {1, TYPE_INT32, "cost_item_count_per_skl"},
	 {1, TYPE_INT32, "need_level"},
	 {1, TYPE_INT32, "need_hp"},
	 {1, TYPE_INT32, "need_mp"},
	 {1, TYPE_INT32, "need_havoc"},
	 {1, TYPE_INT32, "need_havoc_burst"},
	 {1, TYPE_INT32, "need_state_id"},
	 {1, TYPE_INT8, "need_state_level"},
	 {1, TYPE_INT8, "need_state_exhaust"},
	 {1, TYPE_INT8, "vf_one_hand_sword"},
	 {1, TYPE_INT8, "vf_two_hand_sword"},
	 {1, TYPE_INT8, "vf_double_sword"},
	 {1, TYPE_INT8, "vf_dagger"},
	 {1, TYPE_INT8, "vf_double_dagger"},
	 {1, TYPE_INT8, "vf_spear"},
	 {1, TYPE_INT8, "vf_axe"},
	 {1, TYPE_INT8, "vf_one_hand_axe"},
	 {1, TYPE_INT8, "vf_double_axe"},
	 {1, TYPE_INT8, "vf_one_hand_mace"},
	 {1, TYPE_INT8, "vf_two_hand_mace"},
	 {1, TYPE_INT8, "vf_lightbow"},
	 {1, TYPE_INT8, "vf_heavybow"},
	 {1, TYPE_INT8, "vf_crossbow"},
	 {1, TYPE_INT8, "vf_one_hand_staff"},
	 {1, TYPE_INT8, "vf_two_hand_staff"},
	 {1, TYPE_INT8, "vf_shield_only"},
	 {1, TYPE_INT8, "vf_is_not_need_weapon"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "delay_cast"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "delay_cast_per_skl"},
	 {1, TYPE_FLOAT32, "delay_cast_mode_per_enhance"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "delay_common"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "delay_cooltime"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "delay_cooltime_per_skl"},
	 {1, TYPE_FLOAT32, "delay_cooltime_mode_per_enhance"},
	 {1, TYPE_INT32, "cool_time_group_id"},
	 {1, TYPE_INT8, "uf_self"},
	 {1, TYPE_INT8, "uf_party"},
	 {1, TYPE_INT8, "uf_guild"},
	 {1, TYPE_INT8, "uf_neutral"},
	 {1, TYPE_INT8, "uf_purple"},
	 {1, TYPE_INT8, "uf_enemy"},
	 {1, TYPE_INT8, "tf_avatar"},
	 {1, TYPE_INT8, "tf_summon"},
	 {1, TYPE_INT8, "tf_monster"},
	 {1, TYPE_INT32, "skill_lvup_limit"},
	 {1, TYPE_INT16, "target"},
	 {1, TYPE_INT16, "effect_type"},
	 {1, TYPE_INT32, "skill_enchant_link_id"},
	 {1, TYPE_INT32, "state_id"},
	 {1, TYPE_INT32, "state_level_base"},
	 {1, TYPE_FLOAT32, "state_level_per_skl"},
	 {1, TYPE_FLOAT32, "state_level_per_enhance"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "state_second"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "state_second_per_level"},
	 {MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "state_second_per_enhance"},
	 {1, TYPE_INT32, "probability_on_hit"},
	 {1, TYPE_INT32, "probability_inc_by_slv"},
	 {1, TYPE_INT16, "hit_bonus"},
	 {1, TYPE_INT16, "hit_bonus_per_enhance"},
	 {1, TYPE_INT16, "percentage"},
	 {1, TYPE_FLOAT32, "hate_mod"},
	 {1, TYPE_INT32, "hate_basic"},
	 {1, TYPE_FLOAT32, "hate_per_skl"},
	 {1, TYPE_FLOAT32, "hate_per_enhance"},
	 {1, TYPE_INT32, "critical_bonus"},
	 {1, TYPE_INT32, "critical_bonus_per_skl"},
	 {1, TYPE_FLOAT32, "var1"},
	 {1, TYPE_FLOAT32, "var2"},
	 {1, TYPE_FLOAT32, "var3"},
	 {1, TYPE_FLOAT32, "var4"},
	 {1, TYPE_FLOAT32, "var5"},
	 {1, TYPE_FLOAT32, "var6"},
	 {1, TYPE_FLOAT32, "var7"},
	 {1, TYPE_FLOAT32, "var8"},
	 {1, TYPE_FLOAT32, "var9"},
	 {1, TYPE_FLOAT32, "var10"},
	 {1, TYPE_FLOAT32, "var11"},
	 {1, TYPE_FLOAT32, "var12"},
	 {1, TYPE_FLOAT32, "var13"},
	 {1, TYPE_FLOAT32, "var14"},
	 {1, TYPE_FLOAT32, "var15"},
	 {1, TYPE_FLOAT32, "var16"},
	 {1, TYPE_FLOAT32, "var17"},
	 {1, TYPE_FLOAT32, "var18"},
	 {1, TYPE_FLOAT32, "var19"},
	 {1, TYPE_FLOAT32, "var20"},
	 {1, TYPE_INT32, "jp_cost_lv1"},
	 {1, TYPE_INT32, "jp_cost_lv2"},
	 {1, TYPE_INT32, "jp_cost_lv3"},
	 {1, TYPE_INT32, "jp_cost_lv4"},
	 {1, TYPE_INT32, "jp_cost_lv5"},
	 {1, TYPE_INT32, "jp_cost_lv6"},
	 {1, TYPE_INT32, "jp_cost_lv7"},
	 {1, TYPE_INT32, "jp_cost_lv8"},
	 {1, TYPE_INT32, "jp_cost_lv9"},
	 {1, TYPE_INT32, "jp_cost_lv10"},
	 {1, TYPE_INT32, "jp_cost_lv11"},
	 {1, TYPE_INT32, "jp_cost_lv12"},
	 {1, TYPE_INT32, "jp_cost_lv13"},
	 {1, TYPE_INT32, "jp_cost_lv14"},
	 {1, TYPE_INT32, "jp_cost_lv15"},
	 {1, TYPE_INT32, "jp_cost_lv16"},
	 {1, TYPE_INT32, "jp_cost_lv17"},
	 {1, TYPE_INT32, "jp_cost_lv18"},
	 {1, TYPE_INT32, "jp_cost_lv19"},
	 {1, TYPE_INT32, "jp_cost_lv20"},
	 {1, TYPE_INT32, "jp_cost_lv21"},
	 {1, TYPE_INT32, "jp_cost_lv22"},
	 {1, TYPE_INT32, "jp_cost_lv23"},
	 {1, TYPE_INT32, "jp_cost_lv24"},
	 {1, TYPE_INT32, "jp_cost_lv25"},
	 {1, TYPE_INT32, "jp_cost_lv26"},
	 {1, TYPE_INT32, "jp_cost_lv27"},
	 {1, TYPE_INT32, "jp_cost_lv28"},
	 {1, TYPE_INT32, "jp_cost_lv29"},
	 {1, TYPE_INT32, "jp_cost_lv30"},
	 {1, TYPE_INT32, "jp_cost_lv31"},
	 {1, TYPE_INT32, "jp_cost_lv32"},
	 {1, TYPE_INT32, "jp_cost_lv33"},
	 {1, TYPE_INT32, "jp_cost_lv34"},
	 {1, TYPE_INT32, "jp_cost_lv35"},
	 {1, TYPE_INT32, "jp_cost_lv36"},
	 {1, TYPE_INT32, "jp_cost_lv37"},
	 {1, TYPE_INT32, "jp_cost_lv38"},
	 {1, TYPE_INT32, "jp_cost_lv39"},
	 {1, TYPE_INT32, "jp_cost_lv40"},
	 {1, TYPE_INT32, "jp_cost_lv41"},
	 {1, TYPE_INT32, "jp_cost_lv42"},
	 {1, TYPE_INT32, "jp_cost_lv43"},
	 {1, TYPE_INT32, "jp_cost_lv44"},
	 {1, TYPE_INT32, "jp_cost_lv45"},
	 {1, TYPE_INT32, "jp_cost_lv46"},
	 {1, TYPE_INT32, "jp_cost_lv47"},
	 {1, TYPE_INT32, "jp_cost_lv48"},
	 {1, TYPE_INT32, "jp_cost_lv49"},
	 {1, TYPE_INT32, "jp_cost_lv50"},
	 {1, TYPE_INT8, "hasJPCost"},
	 {1, TYPE_INT32, "tooltip_id"},
	 {1, TYPE_INT32, "icon_id"},
	 {256, TYPE_CHAR, "icon_file_name"},
	 {1, TYPE_INT8, "is_projectile"},
	 {1, TYPE_FLOAT32, "projectile_speed"},
	 {1, TYPE_FLOAT32, "projectile_acceleration"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "id\0"
			"text_id\0"
			"desc_id\0"
			"tooltip_id\0"
			"is_valid\0"
			"elemental\0"
			"is_passive\0"
			"is_physical_act\0"
			"is_harmful\0"
			"is_need_target\0"
			"is_corpse\0"
			"is_toggle\0"
			"toggle_group\0"
			"casting_type\0"
			"casting_level\0"
			"cast_range\0"
			"valid_range\0"
			"cost_hp\0"
			"cost_hp_per_skl\0"
			"cost_mp\0"
			"cost_mp_per_skl\0"
			"cost_mp_per_enhance\0"
			"cost_hp_per\0"
			"cost_hp_per_skl_per\0"
			"cost_mp_per\0"
			"cost_mp_per_skl_per\0"
			"cost_havoc\0"
			"cost_havoc_per_skl\0"
			"cost_energy\0"
			"cost_energy_per_skl\0"
			"cost_exp\0"
			"cost_exp_per_enhance\0"
			"cost_jp\0"
			"cost_jp_per_enhance\0"
			"cost_item\0"
			"cost_item_count\0"
			"cost_item_count_per_skl\0"
			"need_level\0"
			"need_hp\0"
			"need_mp\0"
			"need_havoc\0"
			"need_havoc_burst\0"
			"need_state_id\0"
			"need_state_level\0"
			"need_state_exhaust\0"
			"vf_one_hand_sword\0"
			"vf_two_hand_sword\0"
			"vf_double_sword\0"
			"vf_dagger\0"
			"vf_double_dagger\0"
			"vf_spear\0"
			"vf_axe\0"
			"vf_one_hand_axe\0"
			"vf_double_axe\0"
			"vf_one_hand_mace\0"
			"vf_two_hand_mace\0"
			"vf_lightbow\0"
			"vf_heavybow\0"
			"vf_crossbow\0"
			"vf_one_hand_staff\0"
			"vf_two_hand_staff\0"
			"vf_shield_only\0"
			"vf_is_not_need_weapon\0"
			"delay_cast\0"
			"delay_cast_per_skl\0"
			"delay_cast_mode_per_enhance\0"
			"delay_common\0"
			"delay_cooltime\0"
			"delay_cooltime_per_skl\0"
			"delay_cooltime_mode_per_enhance\0"
			"cool_time_group_id\0"
			"uf_self\0"
			"uf_party\0"
			"uf_guild\0"
			"uf_neutral\0"
			"uf_purple\0"
			"uf_enemy\0"
			"tf_avatar\0"
			"tf_summon\0"
			"tf_monster\0"
			"skill_lvup_limit\0"
			"target\0"
			"effect_type\0"
			"skill_enchant_link_id\0"
			"state_id\0"
			"state_level_base\0"
			"state_level_per_skl\0"
			"state_level_per_enhance\0"
			"state_second\0"
			"state_second_per_level\0"
			"state_second_per_enhance\0"
			"probability_on_hit\0"
			"probability_inc_by_slv\0"
			"hit_bonus\0"
			"hit_bonus_per_enhance\0"
			"percentage\0"
			"hate_mod\0"
			"hate_basic\0"
			"hate_per_skl\0"
			"hate_per_enhance\0"
			"critical_bonus\0"
			"critical_bonus_per_skl\0"
			"var1\0"
			"var2\0"
			"var3\0"
			"var4\0"
			"var5\0"
			"var6\0"
			"var7\0"
			"var8\0"
			"var9\0"
			"var10\0"
			"var11\0"
			"var12\0"
			"var13\0"
			"var14\0"
			"var15\0"
			"var16\0"
			"var17\0"
			"var18\0"
			"var19\0"
			"var20\0"
			"jp_cost_lv1\0"
			"jp_cost_lv2\0"
			"jp_cost_lv3\0"
			"jp_cost_lv4\0"
			"jp_cost_lv5\0"
			"jp_cost_lv6\0"
			"jp_cost_lv7\0"
			"jp_cost_lv8\0"
			"jp_cost_lv9\0"
			"jp_cost_lv10\0"
			"jp_cost_lv11\0"
			"jp_cost_lv12\0"
			"jp_cost_lv13\0"
			"jp_cost_lv14\0"
			"jp_cost_lv15\0"
			"jp_cost_lv16\0"
			"jp_cost_lv17\0"
			"jp_cost_lv18\0"
			"jp_cost_lv19\0"
			"jp_cost_lv20\0"
			"jp_cost_lv21\0"
			"jp_cost_lv22\0"
			"jp_cost_lv23\0"
			"jp_cost_lv24\0"
			"jp_cost_lv25\0"
			"jp_cost_lv26\0"
			"jp_cost_lv27\0"
			"jp_cost_lv28\0"
			"jp_cost_lv29\0"
			"jp_cost_lv30\0"
			"jp_cost_lv31\0"
			"jp_cost_lv32\0"
			"jp_cost_lv33\0"
			"jp_cost_lv34\0"
			"jp_cost_lv35\0"
			"jp_cost_lv36\0"
			"jp_cost_lv37\0"
			"jp_cost_lv38\0"
			"jp_cost_lv39\0"
			"jp_cost_lv40\0"
			"jp_cost_lv41\0"
			"jp_cost_lv42\0"
			"jp_cost_lv43\0"
			"jp_cost_lv44\0"
			"jp_cost_lv45\0"
			"jp_cost_lv46\0"
			"jp_cost_lv47\0"
			"jp_cost_lv48\0"
			"jp_cost_lv49\0"
			"jp_cost_lv50\0"
			"unknownValue\0"
			"icon_id\0"
			"icon_file_name\0"
			"is_projectile\0"
			"projectile_speed\0"
			"projectile_acceleration\0";
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst == DF_RDB) {
		*static_cast<int*>(row->getValuePtr("desc_id")) = 0;
	}
}

#ifdef __cplusplus
}
#endif

