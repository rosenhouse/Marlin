/*
	fsr_abl.h contains the functions needed to operate automatic bed leveling with force sensing resistors.

*/
#ifndef fsr_abl_h
#define fsr_abl_h
#endif

#ifdef FSR_BED_LEVELING
int FSR_ABL_Reading();
int FSR_ABL_Get_Read();
int FSR_ABL_Get_Avg();
bool FSR_ABL_Trigger();
#endif