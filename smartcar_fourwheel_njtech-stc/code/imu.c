#include "imu.h"
float gyro_x, gyro_y, gyro_z;
float imu_angle = 0;
float imu_pitch = 0;          // 俯仰角（检测前后翘起）
float avl_gyro_x, avl_gyro_y, avl_gyro_z;
float gyro_x_filtered, gyro_y_filtered, gyro_z_filtered;
float alpha = 0.9 ;

signed short imu_acc_X, imu_acc_Y, imu_acc_Z;
signed short imu_gyro_X, imu_gyro_Y, imu_gyro_Z;

// 翘起检测标志和阈值
uint8 car_tilt_flag = 0;   // 0:正常, 1:翘起
float tilt_threshold = 30; // 翘起角度阈值（度）


gyro_param_t GyroOffset;
void gyroOffset_init(void)
{

	 signed short acc_X, acc_Y, acc_Z;
   signed short gyro_X, gyro_Y, gyro_Z;
	 int i;

    GyroOffset.Xdata = 0.0;
    GyroOffset.Ydata = 0.0;
    GyroOffset.Zdata = 0.0;
    for (i = 0; i < 100; ++i) {
    imu660rb_get_gyro();
    GyroOffset.Xdata += imu660rb_gyro_x;
    GyroOffset.Ydata += imu660rb_gyro_y;
    GyroOffset.Zdata += imu660rb_gyro_z;
    system_delay_ms(10);
    }
    GyroOffset.Xdata /= 100.0;
    GyroOffset.Ydata /= 100.0;
    GyroOffset.Zdata /= 100.0;
}

float acc_transition (int16 acc_value)
{
    float acc_data = 0;
    acc_data = (float)acc_value / 16393;    // 0x30 加速度量程为:±2G      获取到的加速度计数据 除以 16393 ，可以转化为带物理单位的数据，单位：g(m/s^2)
    return acc_data;
}

float gyro_transition (int16 gyro_value)
{
    float gyro_data = 0;
		gyro_data = (float)gyro_value / 14.3f; // 0x5C 陀螺仪量程为:±2000dps 获取到的陀螺仪数据除以 14.3，    可以转化为带物理单位的数据，单位为：°/s
    return gyro_data;
}

void gyro_get()      //角速度
{
	imu660rb_get_gyro(); 
	imu_gyro_X  = imu660rb_gyro_x - GyroOffset.Xdata;
	imu_gyro_Y = imu660rb_gyro_y - GyroOffset.Ydata;
	imu_gyro_Z = imu660rb_gyro_z - GyroOffset.Zdata;
	
	// 获取X轴角速度（用于计算俯仰角）
	gyro_x = imu_gyro_X;
	gyro_x = gyro_transition(imu_gyro_X);
	avl_gyro_x = gyro_x ;	
	gyro_x_filtered = alpha * gyro_x_filtered + (1 - alpha) * avl_gyro_x;
	
	// 获取Y轴角速度（备用）
	gyro_y = imu_gyro_Y;
	gyro_y = gyro_transition(imu_gyro_Y);
	avl_gyro_y = gyro_y ;		
	gyro_y_filtered = alpha * gyro_y_filtered + (1 - alpha) * avl_gyro_y;

	// 获取Z轴角速度（用于计算偏航角）
	gyro_z = imu_gyro_Z;
	gyro_z = gyro_transition(imu_gyro_Z);	
	avl_gyro_z = gyro_z ;
	gyro_z_filtered = alpha * gyro_z_filtered + (1 - alpha) * avl_gyro_z;
}

void angle_get()     //角度获取
{
	// 计算偏航角（Z轴）
	imu_angle += 0.002 * gyro_z_filtered;
	
}

void angle_clear()   //角度清零
{
	imu_angle = 0;	

}


 