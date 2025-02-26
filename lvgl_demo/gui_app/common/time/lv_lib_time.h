#ifndef LV_LIB_TIME_H
#define LV_LIB_TIME_H

#include "../lv_lib_conf.h"

#if LV_USE_LIB_TIME

void lv_lib_get_time(uint8_t *hour, uint8_t *minute);
void lv_lib_get_date(int *year, int *month, int *day);
int lv_lib_calculate_day_of_week(int year, int month, int day);


#endif  /*LV_USE_LIB_TIME*/

#endif // LV_LIB_TIME_H
