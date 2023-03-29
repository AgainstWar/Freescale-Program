//舵机
#ifndef _STEER_H_
#define _STEER_H_
#include "headfile.h"
extern float num_kp;
extern float num_kd;
extern int16 real_steer;
extern float num_kp_normal_h; 
extern float num_kd_normal_h;
extern float num_kp_normal_l; 
extern float num_kd_normal_l;
extern float num_kp_y; 
extern float num_kd_y;  
extern int16 adjust_steer; 
#define pwm_steer PWMB_CH1_P74
#define pwm_duty_center 750
#define pwm_duyt_left 830
#define pwm_duty_right 670
void control_test();
#endif