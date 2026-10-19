/*
 * led.h
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#ifndef _LED_h
#define _LED_h
#include "zf_common_headfile.h"

void led_init(void) ;
void led_set( uint8 dat);
void led_toggle(void) ;

void laser_toggle( void);

#endif