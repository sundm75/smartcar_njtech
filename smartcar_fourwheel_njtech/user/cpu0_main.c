
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"


#define IPS200_TYPE     (IPS200_TYPE_SPI)                                       //  SPI 两寸屏 这里宏定义填写 IPS200_TYPE_SPI（第二排丝印是NC）
                                                                                //  并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8 （第二排丝印是D0 - D7）

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    ips200_init(IPS200_TYPE);
    ips200_show_string(0, 0, "mt9v03x init.");
    while(1)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 80, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }
    ips200_show_string(0, 16, "init success.");

    SMARTCAR_Init();
    led_init();
    usr_key_init();
    pit_ms_init(CCU61_CH0, 10);//10ms key_scan

    printf("init ok\r\n");
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        display_Handle();

        if(mt9v03x_finish_flag)
        {
            ips200_displayimage03x((const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H);                       // 显示原始图像
//            ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 240, 180, 64);     // 显示二值化图像
            mt9v03x_finish_flag = 0;
        }


        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************
// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：屏幕不显示
//      如果使用主板测试，主板必须要用电池供电 检查屏幕供电引脚电压
//      检查屏幕是不是插错位置了 检查引脚对应关系
//      如果对应引脚都正确 检查一下是否有引脚波形不对 需要有示波器
//      无法完成波形测试则复制一个GPIO例程将屏幕所有IO初始化为GPIO翻转电平 看看是否受控
// 问题2：显示 reinit 字样
//      检查接线是否正常
//      主板供电是否使用电量充足的电池供电
// 问题2：显示图像杂乱 错位
//      检查摄像头信号线是否有松动
