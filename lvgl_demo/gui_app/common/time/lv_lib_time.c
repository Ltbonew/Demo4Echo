#include "lv_lib_time.h"
#include <time.h>

void lv_lib_get_time(uint8_t *hour, uint8_t *minute) {
    time_t rawtime;
    struct tm * timeinfo;

    // 获取当前的calendar time
    time(&rawtime);

    // 将calendar time转换为本地时间
    timeinfo = localtime(&rawtime);

    // 提取小时和分钟
    *hour = (uint8_t)timeinfo->tm_hour;
    *minute = (uint8_t)timeinfo->tm_min;
}

void lv_lib_get_date(int *year, int *month, int *day) {
    time_t rawtime;
    struct tm * timeinfo;

    // 获取当前的日历时间
    time(&rawtime);

    // 将日历时间转换为本地时间
    timeinfo = localtime(&rawtime);

    // 提取年、月、日
    if (year) *year = 1900 + timeinfo->tm_year; // tm_year是从1900年起的年数
    if (month) *month = timeinfo->tm_mon + 1;   // tm_mon是从0开始的月份
    if (day) *day = timeinfo->tm_mday;         // tm_mday是当月的第几天
}

int lv_lib_calculate_day_of_week(int year, int month, int day) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }

    int K = year % 100;
    int J = year / 100;

    // 蔡勒公式
    int f = day + ((13 * (month + 1)) / 5) + K + (K / 4) + (J / 4) + (5 * J);
    int dayOfWeek = f % 7;
    // 使用蔡勒公式计算星期几，0代表周日，1代表周一，...，6代表周六
    return (dayOfWeek + 1) % 7;
}