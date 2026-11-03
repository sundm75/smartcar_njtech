/*
 * modor.h
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#ifndef _MOTOR_H
#define _MOTOR_H
#include "zf_common_headfile.h"

#define PWM_MAX 4000

void motor_init(void);
void right_motor_duty(int duty);
void left_motor_duty(int duty);
void motor_duty(int l, int r);

int get_left_duty(void) ;
int get_right_duty(void) ;

#endif

