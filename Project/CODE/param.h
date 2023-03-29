#ifndef __PARAM_H__
#define __PARAM_H__
#include <headfile.h>
#define MOTOR_P 18.0             //电机P参数 L_Kp,R_Kp
#define MOTOR_I 1.30             //电机I参数 L_Ki,R_Ki
#define MOTOR_D 0                //电机D参数 L_Kd,R_Kd
#define STEER_P_MID 4.6         //舵机中速P参数 num_kp_normal_h
#define STEER_D_MID 28          //舵机中速D参数 num_kd_normal_h
#define STEER_P_LOW 3.8          //舵机低速P参数 num_kp_normal_l
#define STEER_D_LOW 20           //舵机低速D参数 num_kd_normal_l
#define STEER_P_HIGH 7.8       //舵机高速P参数 num_kp_normal_h_r
#define STEER_D_HIGH 45        //舵机高速D参数 num_kd_normal_h_r
#define STEER_P_Y 3.50           //舵机三岔P参数 num_kp_y
#define STEER_D_Y 20             //舵机三岔D参数 num_kd_y
#define STEER_P_COEFFICIENT 0.15 //舵机P动态参数 coeffient
#define STEER_D_COEFFICIENT 0.5  //舵机D动态参数 coeffient_d
#define SPEED_HIGH 480           //直道加速最高速
#define SPEED_LOW_R 370          //弯道减速速度
#define SPEED_LOW 390            //直道加速最低速度
#define SPEED_LOSS 290           //丢线速度
#define SPEED_Y_IN 200           //检测到三岔时的速度
#define SPEED_Y_L 400            //在左三叉中速度
#define SPEED_Y_R 400            //在右三叉中速度
#define SPEED_Y_OUT 330          //出三岔的速度
#define DISTANCE_Y_IN_L 15000   //左三岔的积分距离
#define DISTANCE_Y_IN_R 15000    //右三岔的积分距离
#define DISTANCE_Y_OUT 110000    //离开三岔积分距离
#define DISTANCE_ROUND_IN 50000  //进入环岛积分距离
#define DISTANCE_ROUND_OUT 11000 //出环岛直线积分距离
#define DISTANCE_SLOPE 70000     //坡道积分距离
#define DISTANCE_STOP 5000       //停车检测积分距离
#define DISTANCE_INGARAGE -20000 //入库倒车距离
#define SPEED_INGARAGE -200      //入库倒车速度
#define STEER_INGARAGE 20        //倒车入库打角
#endif