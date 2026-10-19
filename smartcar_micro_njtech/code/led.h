/*
 * led.h
 *
 *  Created on: 2023Äê10ÔÂ18ÈÕ
 *      Author: sundm
 */

#ifndef CODE_LED_H_
#define CODE_LED_H_

#include "zf_common_typedef.h"
#include "zf_driver_gpio.h"

#define LED_0 P20_9
#define LED_1 P20_8
#define LED_2 P21_5
#define LED_3 P21_4

void led_init(void);
void led_set(gpio_pin_enum led, uint8 dat);
void led_toggle(gpio_pin_enum led) ;


#endif /* CODE_LED_H_ */
