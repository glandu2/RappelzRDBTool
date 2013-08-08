#include "../Base/DataType.h"
#include "../Base/ExportDLL.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

//refer to CharacterMotion

static FieldDescriptor df[] =
	{{1, TYPE_INT32, "id"},
	 {64, TYPE_CHAR, "string_id"},
	 {1, TYPE_INT32, "deva_m_unarmored"},
	 {1, TYPE_INT32, "deva_m_one_hand_sword"},
	 {1, TYPE_INT32, "deva_m_two_hand_sword"},
	 {1, TYPE_INT32, "deva_m_double_sword"},
	 {1, TYPE_INT32, "deva_m_one_hand_dagger"},
	 {1, TYPE_INT32, "deva_m_double_dagger"},
	 {1, TYPE_INT32, "deva_m_two_hand_spear"},
	 {1, TYPE_INT32, "deva_m_one_hand_axe"},
	 {1, TYPE_INT32, "deva_m_two_hand_axe"},
	 {1, TYPE_INT32, "deva_m_double_axe"},
	 {1, TYPE_INT32, "deva_m_one_hand_staff"},
	 {1, TYPE_INT32, "deva_m_two_hand_staff"},
	 {1, TYPE_INT32, "deva_m_one_hand_mace"},
	 {1, TYPE_INT32, "deva_m_two_hand_mace"},
	 {1, TYPE_INT32, "deva_m_bow"},
	 {1, TYPE_INT32, "deva_m_cross_bow"},
	 {1, TYPE_INT32, "deva_f_unarmored"},
	 {1, TYPE_INT32, "deva_f_one_hand_sword"},
	 {1, TYPE_INT32, "deva_f_two_hand_sword"},
	 {1, TYPE_INT32, "deva_f_double_sword"},
	 {1, TYPE_INT32, "deva_f_one_hand_dagger"},
	 {1, TYPE_INT32, "deva_f_double_dagger"},
	 {1, TYPE_INT32, "deva_f_two_hand_spear"},
	 {1, TYPE_INT32, "deva_f_one_hand_axe"},
	 {1, TYPE_INT32, "deva_f_two_hand_axe"},
	 {1, TYPE_INT32, "deva_f_double_axe"},
	 {1, TYPE_INT32, "deva_f_one_hand_staff"},
	 {1, TYPE_INT32, "deva_f_two_hand_staff"},
	 {1, TYPE_INT32, "deva_f_one_hand_mace"},
	 {1, TYPE_INT32, "deva_f_two_hand_mace"},
	 {1, TYPE_INT32, "deva_f_bow"},
	 {1, TYPE_INT32, "deva_f_cross_bow"},
	 {1, TYPE_INT32, "asura_m_unarmored"},
	 {1, TYPE_INT32, "asura_m_one_hand_sword"},
	 {1, TYPE_INT32, "asura_m_two_hand_sword"},
	 {1, TYPE_INT32, "asura_m_double_sword"},
	 {1, TYPE_INT32, "asura_m_one_hand_dagger"},
	 {1, TYPE_INT32, "asura_m_double_dagger"},
	 {1, TYPE_INT32, "asura_m_two_hand_spear"},
	 {1, TYPE_INT32, "asura_m_one_hand_axe"},
	 {1, TYPE_INT32, "asura_m_two_hand_axe"},
	 {1, TYPE_INT32, "asura_m_double_axe"},
	 {1, TYPE_INT32, "asura_m_one_hand_staff"},
	 {1, TYPE_INT32, "asura_m_two_hand_staff"},
	 {1, TYPE_INT32, "asura_m_one_hand_mace"},
	 {1, TYPE_INT32, "asura_m_two_hand_mace"},
	 {1, TYPE_INT32, "asura_m_bow"},
	 {1, TYPE_INT32, "asura_m_cross_bow"},
	 {1, TYPE_INT32, "asura_f_unarmored"},
	 {1, TYPE_INT32, "asura_f_one_hand_sword"},
	 {1, TYPE_INT32, "asura_f_two_hand_sword"},
	 {1, TYPE_INT32, "asura_f_double_sword"},
	 {1, TYPE_INT32, "asura_f_one_hand_dagger"},
	 {1, TYPE_INT32, "asura_f_double_dagger"},
	 {1, TYPE_INT32, "asura_f_two_hand_spear"},
	 {1, TYPE_INT32, "asura_f_one_hand_axe"},
	 {1, TYPE_INT32, "asura_f_two_hand_axe"},
	 {1, TYPE_INT32, "asura_f_double_axe"},
	 {1, TYPE_INT32, "asura_f_one_hand_staff"},
	 {1, TYPE_INT32, "asura_f_two_hand_staff"},
	 {1, TYPE_INT32, "asura_f_one_hand_mace"},
	 {1, TYPE_INT32, "asura_f_two_hand_mace"},
	 {1, TYPE_INT32, "asura_f_bow"},
	 {1, TYPE_INT32, "asura_f_cross_bow"},
	 {1, TYPE_INT32, "gaia_m_unarmored"},
	 {1, TYPE_INT32, "gaia_m_one_hand_sword"},
	 {1, TYPE_INT32, "gaia_m_two_hand_sword"},
	 {1, TYPE_INT32, "gaia_m_double_sword"},
	 {1, TYPE_INT32, "gaia_m_one_hand_dagger"},
	 {1, TYPE_INT32, "gaia_m_double_dagger"},
	 {1, TYPE_INT32, "gaia_m_two_hand_spear"},
	 {1, TYPE_INT32, "gaia_m_one_hand_axe"},
	 {1, TYPE_INT32, "gaia_m_two_hand_axe"},
	 {1, TYPE_INT32, "gaia_m_double_axe"},
	 {1, TYPE_INT32, "gaia_m_one_hand_staff"},
	 {1, TYPE_INT32, "gaia_m_two_hand_staff"},
	 {1, TYPE_INT32, "gaia_m_one_hand_mace"},
	 {1, TYPE_INT32, "gaia_m_two_hand_mace"},
	 {1, TYPE_INT32, "gaia_m_bow"},
	 {1, TYPE_INT32, "gaia_m_cross_bow"},
	 {1, TYPE_INT32, "gaia_f_unarmored"},
	 {1, TYPE_INT32, "gaia_f_one_hand_sword"},
	 {1, TYPE_INT32, "gaia_f_two_hand_sword"},
	 {1, TYPE_INT32, "gaia_f_double_sword"},
	 {1, TYPE_INT32, "gaia_f_one_hand_dagger"},
	 {1, TYPE_INT32, "gaia_f_double_dagger"},
	 {1, TYPE_INT32, "gaia_f_two_hand_spear"},
	 {1, TYPE_INT32, "gaia_f_one_hand_axe"},
	 {1, TYPE_INT32, "gaia_f_two_hand_axe"},
	 {1, TYPE_INT32, "gaia_f_double_axe"},
	 {1, TYPE_INT32, "gaia_f_one_hand_staff"},
	 {1, TYPE_INT32, "gaia_f_two_hand_staff"},
	 {1, TYPE_INT32, "gaia_f_one_hand_mace"},
	 {1, TYPE_INT32, "gaia_f_two_hand_mace"},
	 {1, TYPE_INT32, "gaia_f_bow"},
	 {1, TYPE_INT32, "gaia_f_cross_bow"}};

#pragma comment(linker, "/EXPORT:registerDBStructure=_registerDBStructure@8")
void EDATABASEDLL DLLCALLCONV registerDBStructure(FieldDescriptor **dfmPtr, int *sizePtr) {
	*dfmPtr = df;
	*sizePtr = sizeof(df) / sizeof(FieldDescriptor);
}

#ifdef __cplusplus
}
#endif

