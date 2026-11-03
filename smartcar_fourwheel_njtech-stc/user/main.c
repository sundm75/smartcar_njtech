
#include "zf_common_headfile.h"

void test_steer(void);
void test_motor();
void test_encoder(void);

void pit_handler (void)
{
	static uint32 count = 0;
	beep_handle();
	
  if(count%4 == 0 )	  //20ms运行
  {	
		key_Handle(); //按键定时中断服务 20ms
  }
	
   //5ms运行
  {

	  /*传感器获得车辆和路面信息 */
	  update_speed_and_distance();
		/*分析路面*/

		
		/*控制车辆 */

	}

	if(count++ >= 200) //1s运行
	{
			count = 0;	
			led_toggle();		
	}
}

void pit_handler1 (void) //2ms
{
	static uint32 count = 0;
	
	gyro_get();   // 2ms 更新  
	angle_get();
	
	if(count++ >= 500) //1s运行
	{
			count = 0;	
			led_toggle();		
	}
}

void main(void)
{
    clock_init(SYSTEM_CLOCK_96M); 				// 时钟配置及系统初始化<务必保留>
    debug_init();                       		// 调试串口信息初始化
	  
    // 此处编写用户代码 例如外设初始化代码等
	  ips200_init();
		SMARTCAR_Init();
    key_init();
		led_init();   
	
		beep_init();
		beep(2, 500, 50);//响2声 周期500ms 响声占50%

		encoder_init();
		steer_init();
		motor_init();	
	
		while(1)
		{
        if (imu660rb_init())
            ips200_show_string(0, 0, "mu660RB reinit.");                               // IMU660RA 初始化失败
					else
							break;
		}
		gyroOffset_init();	
	
    ips200_show_string(0, 0, "mt9v03x init.");
    while(1)
    {
        system_delay_ms(100);   
        if(mt9v03x_init())
            ips200_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
    }
    ips200_show_string(0, 16, "init success.");
	/*=========================以下是每个的外设的测试 都是死循环 不用时要删除====================*/
	test_steer();
	test_motor();
  test_encoder();

		/*==========================每个的外设的测试 结束==========================*/
		
		pit_ms_init(TIM0_PIT, 5, pit_handler);         // 初始化 PIT_CH0 为周期中断 5ms 周期		
    interrupt_set_priority(TIMER0_IRQn, 0);        // 设置 PIT1 对周期中断的中断优先级为 0，0为最低优先级

		pit_ms_init(TIM1_PIT, 2, pit_handler1);         // 初始化 TIM0_PIT  为周期中断 2ms 周期
		interrupt_set_priority(TIMER1_IRQn, 1);        // 设置 TIMER1 对周期中断的中断优先级为 0，0为最低优先级		
		
		system_delay_ms(100);  
		printf("Init OK \r\n " );    
		
    while(1)
    {
        // 此处编写需要循环执行的代码
			  display_Handle(); 
			
        if(mt9v03x_finish_flag)
        {
            ips200_displayimage03x(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
            mt9v03x_finish_flag = 0;
					
					/*跑车时的图像处理 */
					get_reference_point(mt9v03x_image[0]);
					Search_reference_col(mt9v03x_image[0]);
					Search_line(mt9v03x_image[0]);
	
					Fitted_Midline();//求中线 
					Error_sum();//求中线合成偏差 err_sum  转向用
   
        }
        // 此处编写需要循环执行的代码
    }
}

/*============================测试舵机电机 ============================*/

void test_steer(void) // 
{
    uint8 channel_index = 0;
    uint16 duty = STEER_R_MIN;

   while(1)
    {

        //测试 duty 设置
        steer_duty(duty);// 这里调试找到中点 

        duty += 20;
        if (duty > STEER_L_MAX)
            duty = STEER_R_MIN;
        printf("duty= %d.\r\n", duty);
        ips200_show_int32(0, 16 * 1, (int16) duty, 5);
        system_delay_ms(500);
				
    }
}

void test_motor()
{
	int32 duty = 0;
	int8 dir = 1;

    while(1)
    {
        motor_duty(duty*100, duty*100);

        if(dir)                                                                 // 根据方向判断计数方向 本例程仅作参考
        {
            duty ++;                                                            // 正向计数
            if(duty >= 15)                                                // 达到最大值
            dir = 0;                                                            // 变更计数方向
        }
        else
        {
            duty --;                                                            // 反向计数
            if(duty <= -15)                                               // 达到最小值
            dir = 1;                                                            // 变更计数方向
        }
				printf(	"duty= %d.\r\n",duty);   
				ips200_show_int32 (0, 16*1, duty,3);
        system_delay_ms(800);
    }
}

void test_encoder(void) //10 周期调用update_speed_and_distance 更新速度与距离
{
   while(1)
    {
        update_speed_and_distance();
				printf(	"dis= %.0f v_left= %.0f v_right= %.0f\r\n",get_distance(), get_left_speed(), get_right_speed()); 
				ips200_show_float(0,16*0 ,get_distance(),2,3);  
				ips200_show_float(0,16*1 ,get_left_speed(),2,3);  
				ips200_show_float(0,16*2 ,get_right_speed(),2,3);  
			  ips200_show_int32(0,16*4 ,get_left_encoder(),3);  
			  ips200_show_int32(0,16*5 ,get_right_encoder(),3);  
        system_delay_ms(10);
				
    }
}

