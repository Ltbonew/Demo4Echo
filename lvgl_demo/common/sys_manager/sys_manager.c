#include "sys_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h> // For setting system time

const char * sys_config_path = "./system_para.conf"; // 系统参数配置文件路径与可执行文件同目录

int sys_set_lcd_brightness(int brightness) {
    if (brightness < 0 || brightness > 100) return -1;
    // 这里可以添加实际设置硬件背光亮度的代码

    return 0;
}

int sys_get_lcd_brightness(void) {

    return 50;
}

int sys_set_volume(int level) {
    if (level < 0 || level > 100) return -1; // 音量级别应在0到100之间
    // 这里可以添加实际设置硬件音量的代码

    return 0;
}

int sys_get_volume(void) {

    return 50;
}

int sys_set_time(int year, int month, int day, int hour, int minute, int second) {
    struct tm t = {0};
    t.tm_year = year - 1900; // 年份从1900年起计算
    t.tm_mon = month - 1;    // 月份从0起计算
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    

    return 0;
}

void sys_get_time(int *year, int *month, int *day, int *hour, int *minute, int *second) {
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

bool sys_get_wifi_status(void)
{
    return true;
}

int sys_save_system_parameters(const char *filepath, const system_para_t *params) {
    FILE *file = fopen(filepath, "w");
    if (!file) return -1;

    fprintf(file, "year=%d\n", params->year);
    fprintf(file, "month=%d\n", params->month);
    fprintf(file, "day=%d\n", params->day);
    fprintf(file, "hour=%u\n", params->hour);
    fprintf(file, "minute=%u\n", params->minute);
    fprintf(file, "brightness=%u\n", params->brightness);
    fprintf(file, "sound=%u\n", params->sound);
    fprintf(file, "wifi_connected=%s\n", params->wifi_connected ? "true" : "false");
    fprintf(file, "auto_time=%s\n", params->auto_time ? "true" : "false");
    fprintf(file, "auto_location=%s\n", params->auto_location ? "true" : "false");
    fprintf(file, "city=%s\n", params->location.city);
    fprintf(file, "adcode=%s\n", params->location.adcode);

    fclose(file);
    return 0;
}

int sys_load_system_parameters(const char *filepath, system_para_t *params) {
    FILE *file = fopen(filepath, "r");
    if (!file) return -1;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[128], value[128];
        if (sscanf(line, "%[^=]=%s", key, value) != 2) continue;

        if (strcmp(key, "year") == 0) {
            params->year = atoi(value);
        } else if (strcmp(key, "month") == 0) {
            params->month = atoi(value);
        } else if (strcmp(key, "day") == 0) {
            params->day = atoi(value);
        } else if (strcmp(key, "hour") == 0) {
            params->hour = (uint8_t)atoi(value);
        } else if (strcmp(key, "minute") == 0) {
            params->minute = (uint8_t)atoi(value);
        } else if (strcmp(key, "brightness") == 0) {
            params->brightness = (uint16_t)atoi(value);
        } else if (strcmp(key, "sound") == 0) {
            params->sound = (uint16_t)atoi(value);
        } else if (strcmp(key, "wifi_connected") == 0) {
            params->wifi_connected = strcmp(value, "true") == 0;
        } else if (strcmp(key, "auto_time") == 0) {
            params->auto_time = strcmp(value, "true") == 0;
        } else if (strcmp(key, "auto_location") == 0) {
            params->auto_location = strcmp(value, "true") == 0;
        } else if (strcmp(key, "city") == 0) {
            strncpy(params->location.city, value, sizeof(params->location.city)-1);
            params->location.city[sizeof(params->location.city)-1] = '\0'; // 确保字符串以null终止
        } else if (strcmp(key, "adcode") == 0) {
            strncpy(params->location.adcode, value, sizeof(params->location.adcode)-1);
            params->location.adcode[sizeof(params->location.adcode)-1] = '\0'; // 确保字符串以null终止
        }
    }

    fclose(file);
    return 0;
}