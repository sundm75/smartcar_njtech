/*
 * ui.h
 *
 *  Created on: 2026年5月7日
 *      Author: sundm
 */

#ifndef CODE_UI_H_
#define CODE_UI_H_
#include "zf_common_headfile.h"

typedef struct MenuItems
{
    float step_level;
    float max_value;
    float min_value;
    uint8 *title;
} MenuItem;

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

extern SMARTCAR_PARAM smartcar_param ;

int get_param_index(void);
void display_Handle(void);
void key_process(void);

void SMARTCAR_Init(void);


#endif /* CODE_UI_H_ */