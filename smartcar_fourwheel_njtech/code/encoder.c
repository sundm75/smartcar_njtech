
#include "encoder.h"
#include "zf_driver_encoder.h"

#define ENCODER_2                   (TIM2_ENCODER)
#define ENCODER_2_A                 (TIM2_ENCODER_CH1_P33_7)
#define ENCODER_2_B                 (TIM2_ENCODER_CH2_P33_6)

#define ENCODER_4                   (TIM5_ENCODER)
#define ENCODER_4_A                 (TIM5_ENCODER_CH1_P10_3)
#define ENCODER_4_B                 (TIM5_ENCODER_CH2_P10_1)

int16 encoder1=0;
int16 encoder2=0;

int32 dis1=0;
int32 dis2=0;
float distance1;
float distance2;
float distance;  //m

float speed1, speed2;

void encoder_init(void)
{
    encoder_dir_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);
    encoder_dir_init(ENCODER_4, ENCODER_4_A, ENCODER_4_B);
}

void encoder_handler (void)
{
    encoder1 = -encoder_get_count(ENCODER_2);;
    encoder_clear_count(ENCODER_2);
    encoder2 = encoder_get_count(ENCODER_4);
    encoder_clear_count(ENCODER_4);

    dis1 += (int32)encoder1;
    dis2 += (int32)encoder2;

}

void clear_distance(void)
{
 dis1=0;
 dis2=0;
 distance1=0;
 distance2=0;
}

void update_speed_and_distance(void)
{

    float left_speed, right_speed;
    encoder_handler();

    distance1 = (float)(dis1 * ENCODER_DISTANCE_K) ;
    distance2 = (float)(dis2 * ENCODER_DISTANCE_K) ;

    left_speed = (float)(encoder1 * ENCODER_SPEED_K) ;
    right_speed = (float)(encoder2 * ENCODER_SPEED_K) ;

    speed1 =  left_speed;
    speed2 =  right_speed;

    distance = (distance1 + distance2 ) / 2 ;
}

int get_left_encoder(void)
{
    return encoder1;
}
int get_right_encoder(void)
{
    return encoder2;
}

float get_left_speed(void)
{
    return speed1;
}
 float get_right_speed(void)
{
    return speed2;
}
 float get_distance(void)
{
    return distance/10;
}

 float get_speed(void)
{
    return (speed1+ speed2) / 2;
}

