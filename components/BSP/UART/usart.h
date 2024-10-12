#ifndef __USART_H_
#define __USART_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include <string.h>
#include "driver/gpio.h"

#define UART_NUM            UART_NUM_2             // 使用 UART2
#define TXD_PIN             (GPIO_NUM_6)           // UART2 TX 引脚
#define RXD_PIN             (GPIO_NUM_7)           // UART2 RX 引脚
#define BUF_SIZE            1024                   // 接收缓冲区大小
#define TIMEOUT_MS          100


typedef struct
{
    uint8_t uart_revbuf[BUF_SIZE];
    uint16_t pr_w;
    uint16_t pr_r;
}Rev_uart_t;

extern Rev_uart_t Rev_uart;

extern void uart_init(void);
#endif