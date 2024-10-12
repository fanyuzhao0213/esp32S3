#ifndef __MQTT_H
#define __MQTT_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "wifi_smartconfig.h"

extern void mqtt_start(void);
extern void mqtt_pub_task(void *pvParameters);

#endif


