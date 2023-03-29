/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/
#include "headfile.h"
/*
 *关于内核频率的设定，可以查看board.h文件
 *在board_init中,已经将P54引脚设置为复位
 *如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */
int16 ad_data;
void main()
{
  DisableGlobalIRQ(); //关闭总中断
  // sys_clk可选值:30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600。
  //设置系统频率，此频率需要跟STC-ISP软件中的 <输入用户程序运行的IRC频率>选项的频率一致。
  //如果频率设置不对，将会导致串口的数据不正常,PWM的工作不正常等等。
  sys_clk = 30000000; //设置系统频率为30MHz
  board_init();       //初始化寄存器
  //此处编写用户代码(例如：外设初始化代码等)
  system_init(); //初始化系统
 
  //总中断最后开启
  EnableGlobalIRQ(); //开启总中断
  WDT_CONTR = 0x20;
  while (1)
  {
    WDT_CONTR = 0x30;
    //此处编写需要循环执行的代码
    // data_conversion((int16)ad_anwer[0], (int16)ad_anwer[2], (int16)ad_anwer[4], 0, dat);
     //data_conversion((int16)count_encode_R,(int16)count_encode_L, 0,0, dat);
    //display_data();
    // P77 = 1;
    // menu();
  }
}
