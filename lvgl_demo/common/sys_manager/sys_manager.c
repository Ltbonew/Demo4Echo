#include "sys_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h> // For setting system time

// 假设的存储变量，实际应用中应替换为与硬件交互的代码
static int pwm_frequency = 5000; // 默认PWM频率为5kHz
static float lcd_backlight_brightness = 0.5f; // 默认背光亮度为50%
static int volume_level = 75; // 默认音量水平为75%

int sys_set_pwm_frequency(int frequency_hz) {
    if (frequency_hz <= 0) return -1; // 频率必须大于0
    pwm_frequency = frequency_hz;
    // 这里可以添加实际设置硬件PWM频率的代码
    return 0;
}

int sys_get_pwm_frequency(void) {
    return pwm_frequency;
}

int sys_set_lcd_backlight_brightness(float brightness) {
    if (brightness < 0.0f || brightness > 1.0f) return -1; // 亮度范围应在0.0到1.0之间
    lcd_backlight_brightness = brightness;
    // 这里可以添加实际设置硬件背光亮度的代码
    return 0;
}

float sys_get_lcd_backlight_brightness(void) {
    return lcd_backlight_brightness;
}

int sys_set_volume_level(int level) {
    if (level < 0 || level > 100) return -1; // 音量级别应在0到100之间
    volume_level = level;
    // 这里可以添加实际设置硬件音量的代码
    return 0;
}

int sys_get_volume_level(void) {
    return volume_level;
}

int sys_set_system_time(int year, int month, int day, int hour, int minute, int second) {
    struct tm t = {0};
    t.tm_year = year - 1900; // 年份从1900年起计算
    t.tm_mon = month - 1;    // 月份从0起计算
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    

    return 0;
}

void sys_get_system_time(int *year, int *month, int *day, int *hour, int *minute, int *second) {
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    if (year) *year = timeinfo->tm_year + 1900;
    if (month) *month = timeinfo->tm_mon + 1;
    if (day) *day = timeinfo->tm_mday;
    if (hour) *hour = timeinfo->tm_hour;
    if (minute) *minute = timeinfo->tm_min;
    if (second) *second = timeinfo->tm_sec;
}

// 使用蔡勒公式计算星期几，0代表周日，1代表周一，...，6代表周六
int sys_get_day_of_week(int year, int month, int day) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }

    int K = year % 100;
    int J = year / 100;

    // 蔡勒公式
    int f = day + ((13 * (month + 1)) / 5) + K + (K / 4) + (J / 4) + (5 * J);
    int dayOfWeek = f % 7;

    // 根据蔡勒公式的定义调整返回值以匹配常见的一周起始日(0=周日, 1=周一, ..., 6=周六)
    return (dayOfWeek + 1) % 7;
}
