#include "ui_HomePage.h"
#include <time.h>

void get_current_time(uint8_t *hour, uint8_t *minute) {
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

void get_current_date(int *year, int *month, int *day) {
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