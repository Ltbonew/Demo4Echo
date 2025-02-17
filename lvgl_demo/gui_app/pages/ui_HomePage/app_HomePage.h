#ifndef _APP_HOMEPAGE_H
#define _APP_HOMEPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void get_current_time(uint8_t *hour, uint8_t *minute);
void get_current_date(int *year, int *month, int *day);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif