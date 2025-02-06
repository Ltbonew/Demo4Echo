#ifndef USER_LOG_H
#define USER_LOG_H

#include <stdio.h>
#include <time.h>
#include <libgen.h>  // For basename()

// ANSI颜色代码
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"

// 获取当前时间戳
static inline void get_timestamp(char *buf, size_t len) {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(buf, len, "%H:%M:%S", time_info);
}

// 提取文件名（不带路径）
static inline const char* get_filename(const char *path) {
    char *filename = basename((char *)path);
    return filename ? filename : "unknown";
}

// 定义日志宏，带有颜色
#define USER_LOG(level_color, level, format, ...) \
    do { \
        char timestamp[9]; \
        get_timestamp(timestamp, sizeof(timestamp)); \
        fprintf(stderr, "%s[%s]%s" " (%s) %s: " format " %s:%d\n", \
                level_color, level, COLOR_RESET, timestamp, __func__, ##__VA_ARGS__, get_filename(__FILE__), __LINE__); \
    } while (0)

// 具体的日志级别宏定义，应用颜色
#define USER_LOG_INFO(format, ...)   USER_LOG("", "INFO", format, ##__VA_ARGS__)
#define USER_LOG_WARN(format, ...)   USER_LOG(COLOR_YELLOW, "WARN", format, ##__VA_ARGS__)
#define USER_LOG_ERROR(format, ...)  USER_LOG(COLOR_RED, "ERRO", format, ##__VA_ARGS__)

#endif // USER_LOG_H