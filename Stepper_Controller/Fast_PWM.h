/*
 * Fast_PWM.h
 *
 * Created: 2024-06-28 1:01:27 PM
 *  Author: ak915
 */ 


#ifndef FAST_PWM_H_
#define FAST_PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"



//output pin options
#define OUT_PIN_A 1
#define OUT_PIN_B 0
#define OUT_PIN_ALL 2

//frequency options
#define LOW_FREQ 0
#define MEDIUM_FREQ 1
#define HIGH_FREQ 2

struct current_status_pwm {
	uint16_t duty_cycle;
	uint16_t PWM_frequency;
	uint8_t duty_cycle_change_status; //-1 equals decrease, 1 equals increase, 0 is when no change
	uint16_t duty_cycle_change_amt;
};


//function prototypes
void init_fast_pwm_timer1(uint16_t duty, uint8_t prescaler_option, uint8_t output_option, uint8_t freq_option);

void init_timer0_CTC(uint8_t prescaler_option, uint8_t frequency);

void set_duty_cycle_to_increase(uint16_t amt);

void set_duty_cycle_to_decrease(uint16_t amt);

uint16_t return_duty_cycle(void);

#endif /* FAST_PWM_H_ */