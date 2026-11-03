/*
 * steer.c
 *
 *  Created on: 2023年10月18日
 *      Author: sundm
 */
#include "steer.h"

#define SERVO_FREQ              (100 )                                           // 定义主板上舵机频率  请务必注意范围 50-300
#define STEER   ATOM1_CH1_P33_9
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

void steer_init(void)
{
    uint32 duty;
    pwm_init(STEER, SERVO_FREQ, STEER_MIDDLE);
}

void steer_duty(uint32 duty)
{
    pwm_set_duty(STEER, func_limit_ab(duty, STEER_R_MIN, STEER_L_MAX));//注意有限制 SERVO_L_MAX ，SERVO_R_MIN
}



