/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		逐飞科技无线转串口模块
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-03-27
 * @note
                    接线定义：
                    ------------------------------------
                        无线转串口       单片机
                        RX              查看SEEKFREE_WIRELESS.h文件中的WIRELESS_UART_TX宏定义
                        TX              查看SEEKFREE_WIRELESS.h文件中的WIRELESS_UART_RX宏定义
                        RTS             查看SEEKFREE_WIRELESS.h文件中的RTS_PIN宏定义
                        CMD             查看SEEKFREE_WIRELESS.h文件中的CMD_PIN宏定义
                    ------------------------------------
 ********************************************************************************************************************/

#include "SEEKFREE_WIRELESS.h"
#include "zf_uart.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      无线转串口模块回调函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_callback(void)
{
    uint8 wireless_rx_buffer = WIRELESS_DATA_BUF;
    //接到一个字节后单片机将会进入串口中断，通过在此处读取wireless_rx_buffer可以取走数据
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      无线转串口模块初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
void seekfree_wireless_init(void)
{
    wireless_type = WIRELESS_CH9141;
    //本函数使用的波特率为115200，为无线转串口模块的默认波特率，如需其他波特率请自行配置模块并修改串口的波特率

    uart_init(WIRELESS_UART, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN, WIRELESS_UART_BAUD, WIRELESS_TIMER_N); //初始化串口
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      无线转串口模块 发送函数
//  @param      buff        需要发送的数据地址
//  @param      len         发送长度
//  @return     uint32      剩余未发送的字节数
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint32 seekfree_wireless_send_buff(uint8 *buff, uint16 len)
{
    while (len > 30)
    {
        if (RTS_PIN == 1)
        {
            return len; //模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
        }
        // while(RTS_PIN);  //如果RTS为低电平，则继续发送数据
        uart_putbuff(WIRELESS_UART, buff, 30);

        buff += 30; //地址偏移
        len -= 30;  //数量
    }

    if (RTS_PIN == 1)
    {
        return len; //模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
    }
    // while(RTS_PIN);  //如果RTS为低电平，则继续发送数据
    uart_putbuff(WIRELESS_UART, buff, len); //发送最后的数据

    return 0;
}