#ifndef __KEY_H_
#define __KEY_H_

#include "esp_err.h"
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "led.h"


/* 引脚定义 */
#define BOOT_INT_GPIO_PIN   GPIO_NUM_0

/*IO操作*/
#define BOOT                gpio_get_level(BOOT_INT_GPIO_PIN)

/* 函数声明 */
void key_init(void);   /* 外部中断初始化程序 */

#endif