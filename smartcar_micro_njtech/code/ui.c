/*
 * ui.c
 *
 *  Created on: 2023年11月7日
 *      Author: sundm
 */
#include "ui.h"

typedef struct  SmartCar_Param                                      //智能车调整参数
{
        float bend_steer_kp;
        float bend_steer_kd;
        float motor_kp;
        float motor_ki;
        float motor_kd;
        float global_speed;
        float circle_pid_ratio;
        float circle_in_angle;
        float circle_ready_in_dis;
        float circle_in_dis;
        float circle_len;
        float circle_exit_dis;
        float circle_exit_angle;
        float cir_L_1;
        float cir_L_2;
        float cir_L_3;
        float cir_dectect;
        float cir_flg ;
        float run_mode ;
        float steer_middle_value;
        uint16 flash_flag;
} SMARTCAR_PARAM;
SMARTCAR_PARAM smartcar_param ;


/* ----------------------------------------参数初始化---------------------------------------------- */
/**
  * @brief  智能车参数初始化
  * @note
  * @param  None
  * @retval None
  */
void SMARTCAR_Init(void)
{
    float *p_param;
    int i;
    p_param = (float *) &smartcar_param;
    for(i = 0; i < sizeof(SMARTCAR_PARAM)/4; i++)
    {
        *p_param++ = 0;
    }

    smartcar_param.bend_steer_kp      = 18 ;
    smartcar_param.bend_steer_kd      = 30 ;
    smartcar_param.motor_kp           = 1.6;
    smartcar_param.motor_ki           = 0.02;
    smartcar_param.motor_kd           = 0.30;
    smartcar_param.global_speed       = 500.0;
    smartcar_param.circle_pid_ratio   = 1.0 ;
    smartcar_param.circle_in_angle    = 25.0 ;
    smartcar_param.circle_ready_in_dis= 60.0; //
    smartcar_param.circle_in_dis      = 50.0;
    smartcar_param.circle_len         = 380.0;
    smartcar_param.circle_exit_dis    = 100.0;
    smartcar_param.circle_exit_angle  = 4.0 ;
    smartcar_param.cir_L_1            = 13000;
    smartcar_param.cir_L_2            = 80;
    smartcar_param.cir_L_3            = 80;
    smartcar_param.cir_dectect        = 0.0 ;
    smartcar_param.cir_flg       =  0.0 ;
    smartcar_param.run_mode     = 0.0;
    smartcar_param.steer_middle_value = 1450 ;

}

MenuItem items[ ] = {  //共22个参数 3屏  step ,  max , min, title
        {  1, 500.0,    0.0, "bend_kp" },    // bend_steer_kp;
        {  1, 500.0,    0.0, "bend_kd" },    // bend_steer_kd;
        {0.1, 100.0,    0.0, "moto_kp" },    // motor_kp;
        {0.01, 2.0,     0.0, "moto_ki" },    // motor_ki;
        {0.1,  20.0,    0.0, "moto_kd" },    // motor_kd;
        { 50,1000.0,    0.0, "glb_spd" },    // global_speed;
        {0.1,  10.0,    0.0, "cir_pid" },    // circle_pid_ratio;
        {  1,  40.0,  -40.0, "cirinag" },    // circle_in_angle;
        {  5, 100.0,   10.0, "cir_r_d" },    // circle_ready_in_dis;
        {  5, 100.0,    0.0, "cir_i_d" },    // circle_in_dis;

        {  5, 500.0,    0.0, "cir_len" },    // circle_len;
        {  5, 100.0,    0.0, "cir_o_d" },    // circle_exit_dis;
        {  1,  40.0,  -40.0, "cirouta" },    // circle_exit_angle;
        {100,15000.0, 1000.0,"cir_L1 " },    // cir_L_1;  13000
        { 10,1500.0,  1000.0, "cir_L2 " },    // cir_L_2;  500
        { 20, 1200.0,   1000.0, "cir_L3 " },    // cir_L_3;  1000-1500
        {1,   3.0,     0.0, "CirDect" },    // cir_dectect  10 停止 01 圆环
        {1,   5.0,     0.0, "cir_flg" },    // cir_flg ;
        {1,   3.0,     0.0, "run_mod" },    // run_mode ;
        {1,    2000.0,  1000.0, "S_middl" },    // steer_middle_value;
};

/*=================================================================================================*/
/*  智能车综合按键处理显示*/
int param_index = 0;
void key_process(void)
{
    unsigned char str[32];
    int i, page_index;
    MenuItem *p_param_item;
    float *p_param;

    p_param_item = items;
    p_param = (float *) &smartcar_param;
    page_index =    param_index/10;

    /*主屏获取显示按键值并修改 param_index*/
    static int is_editing_flag = 0;
    int key_value;
    key_value = get_key_value();

    /*第2~6行 主屏显示参数*/
    for(i = 0; i < 10; i++)
    {
        if((i+page_index*10) == param_index)
        {
            if(is_editing_flag)
                ips200_show_string(i%2*(120),(2+i/2)*16, "*");
            else
                ips200_show_string(i%2*(120),(2+i/2)*16, ">");
        }
        else
            ips200_show_string(i%2*(120),(2+i/2)*16, " ");


        if((i+10*page_index) < ((sizeof(SMARTCAR_PARAM)-1)/4))
        {
            ips200_show_string(i%2*(120)+8,(2+i/2)*16, (p_param_item+(i+page_index*10))->title);
            if((p_param_item+(i+page_index*10))->step_level <= 0.001)
                sprintf(str,"%0.4f ", *(p_param+(i+page_index*10)));
            else if((p_param_item+(i+page_index*10))->step_level == 0.01)
                sprintf(str,"%0.2f ", *(p_param+(i+page_index*10)));
            else if((p_param_item+(i+page_index*10))->step_level == 0.1)
                sprintf(str,"%3.1f", *(p_param+(i+page_index*10)));
            else
                sprintf(str,"%6.0f", *(p_param+(i+page_index*10)));
            ips200_show_string(i%2*(120)+8*9,(2+i/2)*16, str);
        }
        else
        {
            ips200_show_string(i%2*(120)+8,(2+i/2)*16, "       ");
            ips200_show_string(i%2*(120)+8*9,(2+i/2)*16, "       ");
        }
    }
    /*根据按键修改参数*/
    p_param_item = items;
    p_param = (float *) &smartcar_param;
    switch (key_value)
    {
    case 1: //值 + step
        if(is_editing_flag)
        {
            *(p_param + param_index) += (p_param_item + param_index)->step_level;
            if(*(p_param + param_index)>(p_param_item + param_index)->max_value)
                *(p_param + param_index) = (p_param_item + param_index)->min_value;
        }
        else
        {
            param_index --;
            if(param_index < 0) param_index = (sizeof(SMARTCAR_PARAM)-1)/4 - 1;
        }
        break;
    case 2://值 - step
        if(is_editing_flag)
        {
            *(p_param + param_index) -= (p_param_item + param_index)->step_level;
            if(*(p_param + param_index)<(p_param_item + param_index)->min_value)
                *(p_param + param_index) = (p_param_item + param_index)->max_value;
        }
        else
        {
            param_index ++;
            if(param_index >= (sizeof(SMARTCAR_PARAM)-1)/4)       param_index = 0;
        }
        break;
    case  3:

        break;
    case  4:
        is_editing_flag = !is_editing_flag;
        break;

    default:
        break;
    }
}
int get_param_index(void)
{
    return param_index;
}
/*=================================================================================================*/
/*  智能车显示*/
void display_Handle(void)
{

    key_process();


}


