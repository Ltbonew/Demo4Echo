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
