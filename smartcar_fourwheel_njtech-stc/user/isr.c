/*********************************************************************************************************************
 * STC32G144K Opensourec Library 即（STC32G144K 开源库）是一个基于官方 SDK 接口的第三方开源库
 * Copyright (c) 2025 SEEKFREE 逐飞科技
 *
 * 本文件是STC32G144K开源库的一部分
 *
 * STC32G144K 开源库 是免费软件
 * 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
 * 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
 *
 * 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
 * 甚至没有隐含的适销性或适合特定用途的保证
 * 更多细节请参见 GPL
 *
 * 您应该在收到本开源库的同时收到一份 GPL 的副本
 * 如果没有，请参阅<https://www.gnu.org/licenses/>
 *
 * 额外注明：
 * 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
 * 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
 * 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
 * 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
 *
 * 文件名称
 * 公司名称          成都逐飞科技有限公司
 * 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
 * 开发环境          MDK FOR C251
 * 适用平台          STC32G144K
 * 店铺链接          https://seekfree.taobao.com/
 *
 * 修改记录
 * 日期              作者           备注
 * 2025-11-20         大W            first version
 ********************************************************************************************************************/

#include "zf_common_headfile.h"


void INT0_IRQHandler() interrupt INT0_VECTOR
{
	if (int_irq_handlers[0] != NULL)
    {
        int_irq_handlers[0]();
    }
}

//void INT1_IRQHandler(void) interrupt INT1_VECTOR  //外部中断1，默认留给摄像头

void INT2_IRQHandler(void) interrupt INT2_VECTOR
{
	if (int_irq_handlers[2] != NULL)
    {
		int_irq_handlers[2]();
    }
}
void INT3_IRQHandler(void) interrupt INT3_VECTOR
{
	if (int_irq_handlers[3] != NULL)
    {
		int_irq_handlers[3]();
    }
}

void INT4_IRQHandler(void) interrupt INT4_VECTOR
{
	if (int_irq_handlers[4] != NULL)
    {
		int_irq_handlers[4]();
    }
}

void DMA_UART1_IRQHandler(void) interrupt DMA_UR1R_VECTOR
{
    static vuint8 dwon_count = 0;

    if (DMA_UR1R_STA & 0x01) // 接收完成
    {
        DMA_UR1R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_1); // 设置下一次接收，务必保留

        // 程序自动下载
        if (uart_rx_buff[UART_1][0] == 0x7F)
        {
            if (dwon_count++ > 20)
            {
                IAP_CONTR = 0x60;
            }
        }
        else
        {
            dwon_count = 0;
        }

        if (uart_rx_handlers[UART_1] != NULL)
        {
            uart_rx_handlers[UART_1](uart_rx_buff[UART_1][0]);
        }
    }

    if (DMA_UR1R_STA & 0x02) // 数据丢弃
    {
        DMA_UR1R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_1); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART2_IRQHandler(void) interrupt DMA_UR2R_VECTOR
{
    if (DMA_UR2R_STA & 0x01) // 接收完成
    {
        DMA_UR2R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_2); // 设置下一次接收，务必保留

        if (uart_rx_handlers[UART_2] != NULL)
        {
            uart_rx_handlers[UART_2](uart_rx_buff[UART_2][0]);
        }
    }

    if (DMA_UR2R_STA & 0x02) // 数据丢弃
    {
        DMA_UR2R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_2); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART3_IRQHandler(void) interrupt DMA_UR3R_VECTOR
{
    if (DMA_UR3R_STA & 0x01) // 接收完成
    {
        DMA_UR3R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_3); // 设置下一次接收，务必保留

        if (uart_rx_handlers[UART_3] != NULL)
        {
            uart_rx_handlers[UART_3](uart_rx_buff[UART_3][0]);
        }
    }

    if (DMA_UR3R_STA & 0x02) // 数据丢弃
    {
        DMA_UR3R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_3); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART4_IRQHandler(void) interrupt DMA_UR4R_VECTOR
{
    if (DMA_UR4R_STA & 0x01) // 接收完成
    {
        DMA_UR4R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_4); // 设置下一次接收，务必保留

        if (uart_rx_handlers[UART_4] != NULL)
        {
            uart_rx_handlers[UART_4](uart_rx_buff[UART_4][0]);
        }
    }

    if (DMA_UR4R_STA & 0x02) // 数据丢弃
    {
        DMA_UR4R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_4); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}


void DMA_UART5_IRQHandler(void) interrupt DMA_UR5R_VECTOR
{
    if (DMA_UR5R_STA & 0x01) // 接收完成
    {
        DMA_UR5R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_5); // 设置下一次接收，务必保留

        if (uart_rx_handlers[UART_5] != NULL)
        {
            uart_rx_handlers[UART_5](uart_rx_buff[UART_5][0]);
        }
    }

    if (DMA_UR5R_STA & 0x02) // 数据丢弃
    {
        DMA_UR5R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_5); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART6_IRQHandler(void) interrupt DMA_UR6R_VECTOR
{
    if (DMA_UR6R_STA & 0x01) // 接收完成
    {
        DMA_UR6R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_6); // 设置下一次接收，务必保留

        if (uart_rx_handlers[UART_6] != NULL)
        {
            uart_rx_handlers[UART_6](uart_rx_buff[UART_6][0]);
        }
    }

    if (DMA_UR6R_STA & 0x02) // 数据丢弃
    {
        DMA_UR6R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_6); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART7_IRQHandler(void) interrupt DMA_UR7R_VECTOR
{
    if (DMA_UR7R_STA & 0x01) // 接收完成
    {
        DMA_UR7R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_7); // 设置下一次接收，务必保留

        if (uart_rx_handlers[UART_7] != NULL)
        {
            uart_rx_handlers[UART_7](uart_rx_buff[UART_7][0]);
        }
    }

    if (DMA_UR7R_STA & 0x02) // 数据丢弃
    {
        DMA_UR7R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_7); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART8_IRQHandler(void) interrupt DMA_UR8R_VECTOR
{
    if (DMA_UR8R_STA & 0x01) // 接收完成
    {
        DMA_UR8R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_8); // 设置下一次接收，务必保留

        if (uart_rx_handlers[UART_8] != NULL)
        {
            uart_rx_handlers[UART_8](uart_rx_buff[UART_8][0]);
        }
    }

    if (DMA_UR8R_STA & 0x02) // 数据丢弃
    {
        DMA_UR8R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_8); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}


void TM0_IRQHandler() interrupt TMR0_VECTOR
{
    TIM0_CLEAR_FLAG;
    if (tim_irq_handlers[TIM_0] != NULL)
    {
        tim_irq_handlers[TIM_0]();
    }
}

void TM1_IRQHandler() interrupt TMR1_VECTOR
{
    TIM1_CLEAR_FLAG;
    if (tim_irq_handlers[TIM_1] != NULL)
    {
        tim_irq_handlers[TIM_1]();
    }
}

void TM2_IRQHandler() interrupt TMR2_VECTOR
{
    TIM2_CLEAR_FLAG;
    if (tim_irq_handlers[TIM_2] != NULL)
    {
        tim_irq_handlers[TIM_2]();
    }
}

void TM3_IRQHandler() interrupt TMR3_VECTOR
{
    TIM3_CLEAR_FLAG;
    if (tim_irq_handlers[TIM_3] != NULL)
    {
        tim_irq_handlers[TIM_3]();
    }
}

void TM4_IRQHandler() interrupt TMR4_VECTOR
{
    TIM4_CLEAR_FLAG;
    if (tim_irq_handlers[TIM_4] != NULL)
    {
        tim_irq_handlers[TIM_4]();
    }
}

void TM5_TM6_IRQHandler() interrupt TMR5_TMR6_VECTOR
{
    if(TIM5_GET_FLAG)
    {
        TIM5_CLEAR_FLAG;
        if (tim_irq_handlers[TIM_5] != NULL)
        {
            tim_irq_handlers[TIM_5]();
        }
    }
    
    if(TIM6_GET_FLAG)
    {
        TIM6_CLEAR_FLAG;
        if (tim_irq_handlers[TIM_6] != NULL)
        {
            tim_irq_handlers[TIM_6]();
        }
    }
}

void TM7_IRQHandler() interrupt TMR7_TMR8_VECTOR
{
    if(TIM7_GET_FLAG)
    {
        TIM7_CLEAR_FLAG;
        if (tim_irq_handlers[TIM_7] != NULL)
        {
            tim_irq_handlers[TIM_7]();
        }
    }
    
    if(TIM8_GET_FLAG)
    {
        TIM8_CLEAR_FLAG;
        if (tim_irq_handlers[TIM_8] != NULL)
        {
            tim_irq_handlers[TIM_8]();
        }
    }
}


void TM9_TM10_IRQHandler() interrupt TMR9_TMR10_VECTOR
{
    if(TIM9_GET_FLAG)
    {
        TIM9_CLEAR_FLAG;
        if (tim_irq_handlers[TIM_9] != NULL)
        {
            tim_irq_handlers[TIM_9]();
        }
    }
    
    if(TIM10_GET_FLAG)
    {
        TIM10_CLEAR_FLAG;
        if (tim_irq_handlers[TIM_10] != NULL)
        {
            tim_irq_handlers[TIM_10]();
        }
    }
}



void TM11_IRQHandler() interrupt TMR11_VECTOR
{
    TIM11_CLEAR_FLAG;

    if (tim_irq_handlers[TIM_11] != NULL)
    {
        tim_irq_handlers[TIM_11]();
    }
}

void TM17_TM18_IRQHandler() interrupt TMR17_TMR18_VECTOR
{
    if(TIM17_GET_FLAG)
    {
        TIM17_CLEAR_FLAG;
        if (tim_irq_handlers[TIM_17] != NULL)
        {
            tim_irq_handlers[TIM_17]();
        }
    }
    
    if(TIM18_GET_FLAG)
    {
        TIM18_CLEAR_FLAG;
        if (tim_irq_handlers[TIM_18] != NULL)
        {
            tim_irq_handlers[TIM_18]();
        }
    }
}

void INT1_IRQHandler() interrupt INT1_VECTOR
{
//    if(mt9v03x_finish_flag == 1)
//    {
//        return;
//    }
    mt9v03x_vsync_handler();
}


void DMA_LCM_IRQHandler() interrupt DMA_LCM_VECTOR
{
    mt9v03x_dma_handler();
}





//#define     INT0_VECTOR             0       //0003H
//#define     TMR0_VECTOR             1       //000BH
//#define     INT1_VECTOR             2       //0013H
//#define     TMR1_VECTOR             3       //001BH
//#define     UART1_VECTOR            4       //0023H
//#define     ADC_VECTOR              5       //002BH
//#define     LVD_VECTOR              6       //0033H
////#define   PCA_VECTOR              7       //003BH
//#define     UART2_VECTOR            8       //0043H
//#define     SPI_VECTOR              9       //004BH
//#define     INT2_VECTOR             10      //0053H
//#define     INT3_VECTOR             11      //005BH
//#define     TMR2_VECTOR             12      //0063H
//#define     USER_VECTOR             13      //006BH
//#define     BRK_VECTOR              14      //0073H
//#define     ICEP_VECTOR             15      //007BH
//#define     INT4_VECTOR             16      //0083H
//#define     UART3_VECTOR            17      //008BH
//#define     UART4_VECTOR            18      //0093H
//#define     TMR3_VECTOR             19      //009BH
//#define     TMR4_VECTOR             20      //00A3H
//#define     CMP_VECTOR              21      //00ABH
////#define   PWM_VECTOR              22      //00B3H
////#define   PWMFD_VECTOR            23      //00BBH
//#define     I2C_VECTOR              24      //00C3H
//#define     USB_VECTOR              25      //00CBH
//#define     PWMA_VECTOR             26      //00D3H
//#define     PWMB_VECTOR             27      //00DBH
//#define     CAN1_VECTOR             28      //00E3H
//#define     CAN2_VECTOR             29      //00EBH
//#define     LIN_VECTOR              30      //00F3H

//#define     RTC_VECTOR              36      //0123H
//#define     P0INT_VECTOR            37      //012BH
//#define     P1INT_VECTOR            38      //0133H
//#define     P2INT_VECTOR            39      //013BH
//#define     P3INT_VECTOR            40      //0143H
//#define     P4INT_VECTOR            41      //014BH
//#define     P5INT_VECTOR            42      //0153H
//#define     P6INT_VECTOR            43      //015BH
//#define     P7INT_VECTOR            44      //0163H
//#define     DMA_M2M_VECTOR          47      //017BH
//#define     DMA_ADC_VECTOR          48      //0183H
//#define     DMA_SPI_VECTOR          49      //018BH
//#define     DMA_UR1T_VECTOR         50      //0193H
//#define     DMA_UR1R_VECTOR         51      //019BH
//#define     DMA_UR2T_VECTOR         52      //01A3H
//#define     DMA_UR2R_VECTOR         53      //01ABH
//#define     DMA_UR3T_VECTOR         54      //01B3H
//#define     DMA_UR3R_VECTOR         55      //01BBH
//#define     DMA_UR4T_VECTOR         56      //01C3H
//#define     DMA_UR4R_VECTOR         57      //01CBH
//#define     DMA_LCM_VECTOR          58      //01D3H
//#define     LCM_VECTOR              59      //01DBH
//#define     DMA_I2CT_VECTOR         60      //01E3H
//#define     DMA_I2CR_VECTOR         61      //01EBH
//#define     I2S_VECTOR              62      //01F3H
//#define     DMA_I2ST_VECTOR         63      //01FBH
//#define     DMA_I2SR_VECTOR         64      //0203H
