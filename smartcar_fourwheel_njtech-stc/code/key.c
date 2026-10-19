/*
 * key.c
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#include "key.h"

#define KEY1_PIN    IO_PB2
#define KEY2_PIN    IO_PB3
#define KEY3_PIN    IO_PB4
#define KEY4_PIN    IO_P32



uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

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

typedef enum
{
	KEY_UP   = 0,      // 按键抬起状态
	KEY_DEBOUNCE,      // 按键消抖状态
	KEY_WAIT_RELEASE   // 按键等待释放状态
}KEY_STATE;

void key_init(void)
{
	gpio_init(KEY1_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY2_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY3_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY4_PIN, GPI, 1, GPI_PULL_UP);
}

void key_Handle()
{
		static KEY_STATE KeyState = KEY_UP;     // 按键状态指示变量，初值为按键抬起状态	
    static uint16 pressed_time = 0;
		static uint8 key_numb = 0;	
	
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;


    key1_status = gpio_get_level(KEY1_PIN);
    key2_status = gpio_get_level(KEY2_PIN);
    key3_status = gpio_get_level(KEY3_PIN);
    key4_status = gpio_get_level(KEY4_PIN);

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
int get_key_value(void)  // 检测使用按键值 5-切换编辑状态 1/3-增加 2/4-减少
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
