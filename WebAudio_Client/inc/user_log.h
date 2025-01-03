#ifndef USER_LOG_H
#define USER_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>

// 获取当前时间戳
static inline void get_timestamp(char *buf, size_t len) {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(buf, len, "%H:%M:%S", time_info);
}

// 定义日志宏
#define USER_LOG(level, format, ...) \
    do { \
        char timestamp[9]; \
        get_timestamp(timestamp, sizeof(timestamp)); \
        fprintf(stderr, "[%s] (%s) %s: " format " %s:%d\n", \
                level, timestamp, __func__, ##__VA_ARGS__, __FILE__, __LINE__); \
    } while (0)

// 具体的日志级别宏定义
#define USER_LOG_INFO(format, ...)   USER_LOG("INFO", format, ##__VA_ARGS__)
#define USER_LOG_WARN(format, ...)   USER_LOG("WARN", format, ##__VA_ARGS__)
#define USER_LOG_ERRO(format, ...)  USER_LOG("ERRO", format, ##__VA_ARGS__)

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // USER_LOG_H