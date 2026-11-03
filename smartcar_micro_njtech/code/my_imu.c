/*
 * my_imu.c
 *
 *  Created on: 2025年1月11日
 *      Author: sundm
 */
#include "my_imu.h"

static Quaternion quat = {1.0, 0.0, 0.0, 0.0, 0.0};

#define DT 0.001
double yaw;
double pitch;
gyro_param_t GyroOffset;

void gyroOffset_init (void)
{
    int32 acc_X = 0;
    int32 acc_Y = 0;
    int32 acc_Z = 0;
    int32 gyro_X = 0;
    int32 gyro_Y = 0;
    int32 gyro_Z = 0;
    int i;

    GyroOffset.Xdata = 0.0;
    GyroOffset.Ydata = 0.0;
    GyroOffset.Zdata = 0.0;
    for (i = 0; i < 200; ++i)
    {

        imu660ra_get_gyro();
        GyroOffset.Xdata += imu660ra_gyro_x;
        GyroOffset.Ydata += imu660ra_gyro_y;
        GyroOffset.Zdata += imu660ra_gyro_z;

        system_delay_ms(10);
    }
    GyroOffset.Xdata /= 200.0;
    GyroOffset.Ydata /= 200.0;
    GyroOffset.Zdata /= 200.0;
}

void quaternion_init (void)
{
    quat.q0 = 1.0;
    quat.q1 = 0.0;
    quat.q2 = 0.0;
    quat.q3 = 0.0;
}


//缺省值：IMU660RA_ACC_SAMPLE_SGN_8G    IMU660RA_GYRO_SAMPLE_SGN_2000DPS
//IMU660RA_ACC_SAMPLE_SGN_8G 加速度计量程 ±8G  G = g 重力加速度 g≈9.80 m/s^2
//获取到的加速度计数据 除以4096 （65536/16）     可以转化为带物理单位的数据，单位：g(m/s^2)
//IMU660RA_GYRO_SAMPLE_SGN_2000DPS  陀螺仪量程 ±2000DPS  DPS = Degree Per Second 角速度单位 °/S
//获取到的陀螺仪数据除以16.384 （65536/4000）    可以转化为带物理单位的数据，单位为：°/s

void update_quaternion(void) {//1ms运行 如果 pitch显示的是roll, 则将以下的 x  y 读取值 对调

    imu660ra_get_acc();
    imu660ra_get_gyro();
    double gy = (double)((imu660ra_gyro_x - GyroOffset.Xdata)/10*10) /  16.384 * PI / 180;
    double gx = (double)((imu660ra_gyro_y - GyroOffset.Ydata)/10*10) /  16.384 * PI / 180;
    double gz = (double)((imu660ra_gyro_z - GyroOffset.Zdata)/10*10) /  16.384 * PI / 180;

    double qDot0 = 0.5 * (-quat.q1 * gx - quat.q2 * gy - quat.q3 * gz);
    double qDot1 = 0.5 * ( quat.q0 * gx + quat.q2 * gz - quat.q3 * gy);
    double qDot2 = 0.5 * ( quat.q0 * gy - quat.q1 * gz + quat.q3 * gx);
    double qDot3 = 0.5 * ( quat.q0 * gz + quat.q1 * gy - quat.q2 * gx);

    quat.q0 += qDot0 * DT;
    quat.q1 += qDot1 * DT;
    quat.q2 += qDot2 * DT;
    quat.q3 += qDot3 * DT;

    double norm = func_sqrt(quat.q0 * quat.q0 + quat.q1 * quat.q1 + quat.q2 * quat.q2 + quat.q3 * quat.q3);
    if (norm > 1e-12)
	 { 
        quat.q0 /= norm;
        quat.q1 /= norm;
        quat.q2 /= norm;
        quat.q3 /= norm;
    }
   quat.yaw   = fast_atan2(2.0 * (quat.q0 * quat.q3 + quat.q1 * quat.q2), 1.0 - 2.0 * (quat.q2 * quat.q2 + quat.q3 * quat.q3)) * 180 / PI;
}

void get_euler_angles(double *yaw) {
    *yaw   = quat.yaw;
    if(*yaw<0){
        *yaw+=360;
    }
    *yaw=360-*yaw;

}
