#ifndef __DEBUG_H_
#define __DEBUG_H_
#include "headfile.h"
#define ENCODE_DIR_L P50   //左电机方向
#define ENCODE_DIR_R P07   //右电机方向
#define ENCODE_L CTIM0_P34 //左电机编码器计数
#define ENCODE_R CTIM3_P04 //右电机编码器计数
extern float gyro_x, gyro_y, gyro_z;
extern float acc_x, acc_y, acc_z;
void gyro_data_get();
void gyro_zero_process();
int16 DATE_L_get();
int16 DATE_R_get();
#endif