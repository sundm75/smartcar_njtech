/*
 * my_imu.h
 *
 *  Created on: 2025年1月11日
 *      Author: sundm
 */

#ifndef CODE_MY_IMU_H_
#define CODE_MY_IMU_H_
#include "zf_common_headfile.h"
#include "zf_common_typedef.h"

typedef struct {
    double q0, q1, q2, q3;   // 四元数
    double yaw; // 欧拉角
    double pitch;
} Quaternion;
typedef struct
{
        double Xdata;
        double Ydata;
        double Zdata;
} gyro_param_t;

extern double yaw;
extern gyro_param_t GyroOffset;

void gyroOffset_init (void);

/*基于纯四元数更新（仅陀螺仪数据）*/
void quaternion_init(void);
// 1ms 更新四元数
void update_quaternion(void);

// 获取欧拉角
void get_euler_angles(double *yaw) ;
#endif /* CODE_MY_IMU_H_ */
