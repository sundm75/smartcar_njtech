#ifndef __IMAGE_H_
#define __IMAGE_H_
#include "zf_common_headfile.h"
#define SEARCH_IMAGE_W  	188  //搜线宽度  
#define SEARCH_IMAGE_H  	120  //搜线高度
#define BALACK_POINT     	100  //黑点值
#define WHITE_MAX_MUL        13  //白点最大值
#define WHITE_MIN_MUL      	  7  //白点最小值

#define REFRENCE_ROW    	  4   //参考行数
#define SEARCH_RANGE    	  16  //搜索半径
#define STOP_ROW         	  8   //搜线停止行
#define PIXEL_OFFSET          4   //偏移量

extern uint8 reference_point;		//参考点
extern uint8 white_max_point;		//白点最大
extern uint8 white_min_point;		//白点最小

extern uint8 refenence_col_line[SEARCH_IMAGE_H]; 	//图传
extern uint8 reference_contrast_ratio;         		//对比度阈值
extern uint8 reference_col;					     	//参考列
extern uint8 left_edge_line[SEARCH_IMAGE_H];		//左边线存储数组
extern uint8 right_edge_line[SEARCH_IMAGE_H];		//右边线存储数组
extern uint8 mid_line[SEARCH_IMAGE_H];				//中线数组
extern uint8 left_control_line[SEARCH_IMAGE_H];		//左操作数组
extern uint8 right_control_line[SEARCH_IMAGE_H];	//右操作数组
extern uint8 lost_left,lost_right;
extern uint8 lost_left_num,lost_right_num;//左 右丢线数量
extern uint8 road_width[SEARCH_IMAGE_H]; //道路宽度
extern uint8 top_edge_line[SEARCH_IMAGE_W]; //顶部边界

extern int32 err_sum; 

//对比度搜线算法
void get_reference_point(const uint8 *image);
void Search_reference_col(const uint8 *image);
void Search_line(const uint8 *image);
void Fitted_Midline(void);
void Cross_analysis(void);
void Connect_point(void);
void insert_val(void);//插值函数

void straightAccelerate(void);
void Error_sum(void);

void calculate_road_width();

#endif


