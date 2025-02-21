#include "ui_CalendarPage.h"
#include "app_HomePage.h"
///////////////////// VARIABLES ////////////////////


///////////////////// ANIMATIONS ////////////////////


///////////////////// FUNCTIONS ////////////////////

static void ui_enent_Gesture(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_GESTURE)
    {
        if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT || lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
        {
            lv_lib_pm_OpenPrePage(&page_manager);
        }
    }
}

///////////////////// SCREEN init ////////////////////

void ui_CalendarPage_init()
{
    lv_obj_t * ui_CalendarPage = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_CalendarPage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_t * calendar = lv_calendar_create(ui_CalendarPage);
    lv_obj_t * ui_Calendar1_header = lv_calendar_header_arrow_create(calendar);
    lv_obj_set_width(calendar, 320);
    lv_obj_set_height(calendar, 240);
    lv_obj_align(calendar, LV_ALIGN_TOP_MID, 0, 0);
    int year, month, day;
    get_current_date(&year, &month, &day);
    lv_calendar_set_today_date(calendar, year, month, day);
    lv_calendar_set_showed_date(calendar, year, month);

    // event
    lv_obj_add_event_cb(ui_CalendarPage, ui_enent_Gesture, LV_EVENT_ALL, NULL);

    // load page
    lv_scr_load_anim(ui_CalendarPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_CalendarPage_deinit()
{
    // deinit
}
