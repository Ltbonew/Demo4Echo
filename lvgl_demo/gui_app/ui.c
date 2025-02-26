#include "ui.h"
#include "./pages/ui_HomePage/ui_HomePage.h"
#include "./pages/ui_WeatherPage/ui_WeatherPage.h"
#include "./pages/ui_CalendarPage/ui_CalendarPage.h"
#include "./pages/ui_GameMuyuPage/ui_GameMuyuPage.h"
#include "./pages/ui_Game2048Page/ui_Game2048Page.h"
#include "./pages/ui_ChatBotPage/ui_ChatBotPage.h"
#include "./pages/ui_GameMemoryPage/ui_GameMemoryPage.h"
#include "./pages/ui_DrawPage/ui_DrawPage.h"
///////////////////// VARIABLES ////////////////////

lv_lib_pm_t page_manager;

ui_system_para_t ui_system_para = {
    .brightness = 50,
    .sound = 50,
    .wifi_connected = false
};

///////////////////// TEST LVGL SETTINGS ////////////////////

#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif

///////////////////// all apps ////////////////////

#define _APP_NUMS 8 // number of apps (including HomePage)

ui_app_data_t ui_apps[_APP_NUMS] = {
    {
        .name = "HomePage",
        .init = ui_HomePage_init,
        .deinit = ui_HomePage_deinit,
        .page_obj = NULL
    },
    // {
    //     .name = "SettingPage",
    //     .init = NULL,
    //     .deinit = NULL,
    //     .page_obj = NULL
    // },
    {
        .name = "WeatherPage",
        .init = ui_WeatherPage_init,
        .deinit = ui_WeatherPage_deinit,
        .page_obj = NULL
    },
    {
        .name = "CalendarPage",
        .init = ui_CalendarPage_init,
        .deinit = ui_CalendarPage_deinit,
        .page_obj = NULL
    },
    // {
    //     .name = "MemoPage",
    //     .init = NULL,
    //     .deinit = NULL,
    //     .page_obj = NULL
    // },
    {
        .name = "GameMuyuPage",
        .init = ui_GameMuyuPage_init,
        .deinit = ui_GameMuyuPage_deinit,
        .page_obj = NULL
    },
    {
        .name = "Game2048Page",
        .init = ui_Game2048Page_init,
        .deinit = ui_Game2048Page_deinit,
        .page_obj = NULL
    },
    {
        .name = "ChatBotPage",
        .init = ui_ChatBotPage_init,
        .deinit = ui_ChatBotPage_deinit,
        .page_obj = NULL
    },
    // {
    //     .name = "CameraPage",
    //     .init = NULL,
    //     .deinit = NULL,
    //     .page_obj = NULL
    // },
    // {
    //     .name = "YOLOPage",
    //     .init = NULL,
    //     .deinit = NULL,
    //     .page_obj = NULL
    // },
    {
        .name = "GameMemoryPage",
        .init = ui_GameMemoryPage_init,
        .deinit = ui_GameMemoryPage_deinit,
        .page_obj = NULL
    },
    {
        .name = "DrawPage",
        .init = ui_DrawPage_init,
        .deinit = ui_DrawPage_deinit,
        .page_obj = NULL
    }

};


///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_display_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    lv_lib_pm_Init(&page_manager);
    lv_lib_pm_page_t *pm_page[_APP_NUMS];
    for(int i = 0; i < _APP_NUMS; i++)
    {
        pm_page[i] = lv_lib_pm_CreatePage(&page_manager, ui_apps[i].name, ui_apps[i].init, ui_apps[i].deinit, NULL);
    }
    // lv_lib_pm_page_t *pm_HomePage = lv_lib_pm_CreatePage(&page_manager, "HomePage", ui_HomePage_init, ui_HomePage_deinit, NULL);
    lv_lib_pm_OpenPage(&page_manager, NULL, "HomePage");
}