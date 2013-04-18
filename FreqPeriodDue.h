/*
  FreqPeriodDue.h
  
  Based loosely on the FreqPeriod library from:
  FreqPeriod.h - Library for a Frequency Measurement c.
  Created by Martin Nawrath, KHM Lab3, June. 2010
  Released into the public domain.
*/

#ifndef FreqPeriod_h
#define FreqPeriod_h

#include "Arduino.h"

//#define FR_USE_TC3
#define FR_USE_TC4
//#define FR_USE_TC5

#if defined(FR_USE_TC3) //Timer clock 3
	#define FR_USE_TC_IRQ 	TC3_IRQn
	#define FR_USE_TC 		TC1
	#define FR_USE_CH 		0
	#define FR_TIMER_INTR_NAME	TC3_Handler
#elif defined(FR_USE_TC4) //Timer clock 4
	#define FR_USE_TC_IRQ 	TC4_IRQn
	#define FR_USE_TC 		TC1
	#define FR_USE_CH 		1
	#define FR_TIMER_INTR_NAME	TC4_Handler
#elif defined(FR_USE_TC5) //Timer clock 5
	#define FR_USE_TC_IRQ 	TC5_IRQn
	#define FR_USE_TC 		TC1
	#define FR_USE_CH 		2
	#define FR_TIMER_INTR_NAME	TC5_Handler
#endif

namespace FreqPeriod {


extern volatile unsigned char  f_capt;
extern volatile unsigned long capta;
extern volatile unsigned long captd;
extern volatile unsigned int ocnt;
	
	void FreqPulse();
	void begin(int periodPin);
	unsigned long getPeriod();	
}

#endif
