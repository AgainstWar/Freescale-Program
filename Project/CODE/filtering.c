/*------------------------------
* @file filtering.c  滤波算法：1.加权递推平均滤波 2.算术平均滤波法
* @author Against_War
* @version 1.0.0
* @note  参考程序：https://github.com/leon0516/lv_bo_suan_fa.git
* @date 2020-2-24
-------------------------------*/
#include "filtering.h"
uint8 filter_value; //最终传输的值
float angle; //
/*卡尔曼滤波*/
int16 Kalman_left(int16 data_l)
{
  float kg,c2,p2;
  static int16 p1=6;
  static int16 c1=5;
  int16 q=5;
  int16 r=10;
  p2=p1+q;
  kg=p2*p2/(p2*p2+r*r);
  c2=c1+kg*(data_l-c1);
  p1=(1-kg)/p2;
  c1=c2;

  return c1; 
}
int16 Kalman_right(int16 data_r)
{
    float kg,c2,p2;
  static int16 p1=6;
  static int16 c1=5;
  int16 q=5;
  int16 r=10;
  p2=p1+q;
  kg=p2*p2/(p2*p2+r*r);
  c2=c1+kg*(data_r-c1);
  p1=(1-kg)/p2;
  c1=c2;

  return c1; 
}