#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "weather.h"
#include "wifi_smartconfig.h"

static const char *HTTP_TAG = "Weather";
#define MAX_HTTP_OUTPUT_BUFFER 1300
#define HOST "api.seniverse.com"
#define UserKey "SNJN8m1O95pkg2cEA"
#define Location "Xian"
#define Language "zh-Hans"
#define Strat "-1"
#define Days "5"

static void freeBuffer(char* buf, int bufSize)
{
	for( int i = 0; i < bufSize; i ++ )
		buf[i] = NULL;
}

void parse_weather_data(const char *json_data)
{
    cJSON *root = cJSON_Parse(json_data);
    if (root == NULL) {
        ESP_LOGE(HTTP_TAG, "Error parsing JSON");
        return;
    }

    // 获取结果数组
    cJSON *results = cJSON_GetObjectItem(root, "results");
    if (cJSON_IsArray(results)) {
        int results_size = cJSON_GetArraySize(results);
        for (int i = 0; i < results_size; i++) {
            cJSON *result = cJSON_GetArrayItem(results, i);

            // 获取地点信息
            cJSON *location = cJSON_GetObjectItem(result, "location");
            if (location) {
                const char *name = cJSON_GetObjectItem(location, "name")->valuestring;
                ESP_LOGI(HTTP_TAG, "Location: %s", name);
            }

            // 获取每日天气预报
            cJSON *daily = cJSON_GetObjectItem(result, "daily");
            if (cJSON_IsArray(daily)) {
                int daily_size = cJSON_GetArraySize(daily);
                for (int j = 0; j < daily_size; j++) {
                    cJSON *day = cJSON_GetArrayItem(daily, j);

                    const char *date = cJSON_GetObjectItem(day, "date")->valuestring;
                    const char *text_day = cJSON_GetObjectItem(day, "text_day")->valuestring;
                    const char *high = cJSON_GetObjectItem(day, "high")->valuestring;
                    const char *low = cJSON_GetObjectItem(day, "low")->valuestring;
                    const char *rainfall = cJSON_GetObjectItem(day, "rainfall")->valuestring;
                    const char *humidity = cJSON_GetObjectItem(day, "humidity")->valuestring;
                    const char *wind_speed = cJSON_GetObjectItem(day, "wind_speed")->valuestring;
                    const char *wind_direction = cJSON_GetObjectItem(day, "wind_direction")->valuestring;

                    // 打印所有天气信息
                    ESP_LOGI(HTTP_TAG, "Date: %s, Text Day: %s, High: %s, Low: %s, Rainfall: %s, Humidity: %s, Wind Speed: %s, Wind Direction: %s",
                             date, text_day, high, low, rainfall, humidity, wind_speed, wind_direction);
                }
            }
        }
    }

    cJSON_Delete(root); // 释放内存
}


/** HTTP functions **/
void http_client_task(void *pvParameters)
{
    static uint16_t timecount =0;
    static uint16_t weather_update_flag = 1;
    int retry_count = 0;
    esp_err_t err;

    while(1)
    {
        if(weather_update_flag == 1)
        {
            weather_update_flag = 0;
            char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};   // Buffer to store response of http request
            int content_length = 0;
            static const char *URL = "http://"HOST"/v3/weather/daily.json?"	\
                                    "key="UserKey"&location="Location		\
                                    "&language="Language					\
                                    "&unit=c&start="Strat"&days="Days;
            esp_http_client_config_t config = {
                .url = URL,
            };
            esp_http_client_handle_t client = esp_http_client_init(&config);

            // GET Request
            esp_http_client_set_method(client, HTTP_METHOD_GET);

            //增加重试机制
            while (retry_count < 3) {
                err = esp_http_client_open(client, 0);
                if (err == ESP_OK)
                {
                    break;  // 请求成功，跳出重试循环
                }
                else
                {
                    retry_count++;
                    ESP_LOGE(HTTP_TAG, "Retrying... attempt %d", retry_count);
                    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 等待 1 秒后重试
                }
            }

            if (err != ESP_OK) {
                ESP_LOGE(HTTP_TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
            } else {
                content_length = esp_http_client_fetch_headers(client);
                if (content_length < 0) {
                    ESP_LOGE(HTTP_TAG, "HTTP client fetch headers failed");
                } else {
                    int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
                    if (data_read >= 0) {
                        ESP_LOGI(HTTP_TAG, "HTTP GET Status = %d, content_length = %d",
                        esp_http_client_get_status_code(client),
                        esp_http_client_get_content_length(client));
                        printf("data:%s", output_buffer);
                        parse_weather_data(output_buffer);
                    } else {
                        ESP_LOGE(HTTP_TAG, "Failed to read response");
                    }
                }
            }
            esp_http_client_close(client);
            obtain_time();
        }
        timecount++;
        if(timecount >= WEATHER_UPDATE_TIME)
        {
            timecount = 0;
            weather_update_flag = 1;
        }
        vTaskDelay(1000);       //1S
    }
	vTaskDelete(NULL);
}
