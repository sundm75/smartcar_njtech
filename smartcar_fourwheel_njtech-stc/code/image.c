#include "image.h"

// 图像备份数组，在发送前将图像备份再进行发送，这样可以避免图像出现撕裂的问题
uint8 left_control_line[SEARCH_IMAGE_H];	
uint8 right_control_line[SEARCH_IMAGE_H];


uint16 get_contrast(uint8 temp1,uint8 temp2)
{
    int16 diff = (int16)temp1 - (int16)temp2;
    return  (uint16)((diff < 0 ? -diff : diff) * 200 / (temp1 + temp2 + 1)); 
}


//！！！！！！！！！！！！两个uint8 是绝对不能乘在一起的要不然直接爆！！！！！！！！！！
//寻找参考点  图像底部遍历
uint8 reference_point;		//参考点
uint8 white_max_point;		//白点最大
uint8 white_min_point;		//白点最小
void get_reference_point(const uint8 *image)
{
	uint16 i;
	uint8 *p= image+(SEARCH_IMAGE_H-REFRENCE_ROW)*SEARCH_IMAGE_W;//指针指向遍历区域的头指针
	uint16 temp =0;//保存统计点的总数量
	uint32 tempSum=0;//保存所有统计点的和
	temp = REFRENCE_ROW * SEARCH_IMAGE_W;//计算统计点总数
	for(i=0;i<temp;i++){
		tempSum+= *(p+i);//统计点求和
	}
	reference_point = (uint8)(tempSum/temp);//计算点的平均值，作为图像的参考点
	//根据参考点找最大最小白点的值
	white_max_point = (uint8)func_limit_ab(reference_point*WHITE_MAX_MUL/10,BALACK_POINT,230);
	white_min_point = (uint8)func_limit_ab(reference_point*WHITE_MIN_MUL/10,BALACK_POINT,230);
}


uint8 top_edge_line[SEARCH_IMAGE_W]; //顶部边界
uint8 refenence_col_line[SEARCH_IMAGE_H];  	//图传
uint8 reference_contrast_ratio = 32;      	//对比度阈值
uint8 reference_col;						//参考列
void Search_reference_col(const uint8 *image)
{
	uint8 col,row,temp1,temp2,i, j=0;
	uint8 globe_remote=120,globe_remote_min=120; //临时全局变量每列最小,记录全局列最小
	uint16 contrast=0;
	reference_col = SEARCH_IMAGE_W / 2;

	//几列几列像右   从下向上
	for(col=0;col<SEARCH_IMAGE_W;col+=PIXEL_OFFSET)
	{
		top_edge_line[j] = 0;
		globe_remote=120;//还原
		for(row =SEARCH_IMAGE_H-1;row>PIXEL_OFFSET;row-=PIXEL_OFFSET)
		{
			temp1 =*(image + row*SEARCH_IMAGE_W+col);           	   //当前点灰度值 下点
			temp2 =*(image + (row-PIXEL_OFFSET) * SEARCH_IMAGE_W +col);//获取对比点灰度值 上点

			if(temp2 > white_max_point)//为白点直接下一次循环
			{
				continue;
			}
				
			else if(temp1 < white_min_point)//判定为黑点  进入上一行  此时row 为黑点行 
			{
				if(globe_remote>row)
					globe_remote=row;
				
				top_edge_line[j] = row; //记录顶部边界 ******************
				j++ ;
				
				break;
			}
			
			contrast = get_contrast(temp1,temp2);   //对比度就是差比和
			
			if(contrast > reference_contrast_ratio || row == STOP_ROW)//如果大于这个值,就判定为边界点
			{
				if(globe_remote>row)
					globe_remote=row;
			}

		}	
			//每一行结束再更新
			if(globe_remote<globe_remote_min)
			{
				globe_remote_min=globe_remote; //行
				reference_col= col;//找到最小值，就是最远端白色值  列
			}
				
	}
	
	//比赛记得关了  只是用来图传的
	for(i=0 ;i<SEARCH_IMAGE_H;i++)//赋值给数组来画出那一列
		refenence_col_line[i] = reference_col;
	
}



uint8 left_edge_line [SEARCH_IMAGE_H];		//左边线存储数组
uint8 right_edge_line[SEARCH_IMAGE_H];		//右边线存储数组
uint8 mid_line[SEARCH_IMAGE_H];				//中线数组
uint8 lost_left,lost_right;                 //左丢线和右丢线
void Search_line(const uint8 *image)
{
	const uint8 *p = image;		   		   				//图像指针
	uint8 row_max = SEARCH_IMAGE_H -1;					//行最大值
	uint8 row_min = STOP_ROW;
	uint8 col_max = SEARCH_IMAGE_W - 1;
	uint8 col_min = 0;
	
	uint8 left_start_col  = reference_col;         		//搜线左起始列
	uint8 right_start_col = reference_col+10;          	//搜线右起始列	！！！要改！！！
	uint8 left_end_col   = col_min;         			//搜线左终止列
	uint8 right_end_col  = SEARCH_IMAGE_W-1;        	//搜线右终止列	

	uint8 search_time;									//单点搜线次数(不行了可以搜索两次)
	uint8 temp1,temp2;								//存储图像数据
	int16 contrast;									//对比度
	
	uint8 left_stop=0,right_stop=0,stop_point=0;			//搜线自锁变量
	uint8 col,row;
	
	lost_left=STOP_ROW;
	lost_right=STOP_ROW;//丢线默认是不丢线
	for(row = row_max;row >= row_min;row -- )
	{
		left_edge_line[row]  = 0;
		right_edge_line[row] = SEARCH_IMAGE_W-1;
	}
	//o_O("1.for");
	
	//对每一行进行遍历，从最底部开始
	for(row = row_max;  row >= row_min; row-=PIXEL_OFFSET)
	{
		p=image + row * SEARCH_IMAGE_W;//获取本行起始指针
		if(!left_stop)//左边搜线
		{
			search_time =2;
			do
			{
				if(search_time == 1)
				{
					left_start_col  = reference_col;
					left_end_col    = col_min;
				}
				search_time--;
				//从参考列向左边界搜索
				for(col = left_start_col;col > left_end_col+PIXEL_OFFSET;col-=PIXEL_OFFSET)
				{
					temp1 = *( p + col );				//当前点的灰度(右)
					temp2 = *( p + col-PIXEL_OFFSET);	//向左下一个的灰度

					//如果是参考列点小于黑点阈值,直接终止左边界的寻找后面的点都赋值为左边界
					if(temp1 < white_min_point && col == left_start_col && left_start_col == reference_col)
					{
						left_stop =1;
						search_time=0;//(杀死比赛)
						//往上的都直接赋值成边界
						for(stop_point = row;stop_point>1;stop_point--)
						{
							left_edge_line[stop_point] = col_min;//左边界
						}
						lost_left=row;	//丢线赋值
						break;
					}
					//黑色直接赋值+跳出循环
					if(temp1 < white_min_point)
					{						
						left_edge_line[row]=col;						
						break;
					}
					//白色,直接continue
					if(temp2 > white_max_point)
					{
						continue;
					}
					//计算对比度
					contrast = get_contrast(temp1,temp2);   //对比度就是差比和
					
					if(contrast > reference_contrast_ratio || col == col_min)
					{
						left_edge_line[row]=col;
						left_start_col = (uint8)func_limit_ab(col+SEARCH_RANGE,col,col_max);
						left_end_col   = (uint8)func_limit_ab(col-SEARCH_RANGE,col_min,col);//更新起始和终止
						search_time =0;
						break;
					}
				}
			}while(search_time);
		}
		
		if(!right_stop)//直接和左边对称
		{
			search_time =2;
			do
			{
				if(search_time == 1)
				{
					right_start_col  = reference_col;
					right_end_col    = col_max;
				}
				search_time--;
				//从参考点向左边界搜索
				for(col = right_start_col;col <=right_end_col-PIXEL_OFFSET;col+=PIXEL_OFFSET)
				{
					temp1 = *( p + col );				//当前点的灰度
					temp2 = *( p + col + PIXEL_OFFSET);	//向右下一个的灰度(右)

					//如果是参考列点小于黑点阈值,直接终止左边界的寻找后面的点都赋值为左边界
					if(temp1 < white_min_point && col == right_start_col && right_start_col == reference_col)
					{
						right_stop =1;
						search_time=0;//(杀死比赛)
						//往上的都直接赋值成边界
						for(stop_point=row;stop_point>row_min;stop_point--)
						{
							right_edge_line[stop_point] = SEARCH_IMAGE_W-1;//左边界
						}
						lost_right=row;	//丢线赋值
						break;
					}
					//黑色直接赋值+跳出循环
					if(temp1 < white_min_point)
					{

						right_edge_line[row]=col;
						break;
					}
					//白色,直接continue
					if(temp2 > white_max_point)
					{
						continue;
					}
					//计算对比度
					contrast = get_contrast(temp1,temp2);
					if(contrast > reference_contrast_ratio || col >= col_max - PIXEL_OFFSET)
					{
						
						right_edge_line[row]=col;
						right_start_col = (uint8)func_limit_ab(col-SEARCH_RANGE,col_min,col);
						right_end_col   = (uint8)func_limit_ab(col+SEARCH_RANGE,col,col_max);//更新起始和终止
						search_time =0;
						break;
					}
				}
			}while(search_time);
		}		
	}
	
	//收完之后插值
	insert_val();
	//复制数组
	memcpy(left_control_line, left_edge_line, sizeof(left_edge_line)); 
	memcpy(right_control_line,right_edge_line, sizeof(right_edge_line)); 
}

//插值函数
void insert_val(void)
{
	uint8 i,j;
	uint8 front_p,cur_p;//存找到的点
	uint8 step;
	//左边插值
	front_p=left_edge_line[SEARCH_IMAGE_H-1];
	for(i = SEARCH_IMAGE_H-PIXEL_OFFSET-1;i>= STOP_ROW;i-=PIXEL_OFFSET){
		cur_p=left_edge_line[i];
		if(cur_p >= front_p){
			step=(cur_p-front_p+PIXEL_OFFSET-1)/PIXEL_OFFSET;//向上取整
			for(j=1;j<PIXEL_OFFSET;j++){
				left_edge_line[i-j+PIXEL_OFFSET]=front_p+step*j;
			}				
		}
		else{
			step=(front_p-cur_p+PIXEL_OFFSET-1)/PIXEL_OFFSET;
			for(j=1;j<PIXEL_OFFSET;j++){
				left_edge_line[i-j+PIXEL_OFFSET]=front_p-step*j;
			}
		}
		front_p=cur_p;//更新值
	}
	
	//右边插值
	front_p=right_edge_line[SEARCH_IMAGE_H-1];
	for(i = SEARCH_IMAGE_H-PIXEL_OFFSET-1;i>= STOP_ROW;i-=PIXEL_OFFSET){
		cur_p=right_edge_line[i];
		if(cur_p >= front_p){
			step=(cur_p-front_p+PIXEL_OFFSET-1)/PIXEL_OFFSET;
			for(j=1;j<PIXEL_OFFSET;j++){
				right_edge_line[i-j+PIXEL_OFFSET]=front_p+j*step;
			}				
		}
		else{
			step=(front_p-cur_p+PIXEL_OFFSET-1)/PIXEL_OFFSET;
			for(j=1;j<PIXEL_OFFSET;j++){
				right_edge_line[i-j+PIXEL_OFFSET]=front_p-j*step;
			}
		}
		front_p=cur_p;
	}
}

//合并中线
void Fitted_Midline(void)
{
	uint8 i;
	for(i=0;i<119;i++){
		mid_line[i]=(left_control_line[i]+right_control_line[i])>>1;
	}
}

int32 err_sum=0;
void Error_sum(void)
{
	uint8 i;
    int32 err1;
    int32 err2;
    int32 err3;

	for(i=8;i<28;i++){
		err1+=mid_line[i];
	}
	err1/=20;
	for(i=28;i<89;i++){
		err2+=mid_line[i];
	}
	err2/=60;
	for(i=89;i<111;i++){
		err3+=mid_line[i];
	}
	err3/=30;
	
	// 2:5:3
    err_sum = (err1 * 20 + err2 * 50 + err3 * 30) / 100;
}



uint8 road_width[SEARCH_IMAGE_H] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
32, 32, 34, 36, 38, 40, 42, 44, 46, 48, 
50, 52, 54, 56, 57, 58, 59, 60, 62, 64, 
66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 
86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 
106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 
126, 128, 130, 132, 134, 136, 138, 140, 141, 142, 
143, 144, 146, 148, 150, 152, 154, 156, 158, 160, 
162, 165, 168, 171, 172, 173, 174, 175, 176, 177, 
178, 179, 181, 183, 185, 187, 187, 187, 187, 187, 
187, 187, 187, 187, 187, 187, 187, 187, 187, 187, 
187, 187, 187, 187, 187, 187, 187, 187, 187, 187, 
};

void calculate_road_width()
{
	unsigned char str[32];
	int i;
	printf("road_width:\r\n");
	for(i = 0; i < SEARCH_IMAGE_H; i++)
	{
		if(i % 10 == 0) {
			sprintf(str,"\r\n", i / 10); printf(str);
		}
		sprintf(str, "%d, ", -left_edge_line[i] + right_edge_line[i]); printf(str);
	}
	 printf("\r\n end  \r\n");
				
}

