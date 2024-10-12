
#include "usart.h"

Rev_uart_t Rev_uart;
static const char *TAG = "UART";
static QueueHandle_t uart_queue;
void LOG_HEXDUMP_PRINT(char*str, uint8_t *data, uint16_t len)
{
    printf("%s\n",str);
    for(uint16_t i = 0; i < len;i++)
    {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

void uart_receive_task(void *arg)
{
    uint8_t data[BUF_SIZE];
    TickType_t last_receive_time = 0;  // 记录最后一次接收到数据的时间

    while (1)
    {
        // 等待 UART 队列中的事件
        uart_event_t event;
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY))
        {
            ESP_LOGI(TAG, "Event received: %d", event.type);  // 打印接收到的事件类型

            switch (event.type)
            {
                case UART_DATA:  // 接收到数据
                    uart_read_bytes(UART_NUM, data, event.size, portMAX_DELAY);
                    memcpy(&Rev_uart.uart_revbuf[Rev_uart.pr_w], data, event.size);  // 复制数据
                    Rev_uart.pr_w = (Rev_uart.pr_w + event.size) % BUF_SIZE; // 确保索引不越界
                    // LOG_HEXDUMP_PRINT("REV DATA:",&Rev_uart.uart_revbuf[Rev_uart.pr_w],event.size);
                    LOG_HEXDUMP_PRINT("REV DATA:", data, event.size);

                    break;

                // UART FIFO 缓冲区溢出事件
                case UART_FIFO_OVF:
                    ESP_LOGW(TAG, "Hardware FIFO Overflow");
                    uart_flush_input(UART_NUM);
                    xQueueReset(uart_queue);
                    break;

                // UART 缓冲区满事件
                case UART_BUFFER_FULL:
                    ESP_LOGW(TAG, "Ring Buffer Full");
                    uart_flush_input(UART_NUM);
                    xQueueReset(uart_queue);
                    break;

                // UART 帧错误事件
                case UART_FRAME_ERR:
                    ESP_LOGW(TAG, "UART Frame Error");
                    break;

                // UART 奇偶校验错误事件
                case UART_PARITY_ERR:
                    ESP_LOGW(TAG, "UART Parity Error");
                    break;

                default:
                    ESP_LOGW(TAG, "Unknown event type: %d", event.type);  // 打印未知事件类型
                    break;
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);  // 稍微延时，避免占用过多 CPU
    }
}


void uart_init()
{
    // 配置 UART 参数
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // 初始化 UART
    uart_param_config(UART_NUM, &uart_config);

    // 设置 UART 引脚 (根据实际硬件配置)
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // 安装 UART 驱动程序
    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart_queue, 0);

    ESP_LOGI(TAG, "UART init finished!");

    // 创建处理 UART 事件的任务
    xTaskCreate(uart_receive_task, "uart_receive_task", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "uart_receive_task create finished!");
}




