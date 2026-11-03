/*
 * encoder.c
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#include "encoder.h"



// **************************** 代码区域 ****************************
#define ENCODER_DIR_1                 	(PWMA_ENCODER)              // 带方向编码器对应使用的编码器接口 
#define ENCODER_DIR_PULSE_1            	(PWMA_ENCODER_CH1P_P60)     // PULSE 对应的引脚
#define ENCODER_DIR_DIR_1              	(PWMA_ENCODER_CH2P_P62)     // DIR 对应的引脚

#define ENCODER_DIR_2                 	(PWMC_ENCODER)              // 带方向编码器对应使用的编码器接口
#define ENCODER_DIR_PULSE_2       		(PWMC_ENCODER_CH1P_P40)     // PULSE 对应的引脚
#define ENCODER_DIR_DIR_2           	(PWMC_ENCODER_CH2P_P42)     // DIR 对应的引脚

#define ENCODER_DIR_3                 	(TIM17_ENCODER)             // 带方向编码器对应使用的编码器接口x
#define ENCODER_DIR_PULSE_3       		(TIM17_ENCODER_CH1_P80)     // PULSE 对应的引脚
#define ENCODER_DIR_DIR_3           	(IO_P44)             	    // DIR 对应的引脚

#define ENCODER_DIR_4                 	(TIM18_ENCODER)             // 带方向编码器对应使用的编码器接口
#define ENCODER_DIR_PULSE_4       		(TIM18_ENCODER_CH1_P90)     // PULSE 对应的引脚
#define ENCODER_DIR_DIR_4           	(IO_P46)             	    // DIR 对应的引脚

int16 encoder_data_dir_1 = 0;
int16 encoder_data_dir_2 = 0;
int16 encoder_data_dir_3 = 0;
int16 encoder_data_dir_4 = 0;


int16 left_encoder = 0;
int16 right_encoder = 0;
int16 encoder = 0;

float left_speed;//mm/s
float right_speed;//mm/s
float distance = 0;//cm

void encoder_init(void)
{
    encoder_dir_init(ENCODER_DIR_1, ENCODER_DIR_PULSE_1, ENCODER_DIR_DIR_1);   	// 初始化编码器模块与引脚 带方向增量编码器模式
    encoder_dir_init(ENCODER_DIR_2, ENCODER_DIR_PULSE_2, ENCODER_DIR_DIR_2);    // 初始化编码器模块与引脚 带方向增量编码器模式
    encoder_dir_init(ENCODER_DIR_3, ENCODER_DIR_PULSE_3, ENCODER_DIR_DIR_3);    // 初始化编码器模块与引脚 带方向增量编码器模式
    encoder_dir_init(ENCODER_DIR_4, ENCODER_DIR_PULSE_4, ENCODER_DIR_DIR_4);    // 初始化编码器模块与引脚 带方向增量编码器模式
}

static void pit_handler (void)
{
	  encoder_data_dir_1 = encoder_get_count(ENCODER_DIR_1);                  // 获取编码器计数
    encoder_data_dir_2 = encoder_get_count(ENCODER_DIR_2);                  // 获取编码器计数
//    encoder_data_dir_3 = encoder_get_count(ENCODER_DIR_3);              	// 获取编码器计数
//    encoder_data_dir_4 = encoder_get_count(ENCODER_DIR_4);              	// 获取编码器计数

    encoder_clear_count(ENCODER_DIR_1);                                		// 清空编码器计数
    encoder_clear_count(ENCODER_DIR_2);                                		// 清空编码器计数
//    encoder_clear_count(ENCODER_DIR_3);                             		// 清空编码器计数
//    encoder_clear_count(ENCODER_DIR_4);                             		// 清空编码器计数
	
    left_encoder = encoder_data_dir_1;
    right_encoder = -encoder_data_dir_2;
}


void update_speed_and_distance(void)
{
    pit_handler();
		encoder = (left_encoder + right_encoder) / 2;
		left_speed = left_encoder * ENCODER_SPEED_K;
	  right_speed = right_encoder * ENCODER_SPEED_K;
    distance += encoder * ENCODER_DISTANCE_K;
}


int get_right_encoder(void)
{
    return right_encoder;
}

int get_left_encoder(void)
{
    return left_encoder;
}

float get_left_speed(void)
{
    return left_speed;
}

float get_right_speed(void)
{
    return right_speed;
}

float get_distance(void)
{
    return distance/10;
}

float get_speed(void)
{
    return (encoder * 1.0 );
}

