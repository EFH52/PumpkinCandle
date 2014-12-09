/*
 * main.h
 *
 *  Created on: Oct 20, 2010
 *      Author: Eric Hein
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

void setup();
void srandom(int);
long random();
int initADC();
void startDog();
void stopDog();
void timerStart();
void timerStop();
void pulse();
void wdt();
uint16_t readADC();

#endif /* MAIN_H_ */