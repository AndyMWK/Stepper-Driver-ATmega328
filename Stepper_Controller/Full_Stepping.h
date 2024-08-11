/*
 * Full_Stepping.h
 *
 * Created: 2024-07-07 12:08:38 AM
 *  Author: ak915
 */ 


#ifndef FULL_STEPPING_H_
#define FULL_STEPPING_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"

//2 Phase  motor phases
#define PHASE_A_POSITIVE PORTB6
#define PHASE_A_NEGATIVE PORTB7
#define PHASE_B_POSITIVE PORTB1
#define PHASE_B_NEGATIVE PORTB2

//Full-stepping states
enum full_step {
	T1_A,
	T2_A,
	T3_A,
	T4_A,
};

//full-stepping struct
typedef struct full_stepper {
	volatile int16_t angle;
	volatile uint8_t reverse;
	} full_stepper;

//this is interrupt service routine for updating the full stepping phases
void update_full_stepping(full_stepper *channel);

//initializes the timer
void init_timer1_full_stepping(uint16_t freq, full_stepper *channel);

//changes the current angle information
void change_angle(full_stepper *channel, int16_t new_angle);

#endif /* FULL_STEPPING_H_ */