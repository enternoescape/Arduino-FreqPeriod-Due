/* Frequency & Period Measurement for Arduino
 * Measure frequency on pin 25.
 * Generate frequency for testing on pin 7.
 *
 * Based on the original example from:
 * KHM 2010 /  Martin Nawrath
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne
 */
#include "FreqPeriodDue.h"

double lfrq;
long int pp;
//Use pin 25 for a faster interrupt since it's the first pin on PIOD.
int periodPin = 25;
int frequency = 40000; //Frequency to generate on pin 7.

void setup() {
  Serial.begin(115200);
  FreqPeriod::begin(periodPin); 
  Serial.println("FreqPeriod Library Test");

  //Generate frequency on pin 7 to be measured.
  startPWM(frequency, PIOC, PIO_PERIPH_B, PIO_PC23, PIO_DEFAULT, 6);
}

void loop() {
  pp = FreqPeriod::getPeriod();
  if (pp){
  
    Serial.print("period: ");
    Serial.print(pp);
    Serial.print(" 1/42us  /  frequency: ");

    lfrq= 42000000 / pp;
    Serial.print(lfrq);
    Serial.print(" Hz");
    Serial.println();
  }

}

void startPWM(uint32_t frequency, Pio* port, EPioType pinType, uint32_t pin, uint32_t pinConfig, uint32_t pwmChannel)
{
  uint32_t rc = frequency * 2;
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(PWM_INTERFACE_ID);
  //Set clock A to frequency in hz.
  PWMC_ConfigureClocks(rc, 0, VARIANT_MCK);
  //Changes the port and pin over to the requested peripheral.
  PIO_Configure(port, pinType, pin, pinConfig);
  //Set prescaler (PWM_CMR_CPRE_CLKA), alignment (0) and polarity (0).
  PWMC_ConfigureChannel(PWM_INTERFACE, pwmChannel, PWM_CMR_CPRE_CLKA, 0, 0);
  //Set period to 1MHz divided by the desired frequency.
  PWMC_SetPeriod(PWM_INTERFACE, pwmChannel, 2);
  //Set duty cycle to half of the period.
  PWMC_SetDutyCycle(PWM_INTERFACE, pwmChannel, 1); //50% high, 50% low
  PWMC_EnableChannel(PWM_INTERFACE, pwmChannel);
}