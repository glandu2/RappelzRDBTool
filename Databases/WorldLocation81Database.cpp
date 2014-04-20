#include "IRowManipulator.h"
#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {1, TYPE_INT32, "text_id"},
	 {1, TYPE_INT32, "x"},
	 {1, TYPE_INT32, "y"},
	 {1, TYPE_INT32, "preloading_distance"},
	 {1, TYPE_INT32, "apply_location_name"},
	 {1, TYPE_INT32, "apply_light"},
	 {1, TYPE_INT32, "apply_bgm"},
	 {1, TYPE_INT32, "location_type"},
	 {1, TYPE_INT32, "fog_application"},
	 {1, TYPE_INT32, "time_id"},
	 {1, TYPE_INT32, "weather_id"},
	 {1, TYPE_INT32, "cloud_ratio"},
	 {1, TYPE_INT32, "weather_change_time"},
	 {1, TYPE_INT32, "weather_ratio"},
	 {1, TYPE_INT32, "sky_start_a"},
	 {1, TYPE_INT32, "sky_start_r"},
	 {1, TYPE_INT32, "sky_start_g"},
	 {1, TYPE_INT32, "sky_start_b"},
	 {1, TYPE_INT32, "sky_mid_a"},
	 {1, TYPE_INT32, "sky_mid_r"},
	 {1, TYPE_INT32, "sky_mid_g"},
	 {1, TYPE_INT32, "sky_mid_b"},
	 {1, TYPE_INT32, "sky_end_a"},
	 {1, TYPE_INT32, "sky_end_r"},
	 {1, TYPE_INT32, "sky_end_g"},
	 {1, TYPE_INT32, "sky_end_b"},
	 {1, TYPE_FLOAT32, "sky_mid_rate"},
	 {1, TYPE_INT32, "diffuse_a"},
	 {1, TYPE_INT32, "diffuse_r"},
	 {1, TYPE_INT32, "diffuse_g"},
	 {1, TYPE_INT32, "diffuse_b"},
	 {1, TYPE_INT32, "ambient_a"},
	 {1, TYPE_INT32, "ambient_r"},
	 {1, TYPE_INT32, "ambient_g"},
	 {1, TYPE_INT32, "ambient_b"},
	 {1, TYPE_INT32, "specular_a"},
	 {1, TYPE_INT32, "specular_r"},
	 {1, TYPE_INT32, "specular_g"},
	 {1, TYPE_INT32, "specular_b"},
	 {1, TYPE_INT32, "fog_a"},
	 {1, TYPE_INT32, "fog_r"},
	 {1, TYPE_INT32, "fog_g"},
	 {1, TYPE_INT32, "fog_b"},
	 {1, TYPE_FLOAT32, "v_start"},
	 {1, TYPE_FLOAT32, "v_end"},
	 {1, TYPE_FLOAT32, "h_start"},
	 {1, TYPE_FLOAT32, "h_end"},
	 {1, TYPE_INT16, "fixed_terrain_distance"},
	 {1, TYPE_INT32, "cloud_a"},
	 {1, TYPE_INT32, "cloud_r"},
	 {1, TYPE_INT32, "cloud_g"},
	 {1, TYPE_INT32, "cloud_b"},
	 {1, TYPE_FLOAT32, "land_specular"},
	 {1, TYPE_FLOAT32, "character_diffuse"},
	 {1, TYPE_FLOAT32, "character_ambient"},
	 {256, TYPE_CHAR, "sun_texture_file"},
	 {256, TYPE_CHAR, "moon_texture_file"},
	 {256, TYPE_CHAR, "cloud_texture_file"},
	 {1, TYPE_INT32, "bgm1_ID"},
	 {1, TYPE_INT32, "bgm2_ID"},
	 {1, TYPE_INT32, "bgm3_ID"},
	 {1, TYPE_INT32, "bgm1_ratio"},
	 {1, TYPE_INT32, "bgm2_ratio"},
	 {1, TYPE_INT32, "bgm3_ratio"},
	 {1, TYPE_INT32, "environmental_sound1_ID"},
	 {1, TYPE_INT32, "environmental_sound2_ID"},
	 {1, TYPE_INT32, "environmental_sound3_ID"},
	 {1, TYPE_INT32, "environmental_sound4_ID"},
	 {1, TYPE_INT32, "environmental_sound1_ratio"},
	 {1, TYPE_INT32, "environmental_sound2_ratio"},
	 {1, TYPE_INT32, "environmental_sound3_ratio"},
	 {1, TYPE_INT32, "environmental_sound4_ratio"},
	 {1, TYPE_INT32, "environmental_sound_reverb"},
	 {1, TYPE_INT32, "environmental_sound_volume"},
	 {1, TYPE_INT32, "environmental_sound_equalizer"},
	 {1, TYPE_INT16, "minimap_ani_file_ID"},
	 {1, TYPE_INT16, "worldmap_ani_file_ID"},
	 {1, TYPE_INT8 | TYPE_SQLIGNORE | TYPE_CSVIGNORE, "unknownvalue0"},
	 {1, TYPE_INT32, "zoom_map_type"},
	 {1, TYPE_INT32, "display_positoin_x"},
	 {1, TYPE_INT32, "display_positoin_y"},
	 {1, TYPE_INT32, "zoom_x"},
	 {1, TYPE_INT32, "offset_x"},
	 {1, TYPE_INT32, "offset_y"},
	 {1, TYPE_INT32, "size_x"},
	 {1, TYPE_INT32, "size_y"},
	 {1, TYPE_INT32, "blank_resource_ID"},
	 {1, TYPE_INT32, "blank_a"},
	 {1, TYPE_INT32, "blank_r"},
	 {1, TYPE_INT32, "blank_g"},
	 {1, TYPE_INT32, "blank_b"},
	 {1, TYPE_INT32, "zoom_minimap"},
	 {1, TYPE_INT32, "precipitation_min"},
	 {1, TYPE_INT32, "precipitation_max"},
	 {1, TYPE_FLOAT32, "aurora_t"},
	 {1, TYPE_FLOAT32, "aurora_a"},
	 {1, TYPE_FLOAT32, "aurora_s"},
	 {1, TYPE_FLOAT32, "aurora_h"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#pragma comment(linker, "/EXPORT:getSQLColumnOrder=_getSQLColumnOrder@0")
EDATABASEDLL const char* DLLCALLCONV getSQLColumnOrder() {
	return  "id\0"
			"text_id\0"
			"x\0"
			"y\0"
			"preloading_distance\0"
			"apply_location_name\0"
			"apply_light\0"
			"apply_bgm\0"
			"location_type\0"
			"fog_application\0"
			"time_id\0"
			"weather_id\0"
			"cloud_ratio\0"
			"weather_change_time\0"
			"weather_ratio\0"
			"sky_start_a\0"
			"sky_start_r\0"
			"sky_start_g\0"
			"sky_start_b\0"
			"sky_mid_a\0"
			"sky_mid_r\0"
			"sky_mid_g\0"
			"sky_mid_b\0"
			"sky_end_a\0"
			"sky_end_r\0"
			"sky_end_g\0"
			"sky_end_b\0"
			"sky_mid_rate\0"
			"diffuse_a\0"
			"diffuse_r\0"
			"diffuse_g\0"
			"diffuse_b\0"
			"ambient_a\0"
			"ambient_r\0"
			"ambient_g\0"
			"ambient_b\0"
			"specular_a\0"
			"specular_r\0"
			"specular_g\0"
			"specular_b\0"
			"fog_a\0"
			"fog_r\0"
			"fog_g\0"
			"fog_b\0"
			"v_start\0"
			"v_end\0"
			"h_start\0"
			"h_end\0"
			"fixed_terrain_distance\0"
			"cloud_a\0"
			"cloud_r\0"
			"cloud_g\0"
			"cloud_b\0"
			"land_specular\0"
			"character_diffuse\0"
			"character_ambient\0"
			"sun_texture_file\0"
			"moon_texture_file\0"
			"cloud_texture_file\0"
			"bgm1_id\0"
			"bgm1_ratio\0"
			"bgm2_id\0"
			"bgm2_ratio\0"
			"bgm3_id\0"
			"bgm3_ratio\0"
			"environmental_sound1_id\0"
			"environmental_sound1_ratio\0"
			"environmental_sound2_id\0"
			"environmental_sound2_ratio\0"
			"environmental_sound3_id\0"
			"environmental_sound3_ratio\0"
			"environmental_sound4_id\0"
			"environmental_sound4_ratio\0"
			"environmental_sound_reverb\0"
			"environmental_sound_volume\0"
			"environmental_sound_equalizer\0"
			"minimap_ani_file_id\0"
			"worldmap_ani_file_id\0"
			"zoom_map_type\0"
			"display_positoin_x\0"
			"display_positoin_y\0"
			"zoom_x\0"
			"offset_x\0"
			"offset_y\0"
			"size_x\0"
			"size_y\0"
			"blank_resource_id\0"
			"blank_a\0"
			"blank_r\0"
			"blank_g\0"
			"blank_b\0"
			"zoom_minimap\0"
			"precipitation_min\0"
			"precipitation_max\0"
			"aurora_t\0"
			"aurora_a\0"
			"aurora_s\0"
			"aurora_h\0";
}

#pragma comment(linker, "/EXPORT:convertData=_convertData@16")
void EDATABASEDLL DLLCALLCONV convertData(eDataFormat dst, eDataConvertionType mode, IRowManipulator *row, unsigned int rowNum) {
	if(mode == DCT_Read && dst != DF_RDB) {
		*static_cast<char*>(row->getValuePtr("unknownvalue0")) = 0;
	}
}

#pragma comment(linker, "/EXPORT:getDefaultTableName=_getDefaultTableName@0")
EDATABASEDLL const char*  DLLCALLCONV getDefaultTableName() {
	return "WorldLocation";
}
#ifdef __cplusplus
}
#endif

