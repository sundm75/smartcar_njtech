
#include "zf_common_headfile.h"


void pit_handler (void)
{
	static uint32 count = 0;
    key_Handle();
}

void main(void)
{
    clock_init(SYSTEM_CLOCK_96M); 				// 时钟配置及系统初始化<务必保留>
    debug_init();                       		// 调试串口信息初始化
	  
    // 此处编写用户代码 例如外设初始化代码等
    ips200_init();
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
    // 此处编写用户代码 例如外设初始化代码等
    key_init();
		led_init();
		SMARTCAR_Init();
		
		pit_ms_init(TIM0_PIT, 5, pit_handler);         // 初始化 PIT_CH0 为周期中断 5ms 周期		
    interrupt_set_priority(TIMER0_IRQn, 0);        // 设置 PIT1 对周期中断的中断优先级为 0，0为最低优先级
		
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

   
        }
        // 此处编写需要循环执行的代码
    }
}
