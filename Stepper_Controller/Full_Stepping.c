/*
 * Full_Stepping.c
 *
 * Created: 2024-07-07 12:08:51 AM
 *  Author: ak915
 */ 

#include "Full_Stepping.h"

static volatile enum full_step current_step = T1_A;

void update_full_stepping(full_stepper *channel) {
	switch(current_step) {
		case T1_A:
			//Phase A operation
			if(!(channel->reverse)) {
				PORTB |= (1 << PHASE_A_POSITIVE);
			} else {
				PORTB |= (1 << PHASE_A_NEGATIVE);
			}
		
			//Phase B operation
			PORTB &= ~((1 << PHASE_B_POSITIVE) | (1 << PHASE_B_NEGATIVE));
			current_step = T2_A;
			break;
		
		case T2_A:
			//Phase A Operation
			PORTB &= ~((1 << PHASE_A_POSITIVE) | (1 << PHASE_A_NEGATIVE));
		
			//Phase B operation
			PORTB |= (1 << PHASE_B_POSITIVE);
			current_step = T3_A;
			break;
		
		case T3_A:
			//Phase A operation
			if(!(channel->reverse)) {
				PORTB &= ~(1 << PHASE_A_POSITIVE);
				PORTB |= (1 << PHASE_A_NEGATIVE);
			} else {
				PORTB &= ~(1 << PHASE_A_NEGATIVE);
				PORTB |= (1 << PHASE_A_POSITIVE);
			}
		
			//Phase B operation
			PORTB &= ~((1 << PHASE_B_POSITIVE) | (1 << PHASE_B_NEGATIVE));
			current_step = T4_A;
			break;
		
		case T4_A:
			//Phase A operation
			PORTB &= ~((1 << PHASE_A_POSITIVE) | (1 << PHASE_A_NEGATIVE));
		
			//Phase B operation
			PORTB &= ~(1 << PHASE_B_POSITIVE);
			PORTB |= (1 << PHASE_B_NEGATIVE);
			current_step = T1_A;
			
			//update the angle information
			if(!(channel->reverse)) {
				channel->angle = (channel->angle)+1.8;
			} else {
				channel->angle = (channel->angle)-1.8;
			}
		
			break;
	}
}


//freq = 1/(T_step); sets up CTC Mode.
void init_timer1_full_stepping (uint16_t freq, full_stepper *channel) {
	//Frequency = F_CPU/(2*N*(1+Value))
	
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = (CPU_FREQ/(2*256*freq))-1;
	
	DDRB |= (1 << DDB6)|(1 << DDB7) | (1 << DDB1) | (1 << DDB2);
	
	channel->angle = 0;
	channel->reverse = 0;
}

void change_angle(full_stepper *channel, int16_t new_angle) {
	//disables interrupt to make sure that angle information is changed only in one place
	TIMSK1 &= ~(1 << OCIE1A);
	channel->angle = new_angle;
	
	//enables interrupt again
	TIMSK1 |= (1 << OCIE1A);
}