#include "debug.h"
#include "math.h"
int16 date_l = 0;
int16 date_r = 0;
float gyro_x, gyro_y, gyro_z; //陀螺仪角速度变量
float acc_x, acc_y, acc_z;    //加速度计变量
int16 limit_max = 200;
int16 limit_min = -200;
/*--------------------------------
 * @brief  gyro_data_get
 * @param  void
 * @return void
 * @author Against_War
 * @date 202./04/03
 * @note 获取陀螺仪数据并滤波
 *--------------------------------*/
void gyro_data_get()
{
    static float gyro_x_last = 0.0;
    static float gyro_y_last = 0.0;
    static float gyro_z_last = 0.0;
    float acc_angle_x, acc_angle_y, acc_angle_z;
    get_icm20602_accdata_simiic();
    get_icm20602_gyro_simiic();
    gyro_x = icm_gyro_x / 16.4; //转换为度/秒 详情参阅SEEKFREE_ICM20602.c\get_icm20602_gyro_simiic()下的注释
    gyro_y = icm_gyro_y / 16.4;
    gyro_z = icm_gyro_z / 16.4;
    acc_x = icm_acc_x / 4096; //转换为g(m/s2)
    acc_y = icm_acc_y / 4096;
    acc_z = icm_acc_z / 4096;
    gyro_x = gyro_x_last * 0.1 + gyro_x * 0.9;
    gyro_y = gyro_y_last * 0.1 + gyro_y * 0.9;
    gyro_z = gyro_z_last * 0.1 + gyro_z * 0.9;
    if (gyro_z > limit_max)
    {
        gyro_z = limit_max;
    }
    else if (gyro_z < limit_min)
    {
        gyro_z = limit_min;
    }
    if (gyro_y > limit_max)
    {
        gyro_y = limit_max;
    }
    else if (gyro_y < limit_min)
    {
        gyro_y = limit_min;
    }
    gyro_x_last = gyro_x;
    gyro_z_last = gyro_z;
    gyro_y_last = gyro_y;
    // First_order_Filter(acc_y,gyro_y,0.02);
}
/*----------------------------------------------------------------------------
    @brief gyro_zero_process()
    @param  void
    @return void
    @author Against_War
    @date   2022/04/03
    @note   ICM20602陀螺仪零漂处理
    @Sample usage: gyro_zero_process();
---------------------------------------------------------------------------*/
void gyro_zero_process()
{
    // get_icm20602_accdata_simiic();
    uint8 zero_flag = 1;
    int32 z_zero;
    int32 y_zero;
    uint8 i = 0;
    vuint8 z_zero_Low = 0, z_zero_High = 0, z_zero_finish = 0, y_zero_Low = 0, y_zero_High = 0, y_zero_finish = 0;
    icm20602_init_simspi();     //陀螺仪初始化
    get_icm20602_gyro_simspi(); //获取陀螺仪数据
    if (zero_flag)
    {
        if (i < 200)
        {
            z_zero += icm_gyro_z;
            y_zero += icm_gyro_y;
            i++;
        }
        else
        {
            //这里非常奇怪，只有累加的值除次数的1/2陀螺仪读取值才正常
            z_zero_finish = -(int16)(z_zero / 100);
            z_zero_Low = (uint8)z_zero_finish;
            z_zero_High = (uint8)(z_zero_finish >> 8);
            icm_simspi_w_reg_byte(ICM20602_ZG_OFFS_USRL, z_zero_High);
            icm_simspi_w_reg_byte(ICM20602_ZG_OFFS_USRH, z_zero_Low);
            y_zero_finish = -(int16)(y_zero / 100);
            y_zero_Low = (uint8)y_zero_finish;
            y_zero_High = (uint8)(y_zero_finish >> 8);
            icm_simspi_w_reg_byte(ICM20602_ZG_OFFS_USRL, y_zero_High);
            icm_simspi_w_reg_byte(ICM20602_ZG_OFFS_USRH, y_zero_Low);
            zero_flag = 0; //陀螺仪零漂处理标志位清零
        }
    }
    else
    {
    }
}
/*----------------------------------------------------------------
 * @brief 编码器数据采集
 * @return void
 * @author ***
 * @date 2022/03/08
 * @return  返回采集的值
 * @samples DATE_L_get（）采集左数值   DATE_R_get（） 采集右数值
 *----------------------------------------------------------------*/
int16 DATE_L_get()
{
    ENCODE_DIR_L = DIR_L;
    if (ENCODE_DIR_L == 0)
    {
        date_l = -1 * ctimer_count_read(ENCODE_L);
    }
    else
    {
        date_l = ctimer_count_read(ENCODE_L);
    }
    ctimer_count_clean(ENCODE_L); //清除编码器计数器计数
    return date_l;
}
int16 DATE_R_get()
{
    ENCODE_DIR_R = DIR_R;
    if (ENCODE_DIR_R == 0)
    {
        date_r = ctimer_count_read(ENCODE_R);
    }
    else
    {
        date_r = -1 * ctimer_count_read(ENCODE_R);
    }
    ctimer_count_clean(ENCODE_R); //清除编码器计数器计数
    return date_r;
}