
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"


#define IPS200_TYPE     (IPS200_TYPE_SPI)                                       //  SPI 两寸屏 这里宏定义填写 IPS200_TYPE_SPI（第二排丝印是NC）

void test_steer (void);
void test_motor ();
void test_encoder (void);                                                                                //  并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8 （第二排丝印是D0 - D7）

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    ips200_init(IPS200_TYPE);
    ips200_show_string(0, 0, "mt9v03x init.");
    ips200_show_string(0, 16, "init success.");

    SMARTCAR_Init();
    led_init();
    usr_key_init();

    beep_init();
    motor_init();
    encoder_init();
    //steer_init();

    /*imu660RA初始化*/
    while (1)
    {
        if (imu660ra_init())
            ips200_show_string(0, 0, "mu660RA reinit.");                               // IMU660RA 初始化失败
        else
            break;
        led_toggle(LED_1);
        system_delay_ms(1000);                                                  // 闪灯表示异常
    }
    quaternion_init();
    gyroOffset_init();

    //摄像头初始化
    while(1)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 80, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }

    /*=========================以下是每个的外设的测试 都是死循环 不用时要删除====================*/
      test_steer();
      test_motor();
      test_encoder();
    /*==========================每个的外设的测试 结束==========================*/

    pit_ms_init(CCU61_CH0, 10);//10ms key_scan
    pit_ms_init(CCU60_CH0, 5);//5ms encoder motor
    pit_ms_init(CCU60_CH1, 1);//1ms imu
    pit_ms_init(CCU61_CH1, 1);//1ms beep


    printf("init ok\r\n");
    beep(2, 500, 50);

    image_init();
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

        display_Handle();

        extern uint8 bin_buf[LM_IMAGE_H][LM_IMAGE_W];
        ips200_show_gray_image(0, 120, (const uint8 *)gray_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);




        // 此处编写需要循环执行的代码
    }
}


void test_steer (void) //steer_angle(30~150, 0~2)  steer_duty(1000~2000, 0~2)
{
    uint8 channel_index = 0;
    uint16 duty = STEER_R_MIN;

    while (1)
    {

        //测试 duty 设置
        steer_duty(duty);// 这里调试找到中点 1500
        duty += 20;
        if (duty > STEER_L_MAX)
            duty = STEER_R_MIN;
        printf("duty= %d.\r\n", duty);
        ips200_show_int(0, 16 * 1, (int16) duty, 5);
        system_delay_ms(1000);

    }
}

void test_motor ()
{
    int8 duty = 0;
    int8 dir = 1;

    while (1)
    {
        motor_duty(duty * 100, duty * 100);
        if (dir)                                                                 // 根据方向判断计数方向 本例程仅作参考
        {
            duty++;                                                            // 正向计数
            if (duty >= 20)                                                // 达到最大值
                dir = 0;                                                            // 变更计数方向
        }
        else
        {
            duty--;                                                            // 反向计数
            if (duty <= -20)                                               // 达到最小值
                dir = 1;                                                            // 变更计数方向
        }
        printf("duty= %d.\r\n", duty);
        ips200_show_int(0, 16 * 1, duty, 5);
        system_delay_ms(900);
    }
}

void test_encoder (void) //10 周期调用update_speed_and_distance 更新速度与距离
{
    while (1)
    {
        update_speed_and_distance();
        printf("dis= %.0f v_left= %.0f v_right= %.0f\r\n", get_distance(), get_left_speed(), get_right_speed());
        ips200_show_float(0, 16 * 0, get_distance(), 2, 3);
        ips200_show_float(0, 16 * 1, get_left_speed(), 2, 3);
        ips200_show_float(0, 16 * 2, get_right_speed(), 2, 3);
        ips200_show_int(0, 16 * 4, get_left_encoder(), 3);
        ips200_show_int(0, 16 * 5, get_right_encoder(), 3);
        system_delay_ms(10);

    }
}

#pragma section all restore

