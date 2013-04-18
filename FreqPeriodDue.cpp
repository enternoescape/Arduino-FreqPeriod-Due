/*
  FreqPeriodDue 

  
  
  Based loosely on the FreqPeriod library at:
  Martin Nawrath KHM LAB3
  Kunsthochschule für Medien Köln
  Academy of Media Arts
  http://www.khm.de
  http://interface.khm.de/index.php/labor/experimente/  
*/

#include <FreqPeriodDue.h>


volatile unsigned char  FreqPeriod::f_capt;
volatile unsigned long FreqPeriod::capta;
volatile unsigned long  FreqPeriod::captd;
volatile unsigned int FreqPeriod::ocnt;

void FreqPeriod::begin(int periodPin){  
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)FR_USE_TC_IRQ);
  TC_Configure(FR_USE_TC, FR_USE_CH, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1);
  const uint32_t rc = (VARIANT_MCK / 2); //4.2us overflows.
  TC_SetRA(FR_USE_TC, FR_USE_CH, rc/2);
  TC_SetRC(FR_USE_TC, FR_USE_CH, rc);
  TC_Start(FR_USE_TC, FR_USE_CH);
  FR_USE_TC->TC_CHANNEL[FR_USE_CH].TC_IER=TC_IER_CPCS;
  FR_USE_TC->TC_CHANNEL[FR_USE_CH].TC_IDR=~TC_IER_CPCS;
  NVIC_EnableIRQ(FR_USE_TC_IRQ);
  //A compromise for now until I can find a way to have the interrupt and not cause possible conflicts with other libraries using the same PIO.
  attachInterrupt(periodPin, FreqPulse, FALLING);
}

unsigned long FreqPeriod::getPeriod() {
	unsigned long rr = 0; 
	if ( FreqPeriod::f_capt)
	{
		FreqPeriod::f_capt = 0;
		rr = FreqPeriod::captd;
	}
	return(rr);
}

void FreqPeriod::FreqPulse()
{
	//Get the period between pulses.
	FreqPeriod::captd = (FR_USE_TC->TC_CHANNEL[FR_USE_CH].TC_CV + (FreqPeriod::ocnt * (VARIANT_MCK / 2))) - FreqPeriod::capta;
	//Record current timer clock value.
	FreqPeriod::capta = FR_USE_TC->TC_CHANNEL[FR_USE_CH].TC_CV;
	//Reset overflow counter.
	FreqPeriod::ocnt = 0;
	//Set the captured period flag.
	FreqPeriod::f_capt=1;
}

//Count the number of overflows.
void FR_TIMER_INTR_NAME()
{	
	FR_USE_TC->TC_CHANNEL[FR_USE_CH].TC_SR;
	FreqPeriod::ocnt++; //4.2us
}

