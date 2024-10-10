/**
 ******************************************************************************
 * @file     main.c
 * @version  V1.0
 * @date     2023-08-26
 * @brief    ESP32-IDF基础工程
 ******************************************************************************
 * @attention
 ******************************************************************************
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_psram.h"
#include "esp_flash.h"
#include "led.h"
#include "key.h"
#include "esptim.h"
#include "gptim.h"
#include "wdt.h"
#include "pwm.h"
#include "iic.h"
#include "simple_wifi_sta.h"

uint8_t dir = 1;
uint16_t ledpwmval = 0;

void pwm_test(void)
{
    if (dir == 1)
    {
        ledpwmval += 5; /* dir==1 ledpwmval递增 */
    }
    else
    {
        ledpwmval -= 5; /* dir==0 ledpwmval递减 */
    }

    if (ledpwmval > 1005)
    {
        dir = 0;        /* ledpwmval到达1005后，方向为递减 */
    }

    if (ledpwmval < 5)
    {
        dir = 1;        /* ledpwmval递减到5后，方向改为递增 */
    }
    /* 设置占空比 */
    // ESP_LOGI("PWM", "ledpwmval:%d",ledpwmval);
    pwm_set_duty(ledpwmval);
}

void watch_dog_feed(void)
{
    gptimer_event_t g_tim_evente;
    if (xQueueReceive(queue, &g_tim_evente, 2000))
    {
        restart_timer(1000000);     /* 喂狗 */
        ESP_LOGI("GPTIMER_ALARM", "定时器报警, 计数值： %llu", g_tim_evente.event_count);   /* 打印通用定时器发生一次计数事件后获取到的值 */
    }
    else
    {
        ESP_LOGI("GPTIMER_ALARM", "计数值： %llu", g_tim_evente.event_count);
        ESP_LOGW("GPTIMER_ALARM", "错过一次计数事件");
    }
}

/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    esp_err_t ret;
    uint32_t flash_size;
    esp_chip_info_t chip_info;                                      /* 定义芯片信息结构体变量 */

    ret = nvs_flash_init();                                         /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    esp_flash_get_size(NULL, &flash_size);                          /* 获取FLASH大小 */

    esp_chip_info(&chip_info);
    printf("内核:cup数量%d\n",chip_info.cores);                     /* 获取CPU内核数并显示 */
    printf("FLASH size:%ld MB flash\n",flash_size / (1024 * 1024)); /* 获取FLASH大小并显示 */
    printf("PSRAM size: %d bytes\n", esp_psram_get_size());         /* 获取PARAM大小并显示 */

    /*BSP相关初始化*/
    // led1_init();                                                     /* 初始化LED */
    led2_init();                                                    /* 初始化LED */
    // i2c0_master = iic_init(I2C_NUM_0);                              /* 初始化IIC0 */
    // key_init();                                                   /* 初始化key */
    esptim_int_init(500000);                                          /*  初始化高分辨率定时器，此处设置定时器周期为1秒，
                                                                        但该函数事宜微妙为单位进行计算，
                                                                        故而1秒钟换算为1000000微秒 */

    gptim_int_init(100, 1000000);                                   /* 初始化通用定时器 */
    // wdt_init(5000, 2000000);                                        /* 初始化看门狗相关的定时器 */

    pwm_init(10,1000);


    wifi_sta_init();                        //wifi STA工作模式初始化
    while (1)
    {
        // watch_dog_feed();
        pwm_test();
        vTaskDelay(10);
    }
    vQueueDelete(queue);
}
