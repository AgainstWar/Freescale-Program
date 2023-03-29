#ifndef __AD_H_
#define __AD_H_
#include "headfile.h"
#include "math.h"
//变量
//采集变量
#define ADC_L_1 ADC_P00 //左1
#define ADC_L_M ADC_P01 //左中
#define ADC_M ADC_P05   //中
#define ADC_R_M ADC_P06 ///右中
#define ADC_R_1 ADC_P13 //右1
//#define ADC_B ADC_P16 /后一
//#define guanghuanghuan P33

extern float AD_out;
extern float AD_diff;  //左右电感差
extern float AD_sum;   //左右电感和
extern float AD_level; //数值电感差
extern float AD_vertical;
extern float Gyro_Pitch_Angle;
extern float ad_answer[6];
extern float AD_N;
extern int8 Y_steer;
extern int ganhuangguan ;
extern int line_flag;
extern int8 stop_flag;
extern int8 P32_Status; 
float Q_rsqrt(float number);
float ADC_Ave(uint8, uint8);
void ADC_Get_deal();
void Y_deal();
void round_deal();
void Slope_Process(void);
void Cross();
void Parking_Fuction();
#endif
