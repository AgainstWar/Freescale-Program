/*******************************************************************************
 * 电磁信号处理程序   AD.c
 *
 *
 * @file name:              AD.c
 * @description:           电磁信号处理程序
 * @author:                ---
 * @version：              v0.0.1
 * @date：                 2022.03.03
 * @note:
 *******************************************************************************/
#include "AD.h"
#include "math.h"

#define N 12

float AD_out;   // AD 处理输出
float AD_diff;  //左右电感差
float AD_sum;   //左右电感和
float AD_level; //数值电感差
float AD_vertical;
float AD_N;
float AD_position = 0; //丢线参数
int AD_flag = 0;       //上次AD_out值的正负
int32 count_isr = 0;

float ad_value0[N]; //储存采集的电感值
float ad_value1[N];
float ad_value2[N];
float ad_value3[N];
float ad_value4[N];
float ad_value5[N];

float ad_values[6] = {0.0}; //存放每个电感处理后的平均值
float ad_answer[6];         //归一化后的值

int line_flag = 0;  //丢线标志位
float last_AD_out;  //丢线AD处理
float AD_out_1 = 0; //用于小部分滞后
float AD_out_2 = 0;
int8 AD_out_flag = 0; //滞后标志位

int8 Slope_N = 0; //坡道标志

/*******************************************************************************
 * function name:      Q_rsqrt(float number)
 * description:    开根号
 * author:             ---
 * version:            v0.0.1
 * date：               2022.04.03
 * note:
 *******************************************************************************/
float Q_rsqrt(float number)
{
  long i;
  float x, y;
  const float f = 1.5F;
  x = number * 0.5F;
  y = number;
  i = *(long *)&y;
  i = 0x5f3759df - (i >> 1);

  y = *(float *)&i;
  y = y * (f - (x * y * y));
  y = y * (f - (x * y * y));
  return number * y;
}

/*******************************************************************************
 * @functionname:        ADC_Ave( )
 * @description:         电感值2次求平均函数
 * @author:              ---
 * @version：             v0.0.1
 * @date：                2022.03.03
 * @note:
 *******************************************************************************/

float ADC_Ave(uint8 adcn, uint8 resolution) //管脚，分辨率
{
  float u, j;
  u = adc_once(adcn, resolution);
  j = adc_once(adcn, resolution);
  return ((u + j) / 2);
}

/*******************************************************************************
 * function name:      ADC_Get_deal( )
 * description:        电感值采集和处理函数
 * author:             ---
 * version:            v0.0.1
 * date：               2022.03.05
 * note:
 *******************************************************************************/
void ADC_Get_deal()
{
  int i;
  count_isr++;
  //电感原值
  for (i = 0; i < N; i++)
  {
    ad_value0[i] = ADC_Ave(ADC_R_1, ADC_12BIT);
    ad_value1[i] = ADC_Ave(ADC_R_M, ADC_12BIT);
    ad_value2[i] = ADC_Ave(ADC_M, ADC_12BIT);
    ad_value3[i] = ADC_Ave(ADC_L_M, ADC_12BIT);
    ad_value4[i] = ADC_Ave(ADC_L_1, ADC_12BIT);
    // ad_value5[i] = ADC_Ave(ADC_B, ADC_12BIT);
  }
  //处理

  for (i = 0; i < N; i++)
  {
    ad_values[0] += ad_value0[i];
    ad_values[1] += ad_value1[i];
    ad_values[2] += ad_value2[i];
    ad_values[3] += ad_value3[i];
    ad_values[4] += ad_value4[i];
    // ad_values[5] += ad_value5[i];
  }

  //得到平均值
  ad_values[0] /= 12;
  ad_values[1] /= 12;
  ad_values[2] /= 12;
  ad_values[3] /= 12;
  ad_values[4] /= 12;
  // ad_values[5] /= 12;

  //归一化处理
  ad_answer[0] = (ad_values[0] / 4095);
  ad_answer[1] = (ad_values[1] / 4095);
  ad_answer[2] = (ad_values[2] / 4095);
  ad_answer[3] = (ad_values[3] / 4095);
  ad_answer[4] = (ad_values[4] / 4095);
  // ad_answer[5] = (ad_values[5] / 4095);
  ad_answer[0] *= 300;
  ad_answer[1] *= 300;
  ad_answer[2] *= 300;
  ad_answer[3] *= 300;
  ad_answer[4] *= 300;
  // ad_answer[5] *= 300;

  AD_diff = Q_rsqrt(ad_answer[0]) - Q_rsqrt(ad_answer[4]);
  AD_sum = ad_answer[0] + ad_answer[2] + ad_answer[4];
  // AD_level = fabs((ad_answer[0] - ad_answer[4])/(ad_answer[0] + ad_answer[4]))*100;
  AD_level = fabs(ad_answer[0] + ad_answer[4]);
  AD_level = Q_rsqrt(AD_level);
  AD_vertical = ad_answer[1] + ad_answer[3];
  AD_vertical = Q_rsqrt(AD_vertical) + Q_rsqrt(ad_answer[2]);
  AD_N = 2 * ad_answer[2] - ad_answer[0] - ad_answer[4];

  if (line_flag == 0 && Y_steer == 0)
  {
    if (AD_out_flag == 0)
    {
      AD_out_1 = (AD_diff) / (AD_sum) * (550 - ad_answer[2]);
      AD_out_flag = 1;
    }
    else if (AD_out_flag == 1)
    {
      AD_out_2 = (AD_diff) / (AD_sum) * (550 - ad_answer[2]);
      AD_out_flag = 0;
    }
    AD_out = 0.3 * AD_out_1 + 0.7 * AD_out_2;
    if (ad_answer[0] - ad_answer[2] >= 30 || ad_answer[4] - ad_answer[2] >= 30)
      AD_out = AD_out_2;
    else if (ad_answer[0] - ad_answer[2] >= 0 && ad_answer[0] - ad_answer[2] < 10 || ad_answer[4] - ad_answer[2] >= 0 && ad_answer[4] - ad_answer[2] < 10)
      AD_out = 0.25 * AD_out_1 + 0.75 * AD_out_2;
    else if (ad_answer[0] - ad_answer[2] >= 10 && ad_answer[0] - ad_answer[2] < 20 || ad_answer[4] - ad_answer[2] >= 10 && ad_answer[4] - ad_answer[2] < 20)
      AD_out = 0.2 * AD_out_1 + 0.8 * AD_out_2;
    else if (ad_answer[0] - ad_answer[2] >= 20 && ad_answer[0] - ad_answer[2] < 30 || ad_answer[4] - ad_answer[2] >= 20 && ad_answer[4] - ad_answer[2] < 30)
      AD_out = 0.1 * AD_out_1 + 0.9 * AD_out_2;
    //丢线处理
    if ((ad_answer[0] + ad_answer[2] + ad_answer[4]) < 120)
    {
      // expited_L_R = 150;
      if (AD_out <= 0)
        last_AD_out = -28;
      else if (AD_out > 0)
        last_AD_out = 28;
      line_flag = 1;
    }
  }
  if ((ad_answer[0] + ad_answer[2] + ad_answer[4]) >= 120)
  {
    line_flag = 0;
  }
  if (line_flag == 1)
  {
    AD_out = last_AD_out;
  }
  if (Y_steer == 1) //三岔AD处理
  {
    AD_out = (AD_diff) / (AD_sum)*280;
  }
  if (Y_steer == 0 && count_motor != 0)
  {
    if (AD_out <= 5 && AD_out >= -5 && (ad_answer[1] + ad_answer[3]) <= 100) //直道加速，弯道减速
    {
      expited_L_R += 1;
    }
    else
    {
      expited_L_R = SPEED_LOW_R;
    }
    if (expited_L_R >= SPEED_HIGH)
      expited_L_R = SPEED_LOW;
    if (ad_answer[0] - ad_answer[2] > 30 && ad_answer[0] - ad_answer[2] < 60 || ad_answer[4] - ad_answer[2] > 30 && ad_answer[4] - ad_answer[2] < 60 || line_flag == 1)
    {
      expited_L_R = SPEED_LOSS;
    }
  }
  //大湾减速

  //特殊元素检测
  // Cross();       //十字
  Slope_Process(); //坡道
  if (Slope_N == 0)
  {
    Y_deal(); //三叉
  }
}
// stop_1();   //停车
//  round_deal();  //环岛
/*******************************************************************************
 * function name:      Y_deal( )
 * description:    三叉处理
 * author:             ---
 * version:            v0.0.1
 * date：               2022.04.03
 * note:
 *******************************************************************************/

//三岔变量
int8 Y_steer = 0; //三岔标志位
#define NO_Y 0
#define Y_L_flag 1
#define Y_R_flag 2
#define Real_Out_Y 3
#define Out_Y 4
int8 Y_state = 0;      //三叉判断
int8 Y_flag = 0;       //左右三叉标志
uint32 Y_distante = 0; //积分距离

void Y_deal()
{
  switch (Y_state)
  {
  case NO_Y: //没有三叉时

    if (ad_answer[0] <= 120 && ad_answer[0] >= 50 && // 90 23 150 10 100 4 16
        ad_answer[4] <= 120 && ad_answer[4] >= 50 &&
        ad_answer[1] >= 0 && ad_answer[1] <= 30 &&
        ad_answer[3] >= 0 && ad_answer[3] <= 30 &&
        ad_answer[2] <= 160 && ad_answer[2] >= 70 &&
        AD_level > 10 && AD_level < 15 && AD_vertical > 10 &&
        AD_vertical < 18 && AD_N > 10 && AD_N < 40)
    {
      expited_L_R = SPEED_Y_IN; //调节速度期望
      P77 = 1;                  //蜂鸣器响
      Y_steer = 1;
      if (Y_flag == 1)
      {
        Y_state = Y_L_flag;
        Y_flag = 0;
      }
      else if (Y_flag == 0)
      {
        Y_state = Y_L_flag;
        Y_flag = 1;
      }
    }
    else
    {
      Y_steer = 0;
      Y_state = NO_Y;
    }

    break;
  case Y_L_flag:
    AD_out = 20;
    Y_distante += (R_value + L_value) / 2;
    if (Y_distante >= DISTANCE_Y_IN_L)
    {
      expited_L_R = SPEED_Y_L;
      Y_state = Real_Out_Y;
      Y_distante = 0;
      P77 = 0;
    }
    break;
  case Y_R_flag:
    AD_out = -20;
    Y_distante += (R_value + L_value) / 2;
    if (Y_distante >= DISTANCE_Y_IN_R)
    {
      Y_state = Real_Out_Y;
      expited_L_R = SPEED_Y_R;
      Y_distante = 0;
      P77 = 0;
    }
    break;
  case Real_Out_Y:
    Y_distante += (R_value + L_value) / 2;
    if (Y_distante >= DISTANCE_Y_OUT)
    {
      Y_state = NO_Y;
      Y_distante = 0;
      expited_L_R = SPEED_Y_OUT;
      Y_steer = 0;
    }
    break;
  }
}
/*******************************************************************************
 * function name:      round_deal( )
 * description:           环岛处理
 * author:                 ---
 * version:                 v0.0.1
 * date：                  2022.04.03
 * note:
 *******************************************************************************/
#define NO_round 0
#define in_round_flag 1
#define round_L 2
#define round_R 3
#define Real_Out_round 4

int8 round_state = 0;      //环岛状态判断
uint32 round_distante = 0; //环岛积分距离
float gyro_angle = 0;      //环岛陀螺仪角度积分
uint16 count_round = 0;    //环岛标志

void round_deal()
{
  switch (round_state)
  {
  case NO_round:
    if (ad_answer[0] >= 200 && ad_answer[4] >= 200 && ad_answer[2] >= 200 &&
        (ad_answer[1] + ad_answer[3]) >= 60) //环岛标志
    {
      // round_state = in_round_flag;
      P77 = 1; //蜂鸣器响
      ++count_round;
      if (count_round == 1)
      {
        round_state = Real_Out_round;
      }
      else if (count_round == 2)
      {
        count_round = 0;
        round_state = NO_round;
        P77 = 0;
      }
    }
    break;
  case in_round_flag:
    AD_out = (0.8 * ad_answer[1] - 0.8 * ad_answer[3]) / (0.2 * ad_answer[2] + 0.8 * ad_answer[1] + 0.8 * ad_answer[3] + 0.2 * ad_answer[0] + 0.2 * ad_answer[4]) * 40;
    round_distante += (R_value + L_value) / 2;
    if (round_distante >= DISTANCE_ROUND_IN) //入环岛积分距离
    {
      round_distante = 0;
      round_state = NO_round;
      P77 = 0;
    }
    break;
  case Real_Out_round:
    P77 = 1;
    round_distante += (R_value + L_value) / 2;
    if (round_distante >= DISTANCE_ROUND_OUT) //出环岛直线距离积分
    {
      round_state = in_round_flag;
      round_distante = 0;
    }
    break;
  }
}
/*******************************************************************************
 * function name:      Y_deal( )
 * description:    坡道检测
 * author:             ---
 * version:            v0.0.1
 * date：               2022.05.01
 * note:
 *******************************************************************************/
#define Slope_IN 1
#define Slope_out 2
#define no_Slope 0
int Slope_Flag = 0;
float Slope_Distante = 0;
float Gyro_Pitch_Angle = 0;
void Slope_Process(void)
{
  switch (Slope_Flag)
  {
  case no_Slope:
    if (gyro_y > 0.5 || gyro_y < -0.5)
      Gyro_Pitch_Angle += (gyro_y * 0.00225f);
    if (fabs(Gyro_Pitch_Angle) >= 3)
    {
      Slope_Flag = Slope_IN;
      Slope_N = 1;
      Gyro_Pitch_Angle = 0;
      // P77=1;
    }
    if (count_isr >= 500)
    {
      Gyro_Pitch_Angle = 0;
      count_isr = 0;
    }
    break;

  case Slope_IN:
    Slope_Distante += (L_value + R_value) / 2;
    if (Slope_Distante > DISTANCE_SLOPE)
    {
      Slope_Flag = no_Slope;
      Slope_N = 0;
      Slope_Distante = 0;
      // P77=0;
    }
    break;
  }
}

//停车检测
int ganhuangguan = 0;
#define stop_1_no 0
#define stop_1_int 1
#define stop_1_out 2
#define stop_1_L 3
#define stop_1_R 4
#define stop_1_stop 5
int8 P32_Status = 0;
int8 stop_1_flag;
int32 parking_distante = 0;
int Parking_Count = 0;
int8 parking_flag_L_R = 1; //区分左入库还是右入
int8 stop_flag = 0;
int16 stop_speed = 0;
void Parking_Fuction()
{
  switch (stop_1_flag)
  {
  case stop_1_no:
  {
    if (P32 == 1)
    {
      ganhuangguan = ganhuangguan + 1;
      stop_1_flag = stop_1_int;
      P77 = 1;
    }
    break;
  }
  case stop_1_int:
  {
    if (ganhuangguan < 2)
    {
      parking_distante += (L_value + R_value) / 2;
      if (parking_distante >= DISTANCE_STOP)
      {
        parking_distante = 0;
        stop_1_flag = stop_1_no;
        P77 = 0;
      }
    }
    if (ganhuangguan >= 2)
    {
      expited_L_R = 0;
      stop_flag = 1;
      if ((L_value + R_value) / 2 == 0)
      {
        stop_speed++;
      }
      if (stop_speed >= 100)
      {
        stop_1_flag = stop_1_out;
        stop_speed = 0;
      }
    }
    break;
  }

  case stop_1_out:
  {
    expited_L_R = +200;
    if ((L_value + R_value) / 2 > 0)
    {
      // AD_out = -AD_out;
      parking_distante += (L_value + R_value) / 2;
    }
    if (parking_distante >= 0)
    {
      expited_L_R = 0;
      if ((L_value + R_value) / 2 == 0)
      {
        stop_speed++;
      }
      if (stop_speed >= 100)
      {
        stop_speed = 0;

        if (parking_flag_L_R == 0)
        {
          parking_distante = 0;
          stop_1_flag = stop_1_L;
        }
        if (parking_flag_L_R == 1)
        {
          parking_distante = 0;
          stop_1_flag = stop_1_R;
        }
      }
    }
    break;
  }
  case stop_1_L:
  {
    expited_L_R = SPEED_INGARAGE;
    AD_out = -STEER_INGARAGE;
    if ((L_value + R_value) / 2 < 0)
      parking_distante += (L_value + R_value) / 2;
    if (parking_distante <= DISTANCE_INGARAGE)
    {
      //  parking_distante=0;
      expited_L_R = 0;
    }
    break;
  }
  case stop_1_R:
  {
    expited_L_R = SPEED_INGARAGE;
    AD_out = STEER_INGARAGE;
    if ((L_value + R_value) / 2 < 0)
      parking_distante += (L_value + R_value) / 2;
    if (parking_distante <= DISTANCE_INGARAGE)
    {
      //  parking_distante=0;
      expited_L_R = 0;
    }
    break;
  }
  }
}
//停车检测
/*int ganhuangguan = 0;
#define stop_1_no 0
#define stop_1_int 1
int8 P32_Status = 0;
int8 stop_1_flag;
int32 parking_distante = 0;
int Parking_Count = 0;
void Parking_Fuction()
{
  switch (stop_1_flag)
  {
  case stop_1_no:
    if (P32 == 1)
    {
      ganhuangguan = ganhuangguan + 1;
      stop_1_flag = stop_1_int;
      P77 = 1;
    }
    break;
  case stop_1_int:
    parking_distante += (L_value + R_value) / 2;
    if (parking_distante >= 5000)
    {
      parking_distante = 0;
      stop_1_flag = stop_1_no;
      P77 = 0;
    }
    break;
  }
  if (ganhuangguan >= 2)
  {
    expited_L_R = 0;
  }
}*/

/*十字检测*/
/*#define Cross_int 1
#define Cross_out 2
#define no_Cross 0
#define Cross_r_out 3
int Cross_Flag = 0;
int32 Cross_Distante = 0;
void Cross()
{
  switch (Cross_Flag)
  {
  case no_Cross:
    if (ad_answer[0] >= 90 && ad_answer[4] >= 90 && ad_answer[2] >= 100 && (ad_answer[1]+ad_answer[3] ) > 100 )
    {

      Cross_Flag = Cross_int;
      P77=1;
    }
    break;
  case Cross_int:
    Cross_Distante += (L_value + R_value) / 2;
    if (Cross_Distante > 25000)
    {
      Cross_Flag = Cross_out;
      Cross_Distante = 0;
    }
    break;
  case Cross_out:
    AD_out = (ad_answer[1] - ad_answer[3]) / (ad_answer[2] + ad_answer[1]) * 250;
    if (ad_answer[0] >= 90 && ad_answer[4] >= 90 && ad_answer[2] >= 100 && (ad_answer[1]+ad_answer[3] ) > 100 )
    {
      Cross_Flag = no_Cross;
      P77=0;
    }
    break;
  case Cross_r_out:
    // Cross_Distante += (L_value + R_value) / 2;
    if (ad_answer[0] >= 90 && ad_answer[4] >= 90 && ad_answer[2] >= 150 && ad_answer[1] > 90 && ad_answer[3] >= 90)
    {
      Cross_Flag = no_Cross;
      Cross_Distante = 0;
    }
    break;
  }
}*/
