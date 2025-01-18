#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "gui_app/common/lv_lib.h"

static const char *getenv_default(const char *name, const char *dflt)
{
    return getenv(name) ? : dflt;
}

#if LV_USE_EVDEV
static void lv_linux_indev_init(void)
{
    lv_indev_t * touch;
    touch = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event0");
}
#endif

#if LV_USE_LINUX_FBDEV
static void lv_linux_disp_init(void)
{
    const char *device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
    lv_display_t * disp = lv_linux_fbdev_create();

    lv_linux_fbdev_set_file(disp, device);
}
#elif LV_USE_LINUX_DRM
static void lv_linux_disp_init(void)
{
    const char *device = getenv_default("LV_LINUX_DRM_CARD", "/dev/dri/card0");
    lv_display_t * disp = lv_linux_drm_create();

    lv_linux_drm_set_file(disp, device, -1);
}
#elif LV_USE_SDL
static void lv_linux_disp_init(void)
{
    const int width = atoi(getenv("LV_SDL_VIDEO_WIDTH") ? : "320");
    const int height = atoi(getenv("LV_SDL_VIDEO_HEIGHT") ? : "240");

    lv_sdl_window_create(width, height);
}

static void lv_linux_indev_init(void)
{
    lv_sdl_mouse_create();
}

#else
#error Unsupported configuration
#endif

lv_lib_pm_t page_manager;

static void btn1_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
        lv_lib_pm_OpenPage(&page_manager, NULL, "Page2");
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }
}

void page1_init(void)
{
    lv_obj_t *ui_Page1 = lv_obj_create(NULL);
    lv_obj_t * btn1 = lv_btn_create(ui_Page1);
    lv_obj_add_event_cb(btn1, btn1_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t* label = lv_label_create(btn1);
    lv_label_set_text(label, "Button1");
    lv_obj_center(label);

    // load page
    lv_scr_load_anim(ui_Page1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

void page1_deinit(void)
{
    return;
}

static void btn2_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
        lv_lib_pm_OpenPrePage(&page_manager);
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }
}

void page2_init(void)
{
    lv_obj_t *ui_Page2 = lv_obj_create(NULL);
    lv_obj_t * btn2 = lv_btn_create(ui_Page2);
    lv_obj_add_event_cb(btn2, btn2_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t* label = lv_label_create(btn2);
    lv_label_set_text(label, "Button2");
    lv_obj_center(label);

    // load page
    lv_scr_load_anim(ui_Page2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

void page2_deinit(void)
{
    return;
}

void lv_demo_test()
{   
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    lv_lib_pm_Init(&page_manager);
    lv_lib_pm_page_t *pm_page1 = lv_lib_pm_CreatePage(&page_manager, "Page1", page1_init, page1_deinit, NULL);
    lv_lib_pm_page_t *pm_page2 = lv_lib_pm_CreatePage(&page_manager, "Page2", page2_init, page2_deinit, NULL);
    lv_lib_pm_OpenPage(&page_manager, pm_page1, "Page1");

}

int main(void)
{
    lv_init();

    /*Linux display device init*/
    lv_linux_disp_init();

    lv_linux_indev_init();

    /*Create a Demo*/
    // lv_demo_widgets();
    // lv_demo_widgets_start_slideshow();
    lv_demo_test();
    /*Handle LVGL tasks*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}
