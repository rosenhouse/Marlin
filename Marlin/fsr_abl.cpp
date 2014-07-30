/*
	fsr_abl.cpp contains the functions needed to operate automatic bed leveling with force sensing resistors.

*/
#if defined (FSR_BED_LEVELING) && defined (FSR_PIN) && (FSR_PIN > -1)
// Public Variables

// Private Variables (static preserves between function calls and prevents calling by extern)
static int fsr_rolling_avg;
static int fsr_weighting = 1;
static int fsr_checks = 5; // Number of times to run ADC

int FSR_ABL_Reading()
{
	int fsr_reading = 0;
	#if defined FSR_PIN && FSR_PIN > -1
	// Averages fsr_checks readings
	for (int i=fsr_checks; i>0; i--){
		#if FSR_PIN > 7
			ADCSRB = 1<<MUX5;
		#else
			ADCSRB = 0;
		#endif
		ADMUX = ((1 << REFS0) | (FSR_PIN & 0x07));
		ADCSRA |= 1<<ADSC; // Start conversion
		fsr_reading += ADC;
		}
	fsr_reading /= fsr_checks;
	fsr_rolling_avg = fsr_rolling_avg*fsr_weighting;
	fsr_rolling_avg = fsr_rolling_avg+fsr_reading;
	fsr_rolling_avg = fsr_rolling_avg/(fsr_weighting+1);	  
	#endif
	return(fsr_reading)
}

int FSR_ABL_Get_Avg()
{
	return(fsr_rolling_avg);
}

// FSR_ABL_Trigger reads the adc and checks to see if this value meets the trigger conditions
bool FSR_ABL_Trigger()
{
	int fsr_reading = 0;
	#if FSR_PIN > 7
		ADCSRB = 1<<MUX5;
	#else
		ADCSRB = 0;
	#endif
		ADMUX = ((1 << REFS0) | (FSR_PIN & 0x07));
		ADCSRA |= 1<<ADSC; // Start conversion
		fsr_reading = ADC;
// Check filter for trigger conditions
if (((fsr_reading > 1.3*fsr_rolling_avg) || (fsr_reading < .5*fsr_rolling_avg) || (fsr_reading > 600)) && (fsr_reading > 50)){
	SERIAL_ECHO_START;
	SERIAL_ECHOPGM("Read: ");
	SERIAL_ECHOLN(fsr_reading);
	SERIAL_ECHOPGM(" Roll: ");
	SERIAL_ECHOLN(fsr_rolling_avg);
	SERIAL_PROTOCOLLN("");
	return(true);
	}
	// Update rolling average
  else{
	//fsr_rolling_avg = fsr_rolling_avg*fsr_weighting;
	//fsr_rolling_avg = fsr_rolling_avg+fsr_reading;
	//fsr_rolling_avg = fsr_rolling_avg/(fsr_weighting+1);
	return(false);
	}
}

#endif
