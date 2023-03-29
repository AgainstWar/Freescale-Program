/*******************************************************************************
 * 电机控制程序   motor.c
 * file name:          motor.c
 * description:        电机控制程序
 * author:             ***,AgainstWar
 * version：            v1.0.2
 * date：               2022.02.27
 * note:
 *******************************************************************************/
#include "motor.h"
#include "math.h"
#define PWM_base_duty 2000
int32 speed_L = 0;
int32 speed_R = 0;
int16 speed = 0;
int16 L_value;
int16 R_value;
int16 limit = 9500; //限幅值
int16 expited_L_R = 250;
float L_Kp = MOTOR_P, L_Ki = MOTOR_I, L_Kd = MOTOR_D; // 1.99 1.2  10.0 0.96  i=1  d=3  p=10
float R_Kp = MOTOR_P, R_Ki = MOTOR_I, R_Kd = MOTOR_D; // 3.89
float K = 10.8;                           //轮距10.8cm
float L = 20.4;                           //轴距20.4cm
float a = 1.90;                           //中心到主销距离
int32 L_duty;                             //左电机PWM占空比
int32 R_duty;                             //右电机PWM占空比
uint16 dat[20];
/*直道加速，弯道减速变量*/
int32 count_encode_L = 0;
int32 count_encode_R = 0;
int16 straight_timer = 0;
int16 straight_flag = 0;
float speed_transition = 3;
int16 speed_add_time = 5;
int16 speed_add = 50;
int16 encoder_ex = 400;
int real_steer_difference;
int real_steer_difference_1;
uint32 count_motor = 0;
void motor_calculation()
{
    // L_value=- L_value;
    // R_value=- R_value;
    speed_L += (int32)pid_calculation_1(L_Kp, L_Ki, L_Kd, L_value, expited_L_R, 0);
    speed_R += (int32)pid_calculation_1(R_Kp, R_Ki, R_Kd, R_value, expited_L_R, 1);
}
/***差速***/
void motor_differential()
{
    int16 adjust_steer;
    float adjust_steer_1;
    float adjust_steer_2;
    float angle, radian, speed_dif;
   
    if (fabs(adjust_steer) > 30) //误差大小判断是否进入
    {
         adjust_steer_1 = adjust_steer / 10;
        angle = fabs(adjust_steer) * 0.467;
        radian = tan(angle * 0.0174);
        if (fabs(adjust_steer) > 60) //大弯
        {
            adjust_steer_2 = 1.5*pow(3, adjust_steer_1);
            /*adjust_steer_2 /= 2;
            adjust_steer_2*=1.5;*/
            speed_dif = 3 * (float)adjust_steer_2 * radian * radian;
        }
        else
        {
            adjust_steer_2 = pow(2, adjust_steer_1);
            adjust_steer_2 *= 5.6;
            speed_dif = 3 * (float)adjust_steer_2 * radian * radian;
        }

        if (AD_out >= 0)
        {
            speed_L = (speed_L + speed_dif);
        }
        if (AD_out < 0)
        {
            speed_R = (speed_R + speed_dif);
        }
        if (stop_flag != 0)
        {

            speed_L = 500;
            speed_R =0;
        }
    }
}
void Motor_Caculate()
{
    //直道加速
    if (fabs(AD_out) <= speed_transition && straight_flag == 0) //偏差小的时候，要延时之后加速
    {
        straight_timer++;
        if (straight_timer >= speed_add_time) // 1秒定时
        {
            straight_timer = 0;
            straight_flag = 1; //加速标志位
        }
    }
    if (fabs(AD_out) > speed_transition) //只要偏差大，就清零计数，标志非直道
    {
        straight_timer = 0;
        straight_flag = 0;
    }
    if (straight_flag == 1)
        expited_L_R = 300; //直道加速
    if (straight_flag == 0)
        expited_L_R = 280; //弯道正常
    if (AD_out > 15)
        expited_L_R = 260;
}
/*----------------------------------------------------------------
* @brief  电机控制函数
* @param  power:最终计算电机功率(20.0~100.0)
* @param  Direction:电机转动方向
* @return void
* @author AgainstWar
* @date   2022/02/27
* @Sample usage: motor_control_R(28.5,1);   右电机28.5%功率运行,正转
----------------------------------------------------------------*/
void motor_control_R(int32 power_R)
{
    R_duty = power_R;
    if (R_duty < 0)
    {
        DIR_R = 1;
        R_duty *= -1;
    }
    else
        DIR_R = 0;
    if (R_duty > limit) //限幅
        R_duty = limit;
    pwm_duty(PWM_R, R_duty);
}
//----------------------同上---------
void motor_control_L(int32 power_L)
{
    L_duty = power_L;
    if (L_duty < 0)
    {
        DIR_L = 1;
        L_duty *= -1;
    }
    else
        DIR_L = 0;
    if (L_duty > limit) //限幅
        L_duty = limit;
    pwm_duty(PWM_L, L_duty);
}
void motor_test()
{
    // Motor_Caculate();//直道加速 弯道减速
    if (count_motor == 0) //判断第一次进入
    {
        expited_L_R = 200;
    }
    motor_calculation();
    if (Y_steer == 0)
    {
        motor_differential();
    }
    motor_control_L(speed_L);
    motor_control_R(speed_R);
}
