#include "PID.h"
#include "math.h"
float error_prev[3] = {0};
float error_last[3] = {0};
float error[3];
float coefficient = STEER_P_COEFFICIENT;   //动态P的系数  0.0945  0.101     0.125  0.15
float coefficient_d = STEER_D_COEFFICIENT; //动态D的系数 0.154 0.17 0.19  0.5
float Kb = 0.0;                            //抗饱和系数0.1~3.0*Ki/Kd
/*----------------------------------------------------------------
* @brief  PID抗饱和检测函数
* @param  Kb:抗饱和系数
* @return constrain_error：抗饱和后的误差
----------------------------------------------------------------*/
float intergral_constrain(float out, float max, float min)
{
    float constrain_error;
    if (out > max)
    {
        constrain_error = max;
    }
    else
    {
        if (out < min)
        {
            constrain_error = min;
        }
        else
        {
            constrain_error = out;
        }
    }
    return constrain_error - out;
}
/*----------------------------------------------------------------
@brief：增量式PID计算
@author : AgainstWar
@param value 实际值
@param target 目标值
@param Kp 比例系数
@param Ki 积分系数
@param Kd 微分系数
@return output 计算后的PID输出值
@用法：pid_calculation(12); //计算当前值为12时PID的输出值
----------------------------------------------------------------*/
float pid_calculation_1(float Kp, float Ki, float Kd, int16 value, int16 target, int16 motor)
{

    float output[3] = {0}; // PID输出值
    // Kb = 2.8 * Ki / Kd;
    error[motor] = target - value;
    Kp *= error[motor] - error_last[motor];
    Ki *= error[motor];
    Kd *= error[motor] - 2 * error_last[motor] + error_prev[motor]; //微分值
    // Ki += Kb * intergral_constrain(output[motor], 100, -300);           //抗饱和积分
    output[motor] = Kp + Ki + Kd; //输出值
    error_prev[motor] = error_last[motor];
    error_last[motor] = error[motor]; //误差值更新

    return output[motor]; //输出值
}
/*----------------------------------------------------------------
@brief：位置式PID计算
@author : ***
@param value1 实际值
@return output 计算后的PID输出值
----------------------------------------------------------------*/
float pid_calculation_2(float kp, float kd, float value1)
{
    float error, output_1, target, increment_p = 0, increment_d = 0;
    static float last_error = 0.0;
    target = 0.0;
    error = target - value1;
    if (ad_answer[0] - ad_answer[2] > 0 || ad_answer[4] - ad_answer[2] > 0) // if(fabs(error)>=10&&fabs(ad_anwer[1]-ad_anwer[3])>=35)
    {
        increment_d = 0;
        increment_p = fabs(error * coefficient);
    }
    else
    {

        increment_d = fabs(error * coefficient_d);
        increment_p = 0;
    }
    output_1 = (kp + increment_p) * error + (kd + increment_d) * (error - last_error);
    last_error = error;
    return output_1;
}