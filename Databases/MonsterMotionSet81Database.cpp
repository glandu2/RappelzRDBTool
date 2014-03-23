#include "DataType.h"
#include "ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

//refer to CharacterMotion

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "character_id"},
	 {1, TYPE_INT32, "def"},
	 {1, TYPE_INT32, "battle"},
	 {1, TYPE_INT32, "idle"},
	 {1, TYPE_INT32, "walk"},
	 {1, TYPE_INT32, "run"},
	 {1, TYPE_INT32, "be_attack"},
	 {1, TYPE_INT32, "be_attack2"},
	 {1, TYPE_INT32, "dead"},
	 {1, TYPE_INT32, "flying_dead"},
	 {1, TYPE_INT32, "take"},
	 {1, TYPE_INT32, "attack_1"},
	 {1, TYPE_INT32, "attack_2"},
	 {1, TYPE_INT32, "attack_3"},
	 {1, TYPE_INT32, "draw_bow"},
	 {1, TYPE_INT32, "target_bow"},
	 {1, TYPE_INT32, "fire_bow"},
	 {1, TYPE_INT32, "casting_1"},
	 {1, TYPE_INT32, "casting_2"},
	 {1, TYPE_INT32, "casting_3"},
	 {1, TYPE_INT32, "melee_1"},
	 {1, TYPE_INT32, "melee_2"},
	 {1, TYPE_INT32, "melee_3"},
	 {1, TYPE_INT32, "mount"},
	 {1, TYPE_INT32, "unmount"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

