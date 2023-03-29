/*----------------------------------------------------------------------------
    * @file 软件滤波
    * @author Against_War
    * @version 1.0.0
    * @date 2022-2-24
    * @note 参考程序：https://github.com/leon0516/lv_bo_suan_fa.git
----------------------------------------------------------------------------*/
#ifndef __FILTERING_H_
#define __FILTERING_H_
#include "headfile.h"
int16 Kalman_left(int16 data_l);
int16 Kalman_right(int16 data_r);
#endif
