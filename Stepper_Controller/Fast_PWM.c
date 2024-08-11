/*
 * Fast_PWM.c
 *
 * Created: 2024-06-28 1:17:13 PM
 *  Author: ak915
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Fast_PWM.h"

static volatile struct current_status_pwm status;


//initialization functions
void init_timer0_CTC(uint8_t prescaler_option, uint8_t frequency) {
	
	TCCR0A |= (1 << WGM01);
	
	uint16_t prescaler_value = 1;
	//setting up prescaler
	if(prescaler_option == NO_PRESCALER) {
		TCCR0B |= (1 << CS00);
		} else if(prescaler_option == PRESCALER_8) {
		TCCR0B |= (1 << CS01);
		prescaler_value = 8;
		} else if(prescaler_option == PRESCALER_64) {
		TCCR0B |= (1 << CS01) | (1 << CS00);
		prescaler_value = 64;
		} else if(prescaler_option == PRESCALER_256) {
		TCCR0B |= (1 << CS02);
		prescaler_value = 256;
		} else if(prescaler_option == PRESCALER_1024) {
		TCCR0B |= (1 << CS02) | (1 << CS00);
		prescaler_value = 1024;
	}
	
	//enable interrupt
	TIMSK1 |= (1 << OCIE0A);
	OCR0A = 200;
	
	DDRB |= (1 << DDB6);
}

void init_fast_pwm_timer1(uint16_t duty, uint8_t prescaler_option, uint8_t output_option, uint8_t freq_option) {
	//setting output pins
	if(output_option == OUT_PIN_A) {
		TCCR1A |= (1 << COM1A1);
		DDRB |= (1 << DDB1);
	} else if (output_option == OUT_PIN_B) {
		TCCR1A |= (1 << COM1B1);
		DDRB |= (1 << DDB2);
	} else if(output_option == OUT_PIN_ALL) {
		TCCR1A |= (1 << COM1B1) | (1 << COM1A1);
		DDRB |= (1 << DDB1) | (1 << DDB2);
	}
	
	//setting up frequency
	if(freq_option == LOW_FREQ) {
		TCCR1B |= (1 << WGM12);
		TCCR1A |= (1 << WGM11) | (1 << WGM10);
	} else if(freq_option == MEDIUM_FREQ) {
		TCCR1B |= (1 << WGM12);
		TCCR1A |= (1 << WGM11);
	} else if (freq_option == HIGH_FREQ) {
		TCCR1B |= (1 << WGM12);
		TCCR1A |= (1 << WGM10);
	}
	
	uint16_t prescaler_value = 1;
	//setting up prescaler
	if(prescaler_option == NO_PRESCALER) {
		TCCR1B |= (1 << CS10);
	} else if(prescaler_option == PRESCALER_8) {
		TCCR1B |= (1 << CS11);
		prescaler_value = 8;
	} else if(prescaler_option == PRESCALER_64) {
		TCCR1B |= (1 << CS11) | (1 << CS10);
		prescaler_value = 64;
	} else if(prescaler_option == PRESCALER_256) {
		TCCR1B |= (1 << CS12);
		prescaler_value = 256;
	} else if(prescaler_option == PRESCALER_1024) {
		TCCR1B |= (1 << CS12) | (1 << CS10);
		prescaler_value = 1024;
	}
	
	//enable interrupt
	TIMSK1 |= (1 << OCIE1A);
	
	OCR1A = duty;
	
	//handle the current status variable
	status.duty_cycle = duty;
	status.PWM_frequency = CPU_FREQ/(prescaler_value*2*(duty+1));
	status.duty_cycle_change_status = 0;
}

void set_duty_cycle_to_increase(uint16_t amt) {
	status.duty_cycle_change_amt = amt;
	status.duty_cycle_change_status = 1;
}

void set_duty_cycle_to_decrease(uint16_t amt) {
	status.duty_cycle_change_amt = amt;
	status.duty_cycle_change_status = -1;
}
uint16_t return_duty_cycle(void) {
	return OCR1A;
}


