#include "motor.h"
/*===============================================================*/

#define MAX_DUTY            ( 50 )                                               // 最大 MAX_DUTY% 占空比

#define DIR_2               ( IO_P50 ) 
#define PWM_2               ( PWMD_CH2_P51 )
                              
#define DIR_1               ( IO_P52 ) 
#define PWM_1               ( PWMD_CH4_P53 )
                              
#define DIR_3               ( IO_P75 )
#define PWM_3               ( PWMB_CH1_P74 )
                              
#define DIR_4               ( IO_P77 )
#define PWM_4               ( PWMB_CH3_P76 )
                              
/*===============================================================*/
int left_duty = 0;
int right_duty = 0;

#define MOTOR_DEAD_LEFT  700     //PWM占空比刻度0~10000，实测左轮死区
#define MOTOR_DEAD_RIGHT 700    //PWM占空比刻度0~10000，实测右轮死区

void motor_init(void)
{
    /* DRV8701 驱动 */
    gpio_init(DIR_1, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_1, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_2, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_2, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_3, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_3, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_4, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_4, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0
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
		
    if(left_duty >= 0) //正转
    {
        /*DRV8701驱动*/
				gpio_set_level(DIR_1, GPIO_LOW);                                   // DIR输出高电平
				pwm_set_duty(PWM_1, left_duty );                   // 计算占空比
    }
    else         //反转
    {
        /*DRV8701驱动*/
				gpio_set_level(DIR_1, GPIO_HIGH);                                    // DIR输出低电平
				pwm_set_duty(PWM_1, (-left_duty) );                // 计算占空比
    }	
}

void right_motor_duty(int duty)
{
	  right_duty = duty;
    right_duty = dead_zone_compensate(right_duty, MOTOR_DEAD_RIGHT);
    right_duty = func_limit_ab(right_duty, -PWM_MAX, PWM_MAX);

    if(right_duty >= 0) //正转
    {
        /*DRV8701驱动*/
            gpio_set_level(DIR_2, GPIO_LOW);                                   // DIR输出高电平
            pwm_set_duty(PWM_2, right_duty );                   // 计算占空比
    }
    else         //反转
    {
        /*DRV8701驱动*/
            gpio_set_level(DIR_2, GPIO_HIGH);                                    // DIR输出低电平
            pwm_set_duty(PWM_2, (-right_duty) );                // 计算占空比
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

