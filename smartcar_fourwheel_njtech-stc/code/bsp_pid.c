#include "bsp_pid.h"

_pid left_motor_pid,right_motor_pid;
_pid steer_pid;
_pid turn_pid;
/**
  * @brief  PID参数初始化
	*	@note 	无
  * @retval 无
  */
void PID_param_init(void)
{
    left_motor_pid.Kp = smartcar_param.motor_kp;
    left_motor_pid.Ki = smartcar_param.motor_ki;
    left_motor_pid.Kd = smartcar_param.motor_kd;
	  left_motor_pid.integral_up_limit = 1800;
	  left_motor_pid.integral_down_limit = -1800;
	
    right_motor_pid = left_motor_pid;

		steer_pid.Kp = smartcar_param.bend_steer_kp;
		steer_pid.Ki = 0;
		steer_pid.Kd = smartcar_param.bend_steer_kd;
	  steer_pid.kp2 = smartcar_param.motor_kp2;  	/* 二次项 0.23*/
	  steer_pid.kg = smartcar_param.motor_kgyro;  	/* 陀螺仪补偿  0.01*/ 
		steer_pid.integral_up_limit = 500;
		steer_pid.integral_down_limit = -500;
	  steer_pid.target_val = 95; //目标中线94 车头左时中线80-160  车头右时中线 0-80
	
		turn_pid.Kp = 4.5;
		turn_pid.Ki = 0;
		turn_pid.Kd = 2.0;
		turn_pid.integral_up_limit = 500;
		turn_pid.integral_down_limit = -500;
}

/**
  * @brief  设置目标值
  * @param  val		目标值
	*	@note 	无
  * @retval 无
  */
void set_pid_target(_pid *pid, float temp_val)
{
    pid->target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
	*	@note 	无
  * @retval 目标值
  */
float get_pid_target(_pid *pid)
{
    return pid->target_val;    // 设置当前的目标值
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
	*	@note 	无
  * @retval 无
  */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
    pid->Kp = p;    // 设置比例系数 P
    pid->Ki = i;    // 设置积分系数 I
    pid->Kd = d;    // 设置微分系数 D
}

/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */
float positional_pid_realize(_pid *pid, float actual_val)
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;

    pid->integral += pid->err;

    pid->actual_val = pid->Kp * pid->err +  
                      pid->Ki * pid->integral +
                      pid->Kd * (pid->err - pid->err_next);

    pid->err_last = pid->err_next;
    pid->err_next = pid->err;
    return pid->actual_val;
}

extern signed short imu_gyro_X, imu_gyro_Y, imu_gyro_Z; 
// 简化版舵机PID控制
float PID_servo(_pid *pid, float actual_val) 
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val ;
    pid->integral += pid->err;

    pid->actual_val = pid->Kp * pid->err +  //只有 P项
                      pid->kp2 * pid->err * func_abs(pid->err) // + 二次项
                      - pid->kg * imu_gyro_Z; //(减去陀螺仪补偿)

    pid->err_last = pid->err_next;
    pid->err_next = pid->err;
    return pid->actual_val;
}


/**
  * @brief  增量PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */
float incremental_pid_realize(_pid *pid, float actual_val)
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;
	
	  /* 限定闭环死区*/
    if((pid->err<10.0f ) && (pid->err>-10.0f))
        pid->err = 0.0f;

		/*抗积分饱和：输出已到限幅且误差方向仍使其更饱和时，本拍不再累加*/
    if((pid->actual_val >=  pid->integral_up_limit  && pid->err > 0) ||
       (pid->actual_val <= pid->integral_down_limit && pid->err < 0))
    {
        pid->err_last = pid->err_next;
        pid->err_next = pid->err;
        return pid->actual_val;
    }
		
    /*PID算法实现*/
    pid->actual_val += pid->Kp*(pid->err - pid->err_next)
                       + pid->Ki*pid->err
                       + pid->Kd*(pid->err - 2 * pid->err_next + pid->err_last);
		
    /*输出钳位，防止实际值累积超出占空比范围*/
    if(pid->actual_val >  pid->integral_up_limit)
        pid->actual_val =  pid->integral_up_limit;
    else if(pid->actual_val < pid->integral_down_limit)
        pid->actual_val = pid->integral_down_limit;

    /*传递误差*/
    pid->err_last = pid->err_next;
    pid->err_next = pid->err;
    /*返回当前实际值*/
    return pid->actual_val;
}

/**
  * @brief  清零PID内部累计状态（目标值、输出累计、误差历史）
  * @param  pid		目标PID
  * @retval 无
  */
void pid_clear(_pid *pid)
{
    pid->target_val = 0;
    pid->actual_val = 0;
    pid->err       = 0;
    pid->err_next  = 0;
    pid->err_last  = 0;
    pid->integral  = 0;
}