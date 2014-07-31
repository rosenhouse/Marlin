/*
	fsr_abl.cpp contains the functions needed to operate automatic bed leveling with force sensing resistors.

*/
//#ifdef FSR_BED_LEVELING //&& defined FSR_PIN && FSR_PIN > -1
#include "fsr_abl.h"
#include "Marlin.h"
#include "temperature.h"

// Public Variables

// Private Variables (static preserves between function calls and prevents calling by extern)
static int fsr_rolling_avg;
static float fsr_weighting = .25;
static int fsr_checks = 1; // Number of times to run ADC

void FSR_ABL_Reading()
{
	fsr_rolling_avg = fsr_rolling_avg*fsr_weighting;
	fsr_rolling_avg = fsr_rolling_avg+current_fsr_sample();
	fsr_rolling_avg = fsr_rolling_avg/(fsr_weighting+1);
}

int FSR_ABL_Get_Read()
{
	return(current_fsr_sample());
}

int FSR_ABL_Get_Avg()
{
	return(fsr_rolling_avg);
}

// FSR_ABL_Trigger reads the adc and checks to see if this value meets the trigger conditions
bool FSR_ABL_Trigger()
{
// Check filter for trigger conditions
if (((current_fsr_sample() > 1.3*fsr_rolling_avg) || (current_fsr_sample() < .5*fsr_rolling_avg) || (current_fsr_sample() > 600)) && (current_fsr_sample() > 50)){
	SERIAL_ECHO_START;
	SERIAL_ECHOPGM("Read: ");
	SERIAL_ECHOLN(current_fsr_sample());
	SERIAL_ECHOPGM(" Roll: ");
	SERIAL_ECHOLN(fsr_rolling_avg);
	SERIAL_PROTOCOLLN("");
	return(true);
	}
	// Update rolling average
  else{
	// Do nothing but return false
	return(false);
	}
}
//#endif
