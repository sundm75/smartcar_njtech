/*
 * led.c
 *
 *  Created on: 2023年10月18日
 *      Author: sundm
 */
#include "led.h"

void led_init(void)
{
    gpio_init(LED_0, GPO, 0, GPO_PUSH_PULL);//设置P20_8为输出 默认输出低电平  PUSHPULL：推挽输出
    gpio_init(LED_1, GPO, 0, GPO_PUSH_PULL);
//    gpio_init(LED_2, GPO, 0, GPO_PUSH_PULL);
//    gpio_init(LED_3, GPO, 0, GPO_PUSH_PULL);

    gpio_set_level(LED_0, 1);
    gpio_set_level(LED_1, 1);
//    gpio_set_level(LED_2, 1);
//    gpio_set_level(LED_3, 1);
}

void led_set(gpio_pin_enum led, uint8 dat)
{
    switch (led)
    {
        case 0:
            gpio_set_level(LED_0,dat);
            break;
        case 1:
            gpio_set_level(LED_1,dat);
            break;
//        case 2:
//            gpio_set_level(LED_2,dat);
//            break;
//        case 3:
//            gpio_set_level(LED_3,dat);
//            break;
    }
}
void led_toggle(gpio_pin_enum led)
{
    switch (led)
    {
        case LED_0:
            gpio_toggle_level(LED_0);
            break;
        case LED_1:
            gpio_toggle_level(LED_1);
            break;
//        case LED_2:
//            gpio_toggle_level(LED_2);
//            break;
//        case LED_3:
//            gpio_toggle_level(LED_3);
//            break;
        default:
            break;
    }

}



