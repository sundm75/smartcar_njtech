/*
 * encoder.h
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#ifndef _ENCODER_H
#define _ENCODER_H
#include "zf_common_headfile.h"

#define ENCODER_SPEED_K        17.322412073168246793464068f  //PI*0.064/512/68*30/0.01  mm/s
#define ENCODER_DISTANCE_K     0.17322412073168246793464068f  //PI*0.064/512/68*30   mm

//866mm/s  对应 enc 速度 50  

void encoder_init(void);
void update_speed_and_distance(void);

int get_right_encoder(void);
int get_left_encoder(void);
float get_left_speed(void);
float get_right_speed(void);
float get_distance(void);
float get_speed(void);

#endif
