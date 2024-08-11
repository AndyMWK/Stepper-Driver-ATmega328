/*
 * Stepper_Controller.c
 *
 * Created: 2024-06-28 1:00:06 PM
 * Author : ak915
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "Full_Stepping.h"

//creates full-stepping struct called channel
full_stepper channel;

ISR(TIMER1_COMPA_vect) {
	//updates full stepping phases
	update_full_stepping(&channel);
}

int main(void)
{
	//initialize full-stepping functionality with 120 Hz
	init_timer1_full_stepping(120, &channel);
    sei();
    while (1) 
    {
    }
}


