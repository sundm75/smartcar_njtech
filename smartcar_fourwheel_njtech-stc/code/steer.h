/*
 * steer.h
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */
#ifndef _STEER_H
#define _STEER_H
#include "zf_common_headfile.h"


#define STEER_MIDDLE smartcar_param.steer_middle_value 

#define STEER_R_MIN  (1385)  // ÓÒÆ«
#define STEER_L_MAX (1630)  // ×óÆ«

void steer_init(void);
void steer_duty(uint32 duty);

#endif
