/*
 * steer.h
 *
 *  Created on: 2023Äê11ÔÂ7ÈÕ
 *      Author: sundm
 */

#ifndef _STEER_H
#define _STEER_H
#include "zf_common_headfile.h"

#define STEER_MIDDLE  (int16)smartcar_param.steer_middle_value
#define STEER_R_MIN 1280
#define STEER_L_MAX 1600

void steer_init(void);
void steer_duty(uint32 duty);

#endif
