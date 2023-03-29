#include "init.h"
#define BEEP P77 //蜂鸣器
#define init_freq 16000
#define init_duty 2500
void beep(uint8 n)
{
    uint8 j;
    BEEP = 0; //蜂鸣器置低
    for (j = 0; j < n; j++)
    {
        BEEP = 1;
        delay_ms(10);
        BEEP = 0;
        delay_ms(200);
    }
}
/*-----------------------------
 * @brief  系统初始化
 * @param  void
 * @retval void
 * @author AgainstWar
 * @date   2022/02/27
 * @Sample usage: system_init();
 *----------------------------*/
void system_init()
{
    int i;
    gpio_mode(P7_7, GPO_PP); //设置蜂鸣器为推挽
    gpio_mode(P3_2, GPIO);
    P32 = 0;
    BEEP = 0; //蜂鸣器置低
    // ADC管脚初始化
    adc_init(ADC_P00, ADC_12BIT);  //左1
    adc_init(ADC_P01, ADC_12BIT);  //左中
    adc_init(ADC_P05, ADC_12BIT);  //中
    adc_init(ADC_P06, ADC_12BIT);  //右中
    adc_init(ADC_P13, ADC_12BIT);  //右1
    gyro_zero_process();           //陀螺仪
    pwm_init(PWM_L, init_freq, 0); //初始化L电机PWM f:16000Hz,duty:0%
    pwm_init(PWM_R, init_freq, 0); //初始化R电机PWM f:16000Hz,duty:0%
    gpio_pull_set(P6_6, PULLUP);
    DIR_R = 0;
    gpio_pull_set(P6_0, PULLUP);
    DIR_L = 0;                    //电机方向初始化
    pwm_init(pwm_steer, 50, 750); //初始化舵机 f:50HZ duty7.5%
    pit_timer_ms(TIM_4, 4);       //初始化定时器
    ctimer_count_init(CTIM3_P04);
    ctimer_count_init(CTIM0_P34); //编码器初始化

    //    seekfree_wireless_init();//串口
    uart_init(UART_4, UART4_RX_P02, UART4_TX_P03, 115200, TIM_2);
    oled_init();
    oled_fill(0x00);
    oled_p8x16str(32, 0, "SYS_CHECK"); //屏幕显示
}
/*-----------------------------
 *  @brief 舵机测试函数
 *  @param  void
 *  @return void
 * @author ***
 * @date 2022/03/01
 * @Sample usage: steer_test()；
 *----------------------------*/
void steer_test()
{
    int8 i = 0;

    while (i < 10)
    {
        i++;
        delay_ms(20);
        pwm_duty(pwm_steer, pwm_duty_center + i * 5);
    }
    i = 0;
    while (i < 20)
    {
        i++;
        delay_ms(20);
        pwm_duty(pwm_steer, pwm_duty_center + 50 - i * 5);
    }
    i = 0;
    while (i < 10)
    {
        i++;
        delay_ms(20);
        pwm_duty(pwm_steer, pwm_duty_center - 50 + i * 5);
    }
    pwm_duty(pwm_steer, pwm_duty_center);
}
