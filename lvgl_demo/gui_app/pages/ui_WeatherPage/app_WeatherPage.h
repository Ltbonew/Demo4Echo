#ifndef _APP_WEATHERPAGE_H
#define _APP_WEATHERPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui_WeatherPage.h"
#include <stdint.h>
#include <stddef.h>
#include <curl/curl.h>
#include <json-c/json.h>

typedef struct {
    char weather[32];
    char temperature[16];
    char humidity[16];
    char windpower[16];
} WeatherInfo_t;

/**
 * @brief 根据IP地址获取地理位置信息，并填充城市名称和adcode。
 *
 * @param location 用于存储位置信息的结构体。
 * @return -1: fail, 0: ok
 */
int get_location_info(LocationInfo_t* location);

/**
 * @brief 根据adcode获取天气信息，并填充WeatherInfo结构体。
 *
 * @param adcode 城市adcode。
 * @param weather_info 用于存储天气信息的结构体。
 * @return 0: success, -1: fail
 */
int get_weather_info_by_adcode(const char* adcode, WeatherInfo_t* weather_info);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif