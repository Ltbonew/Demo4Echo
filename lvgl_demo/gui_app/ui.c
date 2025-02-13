#include "ui.h"
#include "./common/lv_lib.h"
#include "./pages/ui_HomePage/ui_HomePage.h"
///////////////////// VARIABLES ////////////////////

lv_lib_pm_t page_manager;

///////////////////// TEST LVGL SETTINGS ////////////////////

#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////


///////////////////// FUNCTIONS ////////////////////


///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_display_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    lv_lib_pm_Init(&page_manager);
    lv_lib_pm_page_t *pm_page1 = lv_lib_pm_CreatePage(&page_manager, "HomePage", ui_HomeScreen_screen_init, ui_HomeScreen_screen_deinit, NULL);
    // lv_lib_pm_page_t *pm_page2 = lv_lib_pm_CreatePage(&page_manager, "Page2", page2_init, page2_deinit, NULL);
    lv_lib_pm_OpenPage(&page_manager, pm_page1, "HomePage");
}