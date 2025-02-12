#ifndef _UI_H
#define _UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../lvgl/lvgl.h"

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_1946976022);    // assets/天气64.png
LV_IMG_DECLARE(ui_img_320358418);    // assets/日历64_nonum.png
LV_IMG_DECLARE(ui_img_634722903);    // assets/备忘录64.png
LV_IMG_DECLARE(ui_img_1318505016);    // assets/日历64.png

// FONTS
LV_FONT_DECLARE(ui_font_iconfont26);
LV_FONT_DECLARE(ui_font_iconfont48);

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif