/*
  fsr_abl.cpp contains the functions needed to operate automatic bed leveling
  with force sensing resistors.
*/

#include "fsr_abl.h"
#include "Marlin.h"
#include "temperature.h"  // contains current_fsr_sample()

#ifdef FSR_BED_LEVELING
   #ifndef FSR_PIN
      #error You must define FSR_PIN
    #endif
   #if FSR_PIN < 0
      #error You must define FSR_PIN >= 0
   #endif

// Private Variables (static preserves between function calls and prevents
// calling by extern)
static int fsr_rolling_avg;
static float fsr_weighting = 4.5; // Higher weighting means ave changes slower
static float fsr_upper_trigger = 1.25; // upper bound % of rolling average for trigger
static float fsr_lower_trigger = 0.65; // lower bound % of rolling average for trigger
static int current_sample;

// Updates rolling average with most recent value from ADC
void FSR_ABL_Reading()
{
  fsr_rolling_avg = fsr_rolling_avg * fsr_weighting;
  fsr_rolling_avg = fsr_rolling_avg + current_fsr_sample();
  fsr_rolling_avg = fsr_rolling_avg / (fsr_weighting + 1);
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

// FSR_ABL_Trigger reads the ADC and checks to see if this value meets the
// trigger conditions
bool FSR_ABL_Trigger()
{
// Check filter for trigger conditions
  current_sample = current_fsr_sample();
  if (((current_sample > fsr_upper_trigger * fsr_rolling_avg)
        || (current_sample < fsr_lower_trigger * fsr_rolling_avg)
        || (current_sample > 520)) && (current_sample > 50)){
    return(true);
    }
  else {
    // Do nothing but return false
    return(false);
    }
}


#endif // FSR_BED_LEVELING
