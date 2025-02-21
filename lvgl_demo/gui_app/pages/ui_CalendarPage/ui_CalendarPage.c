#include "ui_CalendarPage.h"

///////////////////// VARIABLES ////////////////////


///////////////////// ANIMATIONS ////////////////////


///////////////////// FUNCTIONS ////////////////////


///////////////////// SCREEN init ////////////////////

void ui_CalendarPage_init()
{
    lv_obj_t  * calendar = lv_calendar_create(NULL);
    lv_obj_set_size(calendar, 300, 300);
    lv_obj_align(calendar, LV_ALIGN_TOP_MID, 0, 0);

    lv_calendar_set_today_date(calendar, 2024, 03, 22);
    lv_calendar_set_showed_date(calendar, 2024, 03);

#if LV_USE_CALENDAR_HEADER_DROPDOWN
    lv_calendar_header_dropdown_create(calendar);
#elif LV_USE_CALENDAR_HEADER_ARROW
    lv_calendar_header_arrow_create(calendar);
#endif

    lv_calendar_set_chinese_mode(calendar, true);
    lv_obj_set_style_text_font(calendar, &lv_font_simsun_16_cjk, LV_PART_MAIN);

    // load page
    lv_scr_load_anim(calendar, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_CalendarPage_deinit()
{
    // deinit
}
