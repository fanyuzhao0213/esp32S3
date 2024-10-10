/**
 ****************************************************************************************************
 * @file        led.c
 * @version     V1.0
 * @date        2023-08-26
 * @brief       PWM驱动代码
 ****************************************************************************************************
 * @attention
 ****************************************************************************************************
 */

#include "pwm.h"


/**
 * @brief       初始化PWM
 * @param       resolution： PWM占空比分辨率
 *              freq： PWM信号频率
 * @retval      无
 */
void pwm_init(uint8_t resolution, uint16_t freq)
{
    ledc_timer_config_t ledc_timer;                 /* LEDC定时器句柄 */
    ledc_channel_config_t ledc_channel;             /* LEDC通道配置句柄 */

    /* 配置LEDC定时器 */
    ledc_timer.duty_resolution = resolution;        /* PWM占空比分辨率 */
    ledc_timer.freq_hz = freq;                      /* PWM信号频率 */
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;    /* 定时器模式 */
    ledc_timer.timer_num = LEDC_PWM_TIMER;          /* 定时器序号 */
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;             /* LEDC时钟源 */
    ledc_timer_config(&ledc_timer);                 /* 配置定时器 */

    /* 配置LEDC通道 */
    ledc_channel.gpio_num = LEDC_PWM_CH0_GPIO;      /* LED控制器通道对应引脚 */
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;  /* LEDC高速模式 */
    ledc_channel.channel = LEDC_PWM_CH0_CHANNEL;    /* LEDC控制器通道号 */
    ledc_channel.intr_type = LEDC_INTR_DISABLE;     /* LEDC失能中断 */
    ledc_channel.timer_sel = LEDC_PWM_TIMER;        /* 定时器序号 */
    ledc_channel.duty = 0;                          /* 占空比值 */
    ledc_channel_config(&ledc_channel);             /* 配置LEDC通道 */
}

/**
 * @brief       PWM占空比设置
 * @param       duty：PWM占空比
 * @retval      无
 */
void pwm_set_duty(uint16_t duty)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_PWM_CH0_CHANNEL, duty); /* 设置占空比 */
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_PWM_CH0_CHANNEL);    /* 更新占空比 */
}
