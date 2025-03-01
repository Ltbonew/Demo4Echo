#include "sys_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h> // For setting system time
#include <json-c/json.h>

const char * sys_config_path = "./system_para.conf"; // 系统参数配置文件路径与可执行文件同目录
const char * city_adcode_path = "./gaode_adcode.json"; // 城市adcode对应表文件路径与可执行文件同目录

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


// 递归查找 adcode 对应的城市名称
const char* find_city_name(struct json_object *districts, const char *target_adcode) {
    if (!districts || !json_object_is_type(districts, json_type_array)) {
        return NULL;
    }

    size_t array_len = json_object_array_length(districts);
    for (size_t i = 0; i < array_len; i++) {
        struct json_object *district = json_object_array_get_idx(districts, i);
        if (!district) continue;

        struct json_object *adcode_obj, *name_obj, *sub_districts_obj;

        // 获取 adcode 和 name
        if (json_object_object_get_ex(district, "adcode", &adcode_obj) &&
            json_object_object_get_ex(district, "name", &name_obj) &&
            json_object_is_type(adcode_obj, json_type_string) &&
            json_object_is_type(name_obj, json_type_string)) {
            
            const char *adcode_str = json_object_get_string(adcode_obj);
            if (strcmp(adcode_str, target_adcode) == 0) {
                return json_object_get_string(name_obj);
            }
        }

        // 递归查找子地区
        if (json_object_object_get_ex(district, "districts", &sub_districts_obj)) {
            const char *result = find_city_name(sub_districts_obj, target_adcode);
            if (result) {
                return result;
            }
        }
    }

    return NULL;
}

// 解析 JSON 文件并查找 adcode
const char* sys_get_city_name_by_adcode(const char *filepath, const char *target_adcode) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file: %s\n", filepath);
        return NULL;
    }

    // 读取整个 JSON 文件内容
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    char *json_data = (char *)malloc(file_size + 1);
    if (!json_data) {
        fclose(fp);
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL;
    }

    fread(json_data, 1, file_size, fp);
    json_data[file_size] = '\0';
    fclose(fp);

    // 解析 JSON
    struct json_object *root = json_tokener_parse(json_data);
    free(json_data);  // 释放 JSON 读取的内存
    if (!root) {
        fprintf(stderr, "Error parsing JSON file!\n");
        return NULL;
    }

    struct json_object *districts;
    if (!json_object_object_get_ex(root, "districts", &districts)) {
        fprintf(stderr, "Invalid JSON format: missing 'districts' array.\n");
        json_object_put(root);  // 释放 JSON 对象
        return NULL;
    }

    // 递归查找
    const char *result = find_city_name(districts, target_adcode);

    // 释放 JSON 对象
    json_object_put(root);
    return result;
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