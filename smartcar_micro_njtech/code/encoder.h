/*
 * encoder.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */
#ifndef _encoder_h
#define _encoder_h
#include "zf_common_headfile.h"

#define ENCODER_SPEED_K       8.307095957733457742822
#define ENCODER_DISTANCE_K    0.08307095957733457742822

void encoder_init(void);
void update_speed_and_distance(void);

int get_right_encoder(void);
int get_left_encoder(void);
float get_left_speed(void);
float get_right_speed(void);
float get_distance(void);
float get_speed(void);

#endif
