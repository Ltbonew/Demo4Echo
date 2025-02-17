#ifndef _UI_HOMEPAGE_H
#define _UI_HOMEPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"
#include "../../common/lv_lib.h"

typedef struct {
    uint16_t witdh;                 // screen width  
    uint16_t height;                // screen height
    uint8_t container_total_pages;  // total pages of app-container
    uint8_t app_container_index;    // present app-container index
    bool show_dropdown;             // show dropdown or not
    bool scroll_busy;               // avoid scroll too fast, and btns click event happen at the same time
}ui_desktop_data_t;

typedef struct {
    // uint8_t time_hour;              // system time hour
    // uint8_t time_minute;            // system time minute
    uint16_t brightness;            // system brightness
    uint16_t sound;                 // system sound volume
    bool wifi_connected;            // wifi connected or not
}ui_desktop_para_t;

typedef struct {
    lv_lib_pm_page_t * app_page;    // app page
    uint8_t name[16];               // app page name
}ui_app_data_t;

void ui_HomeScreen_screen_init(void);
void ui_HomeScreen_screen_deinit(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif