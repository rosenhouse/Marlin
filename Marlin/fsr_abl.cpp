/*
	fsr_abl.cpp contains the functions needed to operate automatic bed leveling with force sensing resistors.
*/


#include "fsr_abl.h"
#include "Marlin.h"
#include "temperature.h"

#ifdef FSR_BED_LEVELING //&& defined FSR_PIN && FSR_PIN > -1

// Public Variables

// Private Variables (static preserves between function calls and prevents calling by extern)
static int fsr_rolling_avg;
static float fsr_weighting = 4.5; // Higher weighting means ave changes slower

// Updates rolling average with most recent value from ADC
void FSR_ABL_Reading()
{
	fsr_rolling_avg = fsr_rolling_avg*fsr_weighting;
	fsr_rolling_avg = fsr_rolling_avg+current_fsr_sample();
	fsr_rolling_avg = fsr_rolling_avg/(fsr_weighting+1);
}

// Returns most recent sampled value from ADC
int FSR_ABL_Get_Read()
{
	return(current_fsr_sample());
}

// Returns the current rolling average
int FSR_ABL_Get_Avg()
{
	return(fsr_rolling_avg);
}

// FSR_ABL_Trigger reads the adc and checks to see if this value meets the trigger conditions
bool FSR_ABL_Trigger()
{
// Check filter for trigger conditions
if (((current_fsr_sample() > 1.25*fsr_rolling_avg) || (current_fsr_sample() < .65*fsr_rolling_avg) || (current_fsr_sample() > 520)) && (current_fsr_sample() > 50)){
	return(true);
	}
else{
	// Do nothing but return false
	return(false);
	}
}


#endif
