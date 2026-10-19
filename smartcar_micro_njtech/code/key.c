/*
 * key.c
 *
 *  Created on: 2023年10月19日
 *      Author: sundm
 */
#include <key.h>
#include "zf_driver_gpio.h"

#define SWITCH1                 (P11_3)
#define SWITCH2                 (P11_2)

#define KEY1                    (P13_3)
#define KEY2                    (P13_2)
#define KEY3                    (P13_1)
#define KEY4                    (P13_0)

#define HALL_PIN                (P00_8)


uint8 sw1_status;
uint8 sw2_status;
uint8 sw_value;
uint8 hall_value;

uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;  //key4 检测双击 长按 单击 这里不用再写代码
uint8 key_value = 0;

uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;
uint8 is_editing_flag = 0;

void usr_key_init(void)
{
    gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(HALL_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
}

int key3_pressed_flag = 0;
void key_Handle()
{
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;

    key1_status = gpio_get_level(KEY1);
    key2_status = gpio_get_level(KEY2);
    key3_status = gpio_get_level(KEY3);
    key4_status = gpio_get_level(KEY4);
    hall_value = gpio_get_level(HALL_PIN);


    sw_value = (!gpio_get_level(SWITCH1))<<1 | !(gpio_get_level(SWITCH2));

    if(key1_status && !key1_last_status)    key1_flag = 1;
    if(key2_status && !key2_last_status)    key2_flag = 1;
    if(key3_status && !key3_last_status)    key3_flag = 1;
    if(key4_status && !key4_last_status)    key4_flag = 1;

    if(key1_flag)
    {
        key_value = 1;
        key1_flag = 0;
    }

    if(key2_flag)
    {
        key_value = 2;
        key2_flag = 0;
    }

    if(key3_flag)
    {
        key_value = 3;
        key3_flag = 0;

    }

    if(key4_flag)
    {
        key_value = 4;
        key4_flag = 0;
    }

}

int get_key_value(void)  // 检测使用按键值 5-切换编辑状态 1-增加 2-减少
{
    int res;

    res = key_value;
    key_value = 0;
    return res;
}

void set_key_value(uint8 val)
{
	key_value = val;
}


uint8 get_is_editing_flag(void)
{
    return is_editing_flag;
}

int get_sw_value(void)
{
    return sw_value;
}

int get_hall_value(void)
{
    return hall_value;
}

