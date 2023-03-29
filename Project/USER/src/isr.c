///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,逐飞科技
// * All rights reserved.
// * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
// *
// * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
// * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
// *
// * @file       		isr
// * @company	   		成都逐飞科技有限公司
// * @author     		逐飞科技(QQ790875685)
// * @version    		查看doc内version文件 版本说明
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC16F40K128
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"

// UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
    static uint8 dwon_count;
    if (UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if (UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if (res == 0x7F)
        {
            if (dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

// UART2中断
void UART2_Isr() interrupt 8
{
    if (UART2_GET_TX_FLAG)
    {
        UART2_CLEAR_TX_FLAG;
        busy[2] = 0;
    }
    if (UART2_GET_RX_FLAG)
    {
        UART2_CLEAR_RX_FLAG;
        //接收数据寄存器为：S2BUF
    }
}

// UART3中断
void UART3_Isr() interrupt 17
{
    if (UART3_GET_TX_FLAG)
    {
        UART3_CLEAR_TX_FLAG;
        busy[3] = 0;
    }
    if (UART3_GET_RX_FLAG)
    {
        UART3_CLEAR_RX_FLAG;
        //接收数据寄存器为：S3BUF
    }
}

// UART4中断
void UART4_Isr() interrupt 18
{
    if (UART4_GET_TX_FLAG)
    {
        UART4_CLEAR_TX_FLAG;
        busy[4] = 0;
    }
    if (UART4_GET_RX_FLAG)
    {
        UART4_CLEAR_RX_FLAG;

        //接收数据寄存器为：S4BUF;
        if (wireless_type == WIRELESS_SI24R1)
        {
            wireless_uart_callback(); //无线转串口回调函数
        }
        else if (wireless_type == WIRELESS_CH9141)
        {
            bluetooth_ch9141_uart_callback(); //蓝牙转串口回调函数
        }
    }
}

#define LED P52
void INT0_Isr() interrupt 0
{
   /* if (P32)
    {
        LED = 0; //点亮LED
        P77 = 1;
        P32_Status = 1;
    }
    else
    {
        LED = 1; //熄灭LED
        P77 = 0;
        P32_Status = 0;
    }*/
}

void INT1_Isr() interrupt 2
{
}
void INT2_Isr() interrupt 10
{
    INT2_CLEAR_FLAG; //清除中断标志
}
void INT3_Isr() interrupt 11
{
    INT3_CLEAR_FLAG; //清除中断标志
}

void INT4_Isr() interrupt 16
{
    INT4_CLEAR_FLAG; //清除中断标志
}

void TM0_Isr() interrupt 1
{
}
void TM1_Isr() interrupt 3
{
}
/**串口中断**/
void TM2_Isr() interrupt 12
{
    TIM2_CLEAR_FLAG; //清除中断标志
}

void TM3_Isr() interrupt 19
{
    TIM3_CLEAR_FLAG; //清除中断标志
}
uint32 count = 0;
void TM4_Isr() interrupt 20
{
    TIM4_CLEAR_FLAG; //清除中断标志
    count++;         //计数
    ADC_Get_deal();  // ADC采集
    Parking_Fuction();
    if (count % 2 == 0)
    {
        control_test();
    }
    if (count % 3 == 0)
    {

        L_value = Kalman_left(DATE_L_get()); //编码器采集并滤波
        R_value = Kalman_right(DATE_R_get());
        gyro_data_get(); //陀螺仪
    }
    if (count % 4 == 0)
    {
        motor_test();
        count_motor = 1;
    }
    if (count >= 5000)
    {
        count = 0;
    }
}

// void  INT0_Isr()  interrupt 0;
// void  TM0_Isr()   interrupt 1;
// void  INT1_Isr()  interrupt 2;
// void  TM1_Isr()   interrupt 3;
// void  UART1_Isr() interrupt 4;
// void  ADC_Isr()   interrupt 5;
// void  LVD_Isr()   interrupt 6;
// void  PCA_Isr()   interrupt 7;
// void  UART2_Isr() interrupt 8;
// void  SPI_Isr()   interrupt 9;
// void  INT2_Isr()  interrupt 10;
// void  INT3_Isr()  interrupt 11;
// void  TM2_Isr()   interrupt 12;
// void  INT4_Isr()  interrupt 16;
// void  UART3_Isr() interrupt 17;
// void  UART4_Isr() interrupt 18;
// void  TM3_Isr()   interrupt 19;
// void  TM4_Isr()   interrupt 20;
// void  CMP_Isr()   interrupt 21;
// void  I2C_Isr()   interrupt 24;
// void  USB_Isr()   interrupt 25;
// void  PWM1_Isr()  interrupt 26;
// void  PWM2_Isr()  interrupt 27;