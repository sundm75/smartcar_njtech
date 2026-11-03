#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include "zf_common_headfile.h"

typedef struct
{
    float target_val;           //目标值
    float actual_val;        		//实际值
    float err;            //定义当前偏差值
    float err_next;       //定义下一个偏差值
    float err_last;       //定义最后一个偏差值
    float Kp,Ki,Kd;          		//定义比例、积分、微分系数
    float integral;          		//定义积分值
    float integral_up_limit ;     //设置PID限幅
    float integral_down_limit ;     //设置PID限幅
	  float kg;		/* 陀螺仪系数 */
	  float kp2;      /* 二次项系数 */
} _pid;

extern _pid left_motor_pid,right_motor_pid;
extern _pid steer_pid;
extern _pid turn_pid;

void PID_param_init(void);
void set_pid_target(_pid *pid, float temp_val);
void set_p_i_d(_pid *pid, float p, float i, float d);
float positional_pid_realize(_pid *pid, float actual_val);
float incremental_pid_realize(_pid *pid, float actual_val);
float PID_servo(_pid *pid, float actual_val) ;
void pid_clear(_pid *pid);
#endif
