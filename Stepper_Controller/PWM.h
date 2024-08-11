/*
 * PWM.h
 *
 * Created: 2024-07-14 12:38:54 AM
 *  Author: ak915
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"

//PWM macros
#define PWM_HIGH_OC1 PORTB |= (1 << PORTB1) | (1 << PORTB2)
#define PWM_LOW_OC1 PORTB &= ~((1 << PORTB1) | (1 << PORTB2))

//initialize PWM struct
typedef struct pwm {
	volatile uint16_t duty;
	volatile uint16_t freq;
	volatile uint8_t switch_on_off;
	volatile uint16_t reg_value_initial;
	} pwm;

//initializes PWM channel 
void init_pwm(pwm* channel, uint16_t duty, uint16_t frequency, uint8_t perscaler_option);
//This updates the PWM
void interrupt_routine(pwm *channel);

//-------above 2 functions are all you need to generate a changing PWM signal-----------

//these functiuons probably do not serve a function
void increase_duty_cycle(uint16_t amt, pwm *channel);
void decrease_duty_cycle(uint16_t amt, pwm *channel);
void set_new_frequency(uint16_t new_frequency);




#endif /* PWM_H_ */