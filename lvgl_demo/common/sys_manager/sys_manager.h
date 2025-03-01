#ifndef _SYS_MANAGER_H
#define _SYS_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../conf/dev_conf.h"

// 设置PWM频率（单位: Hz）
int sys_set_pwm_frequency(int frequency_hz);

// 获取PWM频率（单位: Hz）
int sys_get_pwm_frequency(void);

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