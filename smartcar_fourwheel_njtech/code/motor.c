/*
 * motor.c
 *
 *  Created on: 2023年10月18日
 *      Author: sundm
 */

#include "motor.h"

#define MOTOR_LEFT_DIR 0
#define MOTOR_RIGHT_DIR 0

#define DIR_L1               (P21_4)
#define PWM_L1               (ATOM0_CH3_P21_5)
#define DIR_R1               (P21_2)
#define PWM_R1               (ATOM0_CH1_P21_3)

int left_duty = 0;
int right_duty = 0;

#define MOTOR_DEAD_LEFT  900     //PWM占空比刻度0~10000，实测左轮死区
#define MOTOR_DEAD_RIGHT 800    //PWM占空比刻度0~10000，实测右轮死区

void motor_init(void)
{
    gpio_init(DIR_R1, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    pwm_init(PWM_R1, 17000, 0);
    gpio_init(DIR_L1, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    pwm_init(PWM_L1, 17000, 0);
}

static int dead_zone_compensate(int duty, int dead_val)
{
    if(duty > 0 && duty < dead_val)
        duty = dead_val;
    else if(duty < 0 && duty > -dead_val)
        duty = -dead_val;
    return duty;
}

void left_motor_duty(int duty)
{
    left_duty = duty;
    left_duty = dead_zone_compensate(left_duty, MOTOR_DEAD_LEFT);
    left_duty = func_limit_ab(left_duty, -PWM_MAX, PWM_MAX);

    if(0 <= left_duty) //电机1   正转
    {
        gpio_set_level(DIR_L1, GPIO_LOW);
        pwm_set_duty(PWM_L1, left_duty);
    }
    else                //电机1   反转
    {
        gpio_set_level(DIR_L1, GPIO_HIGH);
        pwm_set_duty(PWM_L1, -left_duty);
    }
}

void right_motor_duty(int duty)
{
     right_duty = duty;
    right_duty = dead_zone_compensate(right_duty, MOTOR_DEAD_RIGHT);
    right_duty = func_limit_ab(right_duty, -PWM_MAX, PWM_MAX);

    if(0 <= right_duty) //电机2   正转
    {
        gpio_set_level(DIR_R1, GPIO_LOW);                                   // DIR输出低电平
        pwm_set_duty(PWM_R1, right_duty);               // 计算占空比
    }
    else                //电机2   反转
    {
        gpio_set_level(DIR_R1, GPIO_HIGH);                                  // DIR输出高电平
        pwm_set_duty(PWM_R1, -right_duty);                  // 计算占空比
    }

}
void motor_duty(int l, int r)
{
    left_motor_duty(l);
    right_motor_duty(r);
}

int get_left_duty(void) {
    return left_duty;
}
int get_right_duty(void) {
    return right_duty;
}
