#ifndef __WEATHER_H_
#define __WEATHER_H_

//更改更新weather的时间   默认为10min
#define WEATHER_UPDATE_TIME                 60*1           //单位1S
extern void http_client_task(void *pvParameters);

#endif