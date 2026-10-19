/*
 * led.c
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#include "led.h"

#define LED1                    (IO_P52 )//这个灯与 moto方向重复，这里不能使用
#define LASER_HEAD_PIN 					(IO_P67 )

void led_init(void)
{
	gpio_init(LED1, GPO, 0, GPO_PUSH_PULL);
	gpio_init(LASER_HEAD_PIN, GPO, 1, GPO_PUSH_PULL);
	gpio_set_level(LASER_HEAD_PIN, 0);
}

void led_set( uint8 dat)
{
    gpio_set_level(LED1, dat); //这个灯与 moto方向重复，这里不能使用
}

void led_toggle(void)
{
    gpio_toggle_level(LED1);   //这个灯与 moto方向重复，这里不能使用
}

void laser_toggle( void)
{
    gpio_toggle_level(LASER_HEAD_PIN);
}




