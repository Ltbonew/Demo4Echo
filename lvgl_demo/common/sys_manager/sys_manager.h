#ifndef _SYS_MANAGER_H
#define _SYS_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../conf/dev_conf.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char city[36];
    char adcode[16]; // 'gao de' Amap adcode
} LocationInfo_t;

typedef struct {
    int year;                       // system time year             (need store)
    int month;                      // system time month            (need store)
    int day;                        // system time day              (need store)
    int hour;                       // system time hour             (need store)
    int minute;                     // system time minute           (need store)
    uint16_t brightness;            // system brightness            (need store)
    uint16_t sound;                 // system sound volume(0-30)    (need store)
    bool wifi_connected;            // wifi connected or not
    bool auto_time;                 // auto update time or not      (need store)
    bool auto_location;             // auto update location or not  (need store)
    LocationInfo_t location;        // location info                (need store)
}system_para_t;

// 设置LCD背光亮度（0.0到1.0之间）
int sys_set_lcd_backlight_brightness(float brightness);

// 获取LCD背光亮度
float sys_get_lcd_backlight_brightness(void);

// 设置系统音量（0到100之间）
int sys_set_volume_level(int level);

// 获取系统音量
int sys_get_volume_level(void);

// 设置系统时间
int sys_set_system_time(int year, int month, int day, int hour, int minute, int second);

// 获取系统时间
void sys_get_system_time(int *year, int *month, int *day, int *hour, int *minute, int *second);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif