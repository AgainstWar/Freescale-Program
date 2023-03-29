#ifndef __PID_H_
#define __PID_H_
#include "headfile.h"
extern float coefficient;
extern float coefficient_d;
float pid_calculation_1(float Kp, float Ki, float Kd, int16 value, int16 target, int16 motor);
float pid_calculation_2(float kp, float kd, float value1);
#endif