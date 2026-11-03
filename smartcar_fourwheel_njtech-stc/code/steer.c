/*
 * steer.c
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#include "steer.h"
#define CHANNEL_NUMBER          (4)
#define SERVO_PWM1              (PWME_CH1P_PA0)                         // 定义主板上舵机对应引脚
#define SERVO_PWM2              (PWME_CH2P_PA2)                         // 定义主板上舵机对应引脚
#define SERVO_PWM3              (PWME_CH3P_PA4)                         // 定义主板上舵机对应引脚
#define SERVO_PWM4              (PWME_CH4P_PA6)                         // 定义主板上舵机对应引脚

#define SERVO_FREQ              (100 )                                           // 定义主板上舵机频率  请务必注意范围 50-300


// ------------------ 舵机占空比计算方式 ------------------
//
// 舵机对应的 0-180 活动角度对应 控制脉冲的 0.5ms-2.5ms 高电平
//
// 那么不同频率下的占空比计算方式就是
// PWM_DUTY_MAX/(1000/freq)*(1+Angle/180) 在 50hz 时就是 PWM_DUTY_MAX/(1000/50)*(1+Angle/180)
//
// 那么 100hz 下 90度的打角 即高电平时间1.5ms 计算套用为
// PWM_DUTY_MAX/(1000/100)*(1+90/180) = PWM_DUTY_MAX/10*1.5
//
// ------------------ 舵机占空比计算方式 ------------------
#define SERVO_DUTY(x)         (PWM_DUTY_MAX / (1000.0 / SERVO_FREQ) * (1 + (float)(x) / 180.0))
#if ((SERVO_FREQ < 50) || (SERVO_FREQ > 300))
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

int16 duty = 0;
uint8 channel_index = 0;
pwm_channel_enum channel_list[CHANNEL_NUMBER] = {SERVO_PWM1, SERVO_PWM2, SERVO_PWM3, SERVO_PWM4};

void steer_init(void)
{
    for(channel_index = 0; channel_index < CHANNEL_NUMBER; channel_index++) 
    {
        pwm_init(channel_list[channel_index], SERVO_FREQ,  SERVO_DUTY(90));
    }
		channel_index = 0;
		steer_duty(1500);
}

void steer_duty(uint32 duty)
{	
	  pwm_set_duty(channel_list[channel_index], func_limit_ab(duty, STEER_R_MIN, STEER_L_MAX)); 

}
