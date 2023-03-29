#ifndef __motor_h_
#define __motor_h_
#include "headfile.h"
#include "PID.h"
#define PWM_R PWMA_CH2P_P62 //左电机PWM
#define DIR_R P60           //左电机方向
#define PWM_L PWMA_CH3P_P64 //右电机PWM
#define DIR_L P66           //右电机方向
extern int16 speed;
//extern int32 speed_L;
//extern int32 speed_R;
extern int16 L_value;
extern int16 R_value;
extern float L_Kp, L_Ki, L_Kd;
extern float R_Kp, R_Ki, R_Kd;
extern int16 expited_L_R ;
extern uint16 dat[20];
extern int32 R_duty ; //右电机PWM占空比
extern int32 L_duty ; //右电机PWM占空比
extern int32 count_encode_L;
extern int32 count_encode_R;
extern uint32 count_motor;
void motor_calculation();
void motor_control_R(int32 power_R);
void motor_control_L(int32 power_L);
void motor_test();
void motor_differential();
void Motor_Caculate();
#endif