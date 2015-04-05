#include "IRowManipulator.h"
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


int encodeMap[32];
int decodeMap[32];

static FieldDescriptor df[] =
{	{ 1, TYPE_INT32, "id" },
	{ 1, TYPE_INT32, "monster_group" },
	{ 1, TYPE_INT32, "name_id" },
	{ 1, TYPE_INT32, "location_id" },
	{ 1, TYPE_INT32, "transform_level" },
	{ 1, TYPE_INT32, "walk_type" },
	{ 1, TYPE_INT32, "slant_type" },
	{ 1, TYPE_FLOAT32, "size" },
	{ 1, TYPE_FLOAT32, "scale" },
	{ 1, TYPE_FLOAT32, "target_fx_size" },
	{ 1, TYPE_INT32, "level" },
	{ 1, TYPE_INT32, "grp" },
	{ 1, TYPE_INT32, "affiliation" },
	{ 1, TYPE_BIT, "f_fisrt_attack" },
	{ 1, TYPE_BIT, "f_group_first_attack" },
	{ 1, TYPE_BIT, "f_response_casting" },
	{ 1, TYPE_BIT, "f_response_race" },
	{ 1, TYPE_BIT, "f_response_battle" },
	{ 1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "padding0" },
	{ 1, TYPE_INT16 | TYPE_SQLIGNORE | TYPE_CSVIGNORE | TYPE_GUIIGNORE, "padding1" },
	{ 1, TYPE_INT32, "monster_type" },
	{ 1, TYPE_INT32, "race" },
	{ 1, TYPE_INT32, "visible_range" },
	{ 1, TYPE_INT32, "chase_range" },
	{ 1, TYPE_INT32, "magic_type" },
	{ 1, TYPE_INT32, "unknown1" },
	{ 1, TYPE_INT32, "stat_id" },
	{ 1, TYPE_INT32, "fight_type" },
	{ 1, TYPE_INT32, "monster_skill_link_id" },
	{ 1, TYPE_INT32, "standard_walk_spped" },
	{ 1, TYPE_INT32, "standard_run_spped" },
	{ 1, TYPE_INT32, "walk_speed" },
	{ 1, TYPE_INT32, "run_speed" },
	{ MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "attack_range" },
	{ MAKEINDEXEDVAR(2, 1), TYPE_DECIMAL, "hidesense_range" },
	{ 1, TYPE_INT32, "hp" },
	{ 1, TYPE_INT32, "mp" },
	{ 1, TYPE_INT32, "attack_point" },
	{ 1, TYPE_INT32, "magic_point" },
	{ 1, TYPE_INT32, "defence" },
	{ 1, TYPE_INT32, "magic_defence" },
	{ 1, TYPE_INT32, "attack_speed" },
	{ 1, TYPE_INT32, "magic_speed" },
	{ 1, TYPE_INT32, "accuracy" },
	{ 1, TYPE_INT32, "avoid" },
	{ 1, TYPE_INT32, "magic_accuracy" },
	{ 1, TYPE_INT32, "magic_avoid" },
	{ 1, TYPE_INT32, "taming_id" },
	{ 1, TYPE_INT32, "creature_taming_code" },
	{ 1, TYPE_FLOAT32, "taming_percentage" },
	{ 1, TYPE_FLOAT32, "taming_exp_mod" },
	{ 1, TYPE_INT32, "exp" },
	{ 1, TYPE_INT32, "jp" },
	{ 1, TYPE_INT32, "gold_drop_percentage" },
	{ 1, TYPE_INT32, "gold_min" },
	{ 1, TYPE_INT32, "gold_max" },
	{ 1, TYPE_INT32, "chaos_drop_percentage" },
	{ 1, TYPE_INT32, "chaos_min" },
	{ 1, TYPE_INT32, "chaos_max" },
	{ 1, TYPE_INT32, "exp_2" },
	{ 1, TYPE_INT32, "jp_2" },
	{ 1, TYPE_INT32, "gold_min_2" },
	{ 1, TYPE_INT32, "gold_max_2" },
	{ 1, TYPE_INT32, "chaos_min_2" },
	{ 1, TYPE_INT32, "chaos_max_2" },
	{ 1, TYPE_INT32, "drop_table_link_id" },
	{ 1, TYPE_INT32, "texture_group" },
	{ 1, TYPE_INT32, "local_flag" },
	{ 1, TYPE_INT32, "species_id" },
	{ 256, TYPE_CHAR, "model" },
	{ 1, TYPE_INT32, "motion_file_id" },
	{ 1, TYPE_INT32, "camera_x" },
	{ 1, TYPE_INT32, "camera_y" },
	{ 1, TYPE_INT32, "camera_z" },
	{ 1, TYPE_FLOAT32, "target_x" },
	{ 1, TYPE_FLOAT32, "target_y" },
	{ 1, TYPE_FLOAT32, "target_z" },
	{ 1, TYPE_INT32, "ability" },
	{ 1, TYPE_INT32, "material" },
	{ 1, TYPE_INT32, "weapon_type" },
	{ 1, TYPE_INT32, "attack_motion_speed" },
	{ 256, TYPE_CHAR | TYPE_RDBIGNORE, "script_on_dead" }};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);

	//bit swaping map for monster id
	int i, j, oldValue;
	for(i=0; i<32; i++)
		encodeMap[i] = i;
	for(i=0, j=3; i<32; i++) {
		oldValue = encodeMap[i];
		encodeMap[i] = encodeMap[j];
		encodeMap[j] = oldValue;
		j = (j + i + 3) % 32;
	}
	for(i=0; i<32; i++)
		decodeMap[encodeMap[i]] = i;
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	int value, result, i;
	if(mode == DCT_Read && dst == DF_RDB) {
		value = row->getDataInt32("id");
		for(i=0, result=0; i<32; i++) {
			if((1<<i) & value)
				result |= 1 << decodeMap[i];
		}
		row->setDataInt32("id", result);
		int id = result;

		//Data values by Ne0@NCarbon
		if(id == 301 || id == 302 || id == 303 || id == 304 || id == 305 || id == 306 || id == 307 || id == 308 || id == 309 || id == 310 || id == 311 || id == 312 || id == 313 || id == 314 || id == 315 || id == 316 || id == 317 || id == 318 || id == 319 || id == 401 || id == 402 || id == 403 || id == 404 || id == 405 || id == 406 || id == 407 || id == 408 || id == 409 || id == 410 || id == 411 || id == 412 || id == 413 || id == 414 || id == 415 || id == 416 || id == 417 || id == 418 || id == 419 || id == 153008 || id == 321 || id == 322 || id == 323 || id == 324 || id == 325 || id == 326 || id == 327 || id == 328 || id == 329 || id == 330 || id == 331 || id == 332 || id == 333 || id == 334 || id == 335 || id == 336 || id == 337 || id == 338 || id == 339 || id == 340 || id == 421 || id == 422 || id == 423 || id == 424 || id == 425 || id == 426 || id == 427 || id == 428 || id == 429 || id == 430 || id == 431 || id == 432 || id == 433 || id == 434 || id == 435 || id == 436 || id == 437 || id == 438 || id == 439 || id == 440 || id == 341 || id == 342 || id == 343 || id == 344 || id == 345 || id == 346 || id == 347 || id == 348 || id == 349 || id == 350 || id == 351 || id == 352 || id == 353 || id == 354 || id == 355 || id == 356 || id == 357 || id == 358 || id == 359 || id == 360 || id == 441 || id == 442 || id == 443 || id == 444 || id == 381 || id == 382 || id == 383 || id == 384 || id == 385 || id == 386 || id == 387 || id == 388 || id == 389 || id == 390 || id == 391 || id == 392 || id == 393 || id == 394 || id == 395 || id == 396 || id == 397 || id == 398 || id == 399 || id == 400 || id == 361 || id == 362 || id == 363 || id == 364 || id == 365 || id == 366 || id == 367 || id == 368 || id == 369 || id == 370 || id == 371 || id == 372 || id == 373 || id == 374 || id == 375 || id == 376 || id == 377 || id == 378 || id == 379 || id == 380 || id == 153008)
			row->setDataString("script_on_dead", "mainquest2_monster_demongirl()");
		if(id == 158024)
			row->setDataString("script_on_dead", "master_158024()");
		if(id == 158025)
			row->setDataString("script_on_dead", "master_158025()");
		if(id == 159008)
			row->setDataString("script_on_dead", "master_159008()");
		if(id == 159009)
			row->setDataString("script_on_dead", "master_159009()");
		if(id == 159010)
			row->setDataString("script_on_dead", "master_159010()");
		if(id == 159011)
			row->setDataString("script_on_dead", "master_159011()");
		if(id == 159012)
			row->setDataString("script_on_dead", "master_159012()");
		if(id == 159013)
			row->setDataString("script_on_dead", "master_159013()");
		if(id == 159014)
			row->setDataString("script_on_dead", "master_159014()");
		if(id == 159015)
			row->setDataString("script_on_dead", "master_159015()");
		if(id == 159016)
			row->setDataString("script_on_dead", "master_159016()");
		if(id == 159017)
			row->setDataString("script_on_dead", "master_159017()");
		if(id == 159018)
			row->setDataString("script_on_dead", "master_159018()");
		if(id == 159019)
			row->setDataString("script_on_dead", "master_159019()");
		if(id == 159020)
			row->setDataString("script_on_dead", "master_159020()");
		if(id == 10070016)
			row->setDataString("script_on_dead", "add_field_prop(70191, 600, #@pos_x@# , #@pos_y@# , #@pos_layer@# , 16, 0, 0, 0, 1, 1, 1)");
		if(id == 10090023)
			row->setDataString("script_on_dead", "add_field_prop(120291, 600, #@pos_x@# , #@pos_y@# , #@pos_layer@# , 16, 0, 0, 0, 1, 1, 1)");
		if(id == 10125009)
			row->setDataString("script_on_dead", "add_field_prop(110191, 600, #@pos_x@# , #@pos_y@# , #@pos_layer@# , 16, 0, 0, 0, 1, 1, 1)");
		if(id == 10146009)
			row->setDataString("script_on_dead", "add_field_prop(100191, 600, #@pos_x@# , #@pos_y@# , #@pos_layer@# , 16, 0, 0, 0, 1, 1, 1)");
		if(id == 10158006)
			row->setDataString("script_on_dead", "add_field_prop(90191, 600, #@pos_x@# , #@pos_y@# , #@pos_layer@# , 16, 0, 0, 0, 1, 1, 1)");
		if(id == 10165006)
			row->setDataString("script_on_dead", "add_field_prop(80191, 600, #@pos_x@# , #@pos_y@# , #@pos_layer@# , 16, 0, 0, 0, 1, 1, 1)");
		if(id == 150020)
			row->setDataString("script_on_dead", "Vulcanus_boss_hunt_quest()");
		if(id == 1100050 || id == 1100051 || id == 1100052 || id == 1100053)
			row->setDataString("script_on_dead", "on_event_monster_dead( #@monster_handle@# )");
		if(id == 123008 || id == 123009)
			row->setDataString("script_on_dead", "mainquest2_monster_area8_8()");
		if(id == 126010 || id == 126011 || id == 129010 || id == 131010)
			row->setDataString("script_on_dead", "mainquest2_monster_area6_4()");
		if(id == 143012 || id == 143013 || id == 143014 || id == 143015 || id == 143016 || id == 143017 || id == 143018 || id == 143019 || id == 143020 || id == 143021 || id == 144015 || id == 144016 || id == 144017 || id == 144018 || id == 145011 || id == 145012 || id == 145013 || id == 145014 || id == 145015 || id == 145016 || id == 146010 || id == 146011 || id == 146012 || id == 146013 || id == 146014 || id == 147010 || id == 147011 || id == 147012 || id == 147013 || id == 147014 || id == 150013 || id == 150014 || id == 150015 || id == 150016 || id == 150017 || id == 151013 || id == 151014 || id == 151015 || id == 151016 || id == 151017 || id == 153003 || id == 153004 || id == 153005 || id == 153006 || id == 153007)
			row->setDataString("script_on_dead", "mainquest2_monster_area1_1()");
		if(id == 154004 || id == 154005 || id == 154006 || id == 154007 || id == 154008 || id == 156003 || id == 156004 || id == 156005 || id == 156006 || id == 156007 || id == 157002 || id == 157003 || id == 157004 || id == 157005 || id == 157006)
			row->setDataString("script_on_dead", "mainquest2_monster_area1_2()");
		if(id == 185002)
			row->setDataString("script_on_dead", "add_npc( 28414 , 19566, 185003,  1 , 96000)");
		if(id == 136006)
			row->setDataString("script_on_dead", "master_136006()");
		if(id == 136007)
			row->setDataString("script_on_dead", "master_136007()");
		if(id == 138008)
			row->setDataString("script_on_dead", "master_136008()");
		if(id == 136009)
			row->setDataString("script_on_dead", "master_136009()");
		if(id == 136010)
			row->setDataString("script_on_dead", "master_136010()");
		if(id == 136011)
			row->setDataString("script_on_dead", "master_136011()");
		if(id == 136012)
			row->setDataString("script_on_dead", "master_136012()");
		if(id == 136013)
			row->setDataString("script_on_dead", "master_136013()");
		if(id == 136014)
			row->setDataString("script_on_dead", "master_136014()");
		if(id == 136015)
			row->setDataString("script_on_dead", "master_136015()");
		if(id == 136016)
			row->setDataString("script_on_dead", "master_136016()");
		if(id == 136017)
			row->setDataString("script_on_dead", "master_136017()");
		if(id == 136018)
			row->setDataString("script_on_dead", "master_136018()");
		if(id == 145017)
			row->setDataString("script_on_dead", "master_145017()");
		if(id == 145018)
			row->setDataString("script_on_dead", "master_145018()");
		if(id == 145019)
			row->setDataString("script_on_dead", "master_145019()");
		if(id == 145020)
			row->setDataString("script_on_dead", "master_145020()");
		if(id == 145021)
			row->setDataString("script_on_dead", "master_145021()");
		if(id == 145022)
			row->setDataString("script_on_dead", "master_145022()");
		if(id == 145023)
			row->setDataString("script_on_dead", "master_145023()");
		if(id == 145024)
			row->setDataString("script_on_dead", "master_145024()");
		if(id == 168008)
			row->setDataString("script_on_dead", "master_168008()");
		if(id == 168009)
			row->setDataString("script_on_dead", "master_168009()");
		if(id == 177008)
			row->setDataString("script_on_dead", "master_177008()");
		if(id == 177009)
			row->setDataString("script_on_dead", "master_177009()");
		if(id == 177010)
			row->setDataString("script_on_dead", "master_177010()");
		if(id == 177011)
			row->setDataString("script_on_dead", "master_177011()");
		if(id == 177012)
			row->setDataString("script_on_dead", "master_177012()");
		if(id == 177013)
			row->setDataString("script_on_dead", "master_177013()");
		if(id == 177014)
			row->setDataString("script_on_dead", "master_177014()");
		if(id == 177015)
			row->setDataString("script_on_dead", "master_177015()");
		if(id == 177016)
			row->setDataString("script_on_dead", "master_177016()");
		if(id == 177017)
			row->setDataString("script_on_dead", "master_177017()");
		if(id == 177018)
			row->setDataString("script_on_dead", "master_177018()");
		if(id == 177019)
			row->setDataString("script_on_dead", "master_177019()");
		if(id == 177020)
			row->setDataString("script_on_dead", "master_177020()");
		if(id == 178009)
			row->setDataString("script_on_dead", "master_178009()");
		if(id == 178010)
			row->setDataString("script_on_dead", "master_178010()");
		if(id == 9400001 || id == 9400002 || id == 9400003 || id == 9400004 || id == 9400005 || id == 9400006 || id == 9400007 || id == 9400008 || id == 9400009 || id == 9400010 || id == 9400011 || id == 9400012 || id == 9400013 || id == 9400014 || id == 9400015 || id == 9400016 || id == 9400017 || id == 9400018 || id == 9400019 || id == 9400020)
			row->setDataString("script_on_dead", "Vulcanus_hunt_collect_quest()");
		if(id == 9400021 || id == 9400121 || id == 9400221 || id == 9400321 || id == 9400421 || id == 9400521 || id == 9400621 || id == 9400721 || id == 9400821 || id == 9400921 || id == 9401021 || id == 9401121 || id == 9401221 || id == 9401321 || id == 9401421)
			row->setDataString("script_on_dead", "Vulcanus_boss_hunt_quest()");
		if(id == 9400101 || id == 9400102 || id == 9400103 || id == 9400104 || id == 9400105 || id == 9400106 || id == 9400107 || id == 9400108 || id == 9400109 || id == 9400110 || id == 9400111 || id == 9400112 || id == 9400113 || id == 9400114 || id == 9400115 || id == 9400116 || id == 9400117 || id == 9400118 || id == 9400119 || id == 9400120 || id == 9400201 || id == 9400202 || id == 9400203 || id == 9400204 || id == 9400205 || id == 9400206 || id == 9400207 || id == 9400208 || id == 9400209 || id == 9400210 || id == 9400211 || id == 9400212 || id == 9400213 || id == 9400214 || id == 9400215 || id == 9400216 || id == 9400217 || id == 9400218 || id == 9400219 || id == 9400220 || id == 9400301 || id == 9400302 || id == 9400303 || id == 9400304 || id == 9400305 || id == 9400306 || id == 9400307 || id == 9400308 || id == 9400309 || id == 9400310 || id == 9400311 || id == 9400312 || id == 9400313 || id == 9400314 || id == 9400315 || id == 9400316 || id == 9400317 || id == 9400318 || id == 9400319 || id == 9400320 || id == 9400401 || id == 9400402 || id == 9400403 || id == 9400404 || id == 9400405 || id == 9400406 || id == 9400407 || id == 9400408 || id == 9400409 || id == 9400410 || id == 9400411 || id == 9400412 || id == 9400413 || id == 9400414 || id == 9400415 || id == 9400416 || id == 9400417 || id == 9400418 || id == 9400419 || id == 9400420 || id == 9400501 || id == 9400502 || id == 9400503 || id == 9400504 || id == 9400505 || id == 9400506 || id == 9400507 || id == 9400508 || id == 9400509 || id == 9400510 || id == 9400511 || id == 9400512 || id == 9400513 || id == 9400514 || id == 9400515 || id == 9400516 || id == 9400517 || id == 9400518 || id == 9400519 || id == 9400520 || id == 9400601 || id == 9400602 || id == 9400603 || id == 9400604 || id == 9400605 || id == 9400606 || id == 9400607 || id == 9400608 || id == 9400609 || id == 9400610 || id == 9400611 || id == 9400612 || id == 9400613 || id == 9400614 || id == 9400615 || id == 9400616 || id == 9400617 || id == 9400618 || id == 9400619 || id == 9400620 || id == 9400701 || id == 9400702 || id == 9400703 || id == 9400704 || id == 9400705 || id == 9400706 || id == 9400707 || id == 9400708 || id == 9400709 || id == 9400710 || id == 9400711 || id == 9400712 || id == 9400713 || id == 9400714 || id == 9400715 || id == 9400716 || id == 9400717 || id == 9400718 || id == 9400719 || id == 9400720 || id == 9400801 || id == 9400802 || id == 9400803 || id == 9400804 || id == 9400805 || id == 9400806 || id == 9400807 || id == 9400808 || id == 9400809 || id == 9400810 || id == 9400811 || id == 9400812 || id == 9400813 || id == 9400814 || id == 9400815 || id == 9400816 || id == 9400817 || id == 9400818 || id == 9400819 || id == 9400820 || id == 9400901 || id == 9400902 || id == 9400903 || id == 9400904 || id == 9400905 || id == 9400906 || id == 9400907 || id == 9400908 || id == 9400909 || id == 9400910 || id == 9400911 || id == 9400912 || id == 9400913 || id == 9400914 || id == 9400915 || id == 9400916 || id == 9400917 || id == 9400918 || id == 9400919 || id == 9400920 || id == 9401001 || id == 9401002 || id == 9401003 || id == 9401004 || id == 9401005 || id == 9401006 || id == 9401007 || id == 9401008 || id == 9401009 || id == 9401010 || id == 9401011 || id == 9401012 || id == 9401013 || id == 9401014 || id == 9401015 || id == 9401016 || id == 9401017 || id == 9401018 || id == 9401019 || id == 9401020 || id == 9401101 || id == 9401102 || id == 9401103 || id == 9401104 || id == 9401105 || id == 9401106 || id == 9401107 || id == 9401108 || id == 9401109 || id == 9401110 || id == 9401111 || id == 9401112 || id == 9401113 || id == 9401114 || id == 9401115 || id == 9401116 || id == 9401117 || id == 9401118 || id == 9401119 || id == 9401120 || id == 9401201 || id == 9401202 || id == 9401203 || id == 9401204 || id == 9401205 || id == 9401206 || id == 9401207 || id == 9401208 || id == 9401209 || id == 9401210 || id == 9401211 || id == 9401212 || id == 9401213 || id == 9401214 || id == 9401215 || id == 9401216 || id == 9401217 || id == 9401218 || id == 9401219 || id == 9401220 || id == 9401301 || id == 9401302 || id == 9401303 || id == 9401304 || id == 9401305 || id == 9401306 || id == 9401307 || id == 9401308 || id == 9401309 || id == 9401310 || id == 9401311 || id == 9401312 || id == 9401313 || id == 9401314 || id == 9401315 ||
		id == 9401316 || id == 9401317 || id == 9401318 || id == 9401319 || id == 9401320 || id == 9401401 || id == 9401402 || id == 9401403 || id == 9401404 || id == 9401405 || id == 9401406 || id == 9401407 || id == 9401408 || id == 9401409 || id == 9401410 || id == 9401411 || id == 9401412 || id == 9401413 || id == 9401414 || id == 9401415 || id == 9401416 || id == 9401417 || id == 9401418 || id == 9401419 || id == 9401420)
			row->setDataString("script_on_dead", "Vulcanus_hunt_collect_quest()");
		if(id == 110011 || id == 110012 || id == 110013)
			row->setDataString("script_on_dead", "tp_quest_creature_adventure_1_1()");
		if(id == 120004 || id == 120005 || id == 120006 || id == 120007 || id == 120008 || id == 120009 || id == 120010 || id == 120011)
			row->setDataString("script_on_dead", "tp_quest_creature_adventure_1_2()");
		if(id == 130011 || id == 130012 || id == 130013 || id == 130014 || id == 130015)
			row->setDataString("script_on_dead", "tp_quest_creature_adventure_1_3()");
		if(id == 140008 || id == 140009 || id == 140010 || id == 140011 || id == 140012)
			row->setDataString("script_on_dead", "tp_quest_creature_adventure_1_4()");
		if(id == 150021 || id == 150022 || id == 150023 || id == 150024 || id == 150025)
			row->setDataString("script_on_dead", "tp_quest_creature_adventure_1_5()");
		if(id == 160012 || id == 160013 || id == 160014)
			row->setDataString("script_on_dead", "tp_quest_creature_adventure_1_6()");
	} else if(mode == DCT_Write && dst == DF_RDB) {
		value = row->getDataInt32("id");
		for(i=0, result=0; i<32; i++) {
			if((1<<i) & value)
				result |= 1 << encodeMap[i];
		}
		row->setDataInt32("id", result);
		row->setDataInt8("padding0", 0);
		row->setDataInt16("padding1", 0);
	}
}

#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "id\0"
			"monster_group\0"
			"name_id\0"
			"location_id\0"
			"model\0"
			"motion_file_id\0"
			"transform_level\0"
			"walk_type\0"
			"slant_type\0"
			"size\0"
			"scale\0"
			"target_fx_size\0"
			"camera_x\0"
			"camera_y\0"
			"camera_z\0"
			"target_x\0"
			"target_y\0"
			"target_z\0"
			"level\0"
			"grp\0"
			"magic_type\0"
			"race\0"
			"visible_range\0"
			"chase_range\0"
			"f_fisrt_attack\0"
			"f_group_first_attack\0"
			"f_response_casting\0"
			"f_response_race\0"
			"f_response_battle\0"
			"monster_type\0"
			"stat_id\0"
			"fight_type\0"
			"monster_skill_link_id\0"
			"material\0"
			"weapon_type\0"
			"attack_motion_speed\0"
			"ability\0"
			"standard_walk_spped\0"
			"standard_run_spped\0"
			"walk_speed\0"
			"run_speed\0"
			"attack_range\0"
			"hidesense_range\0"
			"hp\0"
			"mp\0"
			"attack_point\0"
			"magic_point\0"
			"defence\0"
			"magic_defence\0"
			"attack_speed\0"
			"magic_speed\0"
			"accuracy\0"
			"avoid\0"
			"magic_accuracy\0"
			"magic_avoid\0"
			"taming_id\0"
			"creature_taming_code\0"
			"taming_percentage\0"
			"taming_exp_mod\0"
			"exp\0"
			"jp\0"
			"gold_drop_percentage\0"
			"gold_min\0"
			"gold_max\0"
			"chaos_drop_percentage\0"
			"chaos_min\0"
			"chaos_max\0"
			"exp_2\0"
			"jp_2\0"
			"gold_min_2\0"
			"gold_max_2\0"
			"chaos_min_2\0"
			"chaos_max_2\0"
			"drop_table_link_id\0"
			"texture_group\0"
			"local_flag\0"
			"script_on_dead\0";
}

#pragma comment(linker, "/EXPORT:getCSVColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getCSVColumnOrder() {
	return getSQLColumnOrder();
}

#ifdef __cplusplus
}
#endif
