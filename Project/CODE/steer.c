/*******************************************************************************
 * 电机控制程序  steer.c
 *
 *
 * file name:           steer.c
 * description:        舵机控制程序
 * author:               ***
 * version：            v0.0.1
 * date：                2022.02.27
 * note:
 *******************************************************************************/
#include "steer.h"
#include "math.h"
float num_kp;                        // 4.94  4.0
float num_kd;                        // 37
float num_kp_y = STEER_P_Y;          // 3.5
float num_kd_y = STEER_D_Y;          // 25
float num_kp_normal_h = STEER_P_MID; // 3.54  4.94  3.6  3.60  3.2  2.8 3.0  3.2  3.3  高速  3.4
float num_kd_normal_h = STEER_D_MID; // 27
float num_kp_normal_l = STEER_P_LOW; //低速  1.0  1.7
float num_kd_normal_l = STEER_D_LOW;
float num_kp_normal_h_r = STEER_P_HIGH; //低速  1.0  1.7
float num_kd_normal_h_r = STEER_D_HIGH; // 27
int16 value;                            //真实的
int16 adjust_steer;                     //调整的pwm
int16 real_steer = pwm_duty_center;     //输出的pwm

void control_test()
{
  if (Y_steer != 0)
  {
    num_kp = num_kp_y;
    num_kd = num_kd_y;
  }
  else if ((L_value + R_value) / 2 <= 370 && expited_L_R != 290)
  {
    num_kp = num_kp_normal_l;
    num_kd = num_kd_normal_l;
  }
  else if ((L_value + R_value) / 2 >= 410)
  {
    num_kp = num_kp_normal_h_r;
    num_kd = num_kd_normal_h_r;
  }
  else
  {
    num_kp = num_kp_normal_h;
    num_kd = num_kd_normal_h;
  }

  adjust_steer = pid_calculation_2(num_kp, num_kd, AD_out);
  real_steer = adjust_steer + pwm_duty_center;
  /***限幅***/
  if (real_steer > pwm_duyt_left)
  {
    real_steer = pwm_duyt_left;
  }
  else if (real_steer < pwm_duty_right)
  {
    real_steer = pwm_duty_right;
  }
  pwm_duty(pwm_steer, real_steer);
}
