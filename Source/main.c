/*
 * main.c
 *
 *  Created on: Oct 13, 2010
 *      Author: Eric Hein
 * comments further modified May 31, 2012
 */

#include "main.h"

static uint16_t DARKNESS = 800; 	//controls the level of darkness to trigger on
static uint16_t DELAY = 1000;  		//controls the rate of the flicker,  the bigger the number the slower the change

static uint16_t cycles;   //counts iterations of the clock interrupt
static uint16_t wait;     //duration between changes in intensity
static uint8_t wdtTime; 	//1 if device should be sleeping
static uint8_t pulseTime; //1 if device should be flickering the LEDs

//As it says, the main program
int main()
{
	setup(); 		//intialize chip registers

	startDog(); //Put device into deep sleep and set the wake cycle

	while(1) 		//functional loop for whenever the  device wakes
	{
		if (wdtTime) wdt(); 		//Should the device be sleeping? If so, check.
		if (pulseTime) pulse(); //Should the device be flickering? Then flicker!
		sleep_mode(); 					//sleep until the timer overflows or the WDT kicks off
	}
	return 0;
}

//Fucntions defined
void wdt() //if the it is dark out turn the WDT off and start the flicker cycle otherwise go back to sleep
{
					if (readADC() >= DARKNESS)
					{
						stopDog();
						srandom(123);
						timerStart();
					}
					else
					{
						timerStop();
						startDog();
					}
}

void pulse()
{
			if (cycles < wait)   				// are we still waiting? continue to wait
			{
				cycles++;
			}
			else                 				//not waiting, let's do our stuff!
			{
				if (readADC() < DARKNESS) //if it got light out, go to sleep
				{
					startDog();
					timerStop();
				}
				else 											//Not waiting, not light out, well then let us flicker this candle!
				{
					wait = (random()%DELAY);  	//determine new period to wait between changes
					cycles = 0;              		//need to start counting at 0 again for our wait duration
					OCR0A = (random()%128+128); //Set the Yellow LED intensity
					OCR0B = (random()%128+128); //Set the Red LED intesity
				}
			}
}

void startDog() //This function keeps a low-power timer (WDT) going to reset the device after timer overflows
{
	MCUCR |= (1<<SM1);    //Enable Deep Sleep
	WDTCR |= (1<<WDIE) | (1<<WDP3) | (1<<WDP0);   //Turn on Watchdog Timer interrupt in place of reset
	wdtTime = 1; //flags WDT as on
}

void stopDog() // This function disables the WDT
{
	wdt_disable(); //turns off WDT
	wdtTime = 0; //flags WDT off
}

void timerStart() //initialize the settings for the timer cycle, disable sleep, enable PWM, start the timer, and set the mode for flicker operation
{
	PRR &= ~(1<<PRTIM0);
	MCUCR &= ~(1<<SM1);   //Disable Deep Sleep
	TCCR0A |= (1<<WGM00) | (1<<WGM01) | (1<<COM0A1) | (1<<COM0B1); //WGM 00 and 01 set for Fast PWM. COMA and B on for noninverted PWM.
	TCCR0B = (1<<CS00);   //Start Timer0
	pulseTime = 1;
}

void timerStop() //Turns the timer used in pulse() off to save power when sleeping and disables pulsetime
{
	TCCR0A = 0;
	TCCR0B = 0;   //Start Timer0
	TCCR0B &= ~(1<<CS00); //Stop Timer0
	PRR |= (1<<PRTIM0);
	pulseTime = 0;
}

void setup()
{
	cli();
	PRR |= (1<<PRTIM1) | (1<<PRUSI);

	DDRB |= (1<<DDB1); //PortB1 set to output
	DDRB |= (1<<DDB0); //PortB0 set to output
	DDRB |= (1<<DDB3); //PortB3 set to output
	PORTB &= ~(1<<DDB3); //PortB3 set to low(off)
	DDRB &= ~(1<<DDB4); //PortB4 is set to input
	PORTB &= ~(1<<DDB4); //PortB4  pullup resistor is set to off
	TIMSK |= (1<<TOIE0); //Enable Timer0 overflow and compare registers A and B.
	sei();
}

ISR (WDT_vect) //run each time the WDT trips
{
	wdtTime = 1;
}

ISR (TIMER0_OVF_vect)    //The candle function operates off of the interrupt for the timer0.  Each overflow ticks up the cycles count.
{
	pulseTime = 1; //ensures the flickermode stays on.
}

uint16_t readADC() // This functoin reads the phototransistor through the analog/digital converter
{
	uint16_t sum = 0;
	PRR &= ~(1<<PRADC); //Turn power to ADC back on
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1);
	ADCSRA &= ~(1<<ADPS0); //Sets the prescalar(64) for the ADC (datasheet says between 50khz and 200khz)

	PORTB |= (1<<DDB3);
	ADCSRA |= (1<<ADEN);
	ADMUX |= 0b00000010;

	for (int i =0;i<8;i++) // 8x oversampling loop
	{
		ADCSRA |= (1<<ADSC);
		loop_until_bit_is_clear(ADCSRA,ADSC);
		sum += ADC;
	}

	PORTB &= ~(1<<DDB3);
	ADCSRA &= ~(1<<ADEN);
	PRR |= (1<<PRADC);  //Completely turn off ADC module, reduce power consumption while sleeping.
	sum = (sum/8); //Average the value from the 8x oversample
	return sum; //return the attained value
}