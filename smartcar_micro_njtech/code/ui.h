/*
 * ui.h
 *
 *  Created on: 2023年1月7日
 *      Author: sundm
 */

#ifndef CODE_UI_H_
#define CODE_UI_H_
#include "zf_common_headfile.h"

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

extern SMARTCAR_PARAM smartcar_param ;

void SMARTCAR_Init(void);

typedef struct MenuItems
{
    float step_level;
    float max_value;
    float min_value;
    uint8 *title;
} MenuItem;

int get_param_index(void);
void display_Handle(void);



#endif /* CODE_UI_H_ */
