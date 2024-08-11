/*
 * PWM.c
 *
 * Created: 2024-07-14 12:40:45 AM
 *  Author: ak915
 */ 

#include "PWM.h"

static void update_duty_cycle(uint16_t amt, pwm *channel);

void init_pwm(pwm *channel, uint16_t duty, uint16_t frequency, uint8_t prescaler_option) {
	
	//enable timers and set outputs
	TCCR1B |= (1 << WGM12);
	DDRB |= (1 << DDB1) | (1 << DDB2);
	
	//prescaler options
	uint16_t prescaler = 1;
	
	if(prescaler_option == PRESCALER_256) {
		TCCR1B |= (1 << CS12);
		prescaler = 256;
	} else if(prescaler_option == PRESCALER_1024) {
		TCCR1B |= (1 << CS12) | (1 << CS10);
		prescaler = 1024;
	}
	
	TIMSK1 |= (1 << OCIE1A);
	OCR1A= CPU_FREQ/(2*prescaler*frequency);
	
	channel->freq= frequency;
	channel->duty = OCR1A/duty;
	channel->reg_value_initial = OCR1A;
	channel->switch_on_off = 1;
}

void increase_duty_cycle(uint16_t amt, pwm *channel) {
	channel->duty = channel->duty - amt;
}

void decrease_duty_cycle(uint16_t amt, pwm *channel) {
	channel->duty = channel->duty + amt;
}

void interrupt_routine(pwm *channel) {
	if(channel->switch_on_off) {
		PWM_HIGH_OC1;
		OCR1A = channel->reg_value_initial;
		channel->switch_on_off = 0;
	}
	else if(!channel->switch_on_off) {
		update_duty_cycle(100, channel);
		PWM_LOW_OC1;
		OCR1A = channel->duty;
		channel->switch_on_off = 1;
	}
}


static void update_duty_cycle(uint16_t amt, pwm *channel) {
	if(channel->duty < channel->reg_value_initial) {
		channel->duty = channel->duty + amt;
	} else if (channel->duty >= channel->reg_value_initial) {
		channel->duty = 0;
	}
}

