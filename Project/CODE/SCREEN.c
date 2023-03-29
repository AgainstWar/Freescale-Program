#include "SCREEN.h"
/*------------------------------
* @brief  display_data() 屏幕显示函数
* @param  void
* @return void
* @author Against_War
* @date 2020/03/08
* @note 数据显示示例（排布数据时请参考以下坐标轴）
-------------------------------*/
// float angle = 0;
void display_data()
{

    // get_icm20602_gyro_simiic();
    oled_fill(0);
     /*oled_printf_float(0, 5, num_kp,3,2);
     oled_printf_float(0, 1, coefficient,3,3);
    oled_printf_float(0, 2, num_kd,3,2);*/
    
   /*oled_int16(0, 0, R_value);
    oled_int16(0, 1, L_value);*/
    oled_int16(0, 2, expited_L_R); 
    //oled_int16(0, 3, ganhuangguan);

    //angle += gyro_z * 0.00225f;
    //oled_printf_float(0, 1, gyro_x, 3, 2);
    //oled_printf_float(0, 2, gyro_z_num, 3, 2);
    /*oled_printf_float(0, 0, gyro_x, 3, 2);
    oled_printf_float(0, 2, gyro_y, 3, 2);
    oled_printf_float(0, 3, gyro_z, 3, 2);*/
    /*oled_printf_float(0, 4, acc_x, 3, 2);
    oled_printf_float(0, 5, acc_y, 3, 2);
    oled_printf_float(0, 6, acc_z, 3, 2);*/
    // oled_printf_float(0, 6, angle, 3, 2);

    //oled_printf_float(2, 4, angle, 3, 2);
   /*oled_printf_float(64 ,0, ad_answer[0], 3, 2);   
     oled_printf_float(64, 1, ad_answer[1], 3, 2);
     oled_printf_float(64, 2, ad_answer[2], 3, 2);
     oled_printf_float(64, 3, ad_answer[3], 3, 2);
     oled_printf_float(64, 4, ad_answer[4], 3, 2);
     //oled_printf_float(64, 5, ad_anwer[5], 3, 2);
     oled_printf_float(64, 5,  AD_out, 3, 2);*/
    /* oled_printf_float(64, 5,  AD_level, 3, 2);
     oled_printf_float(64, 6,  AD_vertical, 3, 2);*/
    oled_printf_int32(64, 1, count_encode_R,10);
     oled_printf_int32(64, 2, count_encode_L,10);
    //  oled_printf_float(0, 1,  AD_out, 3, 2);

    //  oled_printf_float(64, 2, num_kp, 2, 2);
    /* oled_printf_float(64, 4, R_Kp, 2, 3);
     oled_printf_float(64, 5, R_Ki, 2, 3);*/
    /*oled_int16(4, 0, gyro_x); //陀螺仪角度显示
    oled_int16(4, 4, gyro_y);
    oled_int16(4, 6, gyro_z);
    oled_int16(64, 0, acc_x); //陀螺仪加速度显示
    oled_int16(64, 4, acc_y);
    oled_int16(64, 6, acc_z);*/
}