/*
 * motor.h
 *
 *  Created on: 2023Äê10ÔÂ19ÈÕ
 *      Author: sundm
 */

#ifndef _motor_h
#define _motor_h

#include "zf_common_headfile.h"

#define PWM_MAX 5000

void motor_init(void);
void right_motor_duty(int duty);
void left_motor_duty(int duty);
void motor_duty(int l, int r);

int get_left_duty(void) ;
int get_right_duty(void) ;
    
#endif
