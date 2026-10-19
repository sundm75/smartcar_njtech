/*
 * ui.c
 *
 *  Created on: 2026年5月7日
 *    
 */
#include "ui.h"

typedef struct  SmartCar_Param										//智能车调整参数
{
    float bend_steer_kp;
    float bend_steer_kd;
    float motor_kp;
    float motor_ki;
    float motor_kd;
    float global_speed;
    float motor_kp2;
    float motor_kgyro;
    float circle_set_step; 
    float fan_speed;
	
    float circle_len;
    float circle_exit_dis;
    float circle_exit_angle;
    float cir_L_1;
    float cir_L_2;
    float cir_L_3;
	  float laser_flag;
    float circle_flag ;  
    float run_mode;
	  float steer_middle_value;
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
    smartcar_param.bend_steer_kp      = 16.0 ;
    smartcar_param.bend_steer_kd      = 30.0 ;
    smartcar_param.motor_kp           = 1.6;
    smartcar_param.motor_ki           = 0.02;
    smartcar_param.motor_kd           = 0.30;
    smartcar_param.global_speed       = 400.0;
    smartcar_param.motor_kp2   				= 0.02 ; /* 二次项 0.02*/
    smartcar_param.motor_kgyro    		= 0.11 ;/* 陀螺仪补偿  0.11*/ 
    smartcar_param.circle_set_step		= 0.0; //  circle_set_step
    smartcar_param.fan_speed	  			= 2600.0;
		
    smartcar_param.circle_len		  = 380.0;
    smartcar_param.circle_exit_dis    = 53.0;
    smartcar_param.circle_exit_angle  = 26.0 ;
    smartcar_param.cir_L_1  		  = 13000;
    smartcar_param.cir_L_2  		  = 150;
    smartcar_param.cir_L_3  		  = 80;
    smartcar_param.laser_flag    	=	1.0 ;   //laser_flag
    smartcar_param.circle_flag    = 1.0 ; //circle_flag
    smartcar_param.run_mode      = 1.0 ;
	  smartcar_param.steer_middle_value = 1550.0 ;
		
}

MenuItem items[ ] = {  //共20个参数 2屏  step ,  max , min, title
        {1.0,  20.0,   0.0, "bkp" },    // bend_steer_kp;  
        {5.0, 200.0,   0.0, "bkd" },    // bend_steer_kd;
        {0.1,  5.0,    0.0, "mkp" },    // motor_kp;
        {0.01,  5.0,     0.0, "mki" },    // motor_ki;
        {0.1,  10.0,    0.0, "mkd" },    // motor_kd;
        { 20, 500.0,    0.0, "gsd" },    // global_speed;
        {0.01,  2.0,    0.0, "kp2" },    // motor_kp2;
        {0.01,  2.0,    0.0, "mkg" },    // motor_kgyro;
        {  1, 10.0,     0.0, "css" },    // circle_set_step;
        {100, 3000.0,  1900.0, "fsd" },    // fan_speed;    
				
        {  5, 500.0,    0.0, "cle" },    // circle_len;
        {  5, 100.0,    0.0, "cod" },    // circle_exit_dis;
        {  1,  40.0,  -40.0, "coa" },    // circle_exit_angle;
        {100,15000.0, 1000.0,"cL1 " },    // cir_L_1;  13000
        { 10,1000.0,  100.0, "cL2 " },    // cir_L_2;  500
        { 10,1000.0,   10.0, "cL3 " },    // cir_L_3;  100
        {  1,    1.0,    0.0, "lfg" },    // laser_flag
        {  1,    1.0,  0.0,   "cfg" },    //   circle_flag     
        {1,   6.0,     0.0, "mod" },    // run_mode ;
        {2,   900.0,   650.0, "mid" },    // steer_middle_value; 19  
};

/*=================================================================================================*/
/*  智能车综合按键处理显示*/
int param_index = 0;
void key_process(void)
{
    static int is_editing_flag = 0;
    unsigned char txt[32];
    unsigned char str[32];
    unsigned char i, page_index;
    MenuItem *p_param_item;
    float *p_param;
	  int key_value;

    p_param_item = items;
    p_param = (float *) &smartcar_param;
    page_index =    param_index/10;
	
    /*根据按键修改参数*/
    p_param_item = items;
    p_param = (float *) &smartcar_param;
	
  key_value = get_key_value();
	switch (key_value)
    {
    case 1: 
        if(is_editing_flag) 
        {
            *(p_param + param_index) -= (p_param_item + param_index)->step_level;
            if(*(p_param + param_index)<(p_param_item + param_index)->min_value)
                *(p_param + param_index) = (p_param_item + param_index)->max_value;
        }
        else
        {
            param_index --;
            if(param_index < 0) param_index = (sizeof(SMARTCAR_PARAM))/4 - 1;
        }
			  
        break;
		break;
		case 2: 
        if(is_editing_flag)
        {
            *(p_param + param_index) += (p_param_item + param_index)->step_level;
            if(*(p_param + param_index)>(p_param_item + param_index)->max_value)
                *(p_param + param_index) = (p_param_item + param_index)->min_value;
        }
        else
        {
            param_index ++;
            if(param_index >= (sizeof(SMARTCAR_PARAM))/4)       param_index = 0;
        }
        break;
		case 3:
    case 4:
			is_editing_flag = !is_editing_flag;

		break;
    default:
        break;
    }	
		
    for(i = 0; i < 10; i++)
    {
        if((i+page_index*10) == param_index)
        {
					     if(is_editing_flag)
                ips200_show_string(i%2*(120),120+(2+i/2)*16, "*");
							 else
								 ips200_show_string(i%2*(120),120+(2+i/2)*16, ">");
        }
        else
             ips200_show_string(i%2*(120),120+(2+i/2)*16, " ");

        if((i+10*page_index) < ((sizeof(SMARTCAR_PARAM))/4))
        {
            ips200_show_string(i%2*(120)+8,120+(2+i/2)*16, (p_param_item+(i+page_index*10))->title);
            if((p_param_item+(i+page_index*10))->step_level <= 0.001)
                sprintf(str,"% 1.4f ", *(p_param+(i+page_index*10)));
            else if((p_param_item+(i+page_index*10))->step_level <= 0.01)
                sprintf(str,"% 4.2f ", *(p_param+(i+page_index*10)));
            else if((p_param_item+(i+page_index*10))->step_level <= 0.1)
                sprintf(str,"% 3.1f", *(p_param+(i+page_index*10)));
            else
                sprintf(str,"% 6.0f", *(p_param+(i+page_index*10)));
            ips200_show_string(i%2*(120)+8*4,120+(2+i/2)*16, str);
				}
        else
        {
            ips200_show_string(i%2*(120),120+(2+i/2)*16, "       ");
            ips200_show_string(i%2*(120)+8*4,120+(2+i/2)*16, "       ");
        }
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