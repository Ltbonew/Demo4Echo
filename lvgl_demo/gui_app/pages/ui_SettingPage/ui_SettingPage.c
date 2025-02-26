#include "ui_SettingPage.h"

///////////////////// VARIABLES ////////////////////


///////////////////// ANIMATIONS ////////////////////


///////////////////// FUNCTIONS ////////////////////


///////////////////// SCREEN init ////////////////////

void ui_SettingPage_init()
{
    lv_obj_t * SettingPage = lv_obj_create(NULL);

    // load page
    lv_scr_load_anim(SettingPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_SettingPage_deinit()
{
    // deinit
}