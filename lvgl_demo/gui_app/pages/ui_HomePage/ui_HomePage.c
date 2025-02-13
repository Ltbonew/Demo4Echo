#include "ui_HomePage.h"

///////////////////// VARIABLES ////////////////////

#define UI_HOME_WITDH 320
#define UI_HOME_HEIGHT 240
#define UI_HOME_CONTAINER_PAGES 2
uint8_t ui_home_container_page_index = 0; // first page
lv_obj_t * ui_ScrollDots[UI_HOME_CONTAINER_PAGES];

#define UI_HOME_STATE_SHOW_DESKTOP 0
#define UI_HOME_STATE_SHOW_DROP_DOWN_PAGE 1
uint8_t ui_home_state = UI_HOME_STATE_SHOW_DESKTOP;

uint8_t ui_home_scroll_busy = 0; // avoid scroll too fast, and btns click event happen at the same time

///////////////////// ANIMATIONS ////////////////////

void _ui_anim_callback_set_x(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

void AppContLeft_Animation(lv_obj_t * TargetObject, int delay)
{
    int32_t x_pos_now = lv_obj_get_x(TargetObject);
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_var(&PropertyAnimation_0, TargetObject);
    lv_anim_set_time(&PropertyAnimation_0, 250);
    lv_anim_set_values(&PropertyAnimation_0, x_pos_now, x_pos_now - UI_HOME_WITDH);
    lv_anim_set_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}

void AppContRight_Animation(lv_obj_t * TargetObject, int delay)
{
    int32_t x_pos_now = lv_obj_get_x(TargetObject);
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_var(&PropertyAnimation_0, TargetObject);
    lv_anim_set_time(&PropertyAnimation_0, 250);
    lv_anim_set_values(&PropertyAnimation_0, x_pos_now, x_pos_now + UI_HOME_WITDH);
    lv_anim_set_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}

///////////////////// FUNCTIONS ////////////////////

void ui_event_TopDrag(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * dropdown_panel = lv_event_get_user_data(e);

    if(event_code == LV_EVENT_PRESSING) {
        lv_indev_t * indev = lv_indev_active();
        if(indev == NULL)  return;

        lv_point_t vect;
        lv_indev_get_vect(indev, &vect);

        int32_t y = lv_obj_get_y_aligned(obj) + vect.y;
        lv_obj_set_y(obj, y); // set drag position follow the touch
        lv_obj_set_y(dropdown_panel, y-UI_HOME_HEIGHT); // set dorp down panel
    }

    else if(event_code == LV_EVENT_RELEASED) {
        int32_t y = lv_obj_get_y_aligned(obj);
        if(y >= UI_HOME_HEIGHT/2)
        {
            lv_obj_set_y(dropdown_panel, 0);
            lv_obj_set_y(obj, UI_HOME_HEIGHT-lv_obj_get_height(obj));
            ui_home_state = UI_HOME_STATE_SHOW_DROP_DOWN_PAGE;
        }
        else 
        {
            lv_obj_set_y(dropdown_panel, -UI_HOME_HEIGHT);
            lv_obj_set_y(obj, 0);
            ui_home_state = UI_HOME_STATE_SHOW_DESKTOP;
        }
    }
}

void ui_event_gesture(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * AppContainer = lv_event_get_user_data(e);
    if(event_code == LV_EVENT_GESTURE) {
        if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
        {   
            // not in the end page
            if(lv_obj_get_x(AppContainer) != (-UI_HOME_WITDH) * (UI_HOME_CONTAINER_PAGES-1))
            {
                AppContLeft_Animation(AppContainer, 0);
                lv_obj_set_style_bg_opa(ui_ScrollDots[ui_home_container_page_index], 96, LV_PART_MAIN | LV_STATE_DEFAULT);
                ui_home_container_page_index++;
                lv_obj_set_style_bg_opa(ui_ScrollDots[ui_home_container_page_index], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
        else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
        {
            // not in the first page
            if(lv_obj_get_x(AppContainer) != 0)
            {
                AppContRight_Animation(AppContainer, 0);
                lv_obj_set_style_bg_opa(ui_ScrollDots[ui_home_container_page_index], 96, LV_PART_MAIN | LV_STATE_DEFAULT);
                ui_home_container_page_index--;
                lv_obj_set_style_bg_opa(ui_ScrollDots[ui_home_container_page_index], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
    }
}

void ui_event_CalendarBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(event_code == LV_EVENT_CLICKED) {
        LV_LOG_USER("LV_EVENT_CLICKED");
    }
}

///////////////////// SCREEN init ////////////////////

void ui_HomeScreen_screen_init(void)
{
    lv_obj_t * ui_HomeScreen = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_HomeScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // apps‘ container, to contain all apps btns
    lv_obj_t * ui_AppIconContainer = lv_obj_create(ui_HomeScreen);
    lv_obj_set_width(ui_AppIconContainer, UI_HOME_WITDH * UI_HOME_CONTAINER_PAGES);
    lv_obj_set_height(ui_AppIconContainer, UI_HOME_HEIGHT);
    lv_obj_set_align(ui_AppIconContainer, LV_ALIGN_LEFT_MID);
    lv_obj_remove_flag(ui_AppIconContainer, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_AppIconContainer, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_AppIconContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_AppIconContainer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_AppIconContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // gesture event
    lv_obj_add_event_cb(ui_HomeScreen, ui_event_gesture, LV_EVENT_GESTURE, ui_AppIconContainer);

    // time Label
    lv_obj_t * ui_TimeLabel = lv_label_create(ui_HomeScreen);
    lv_obj_set_width(ui_TimeLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_TimeLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_TimeLabel, 0);
    lv_obj_set_y(ui_TimeLabel, 5);
    lv_obj_set_align(ui_TimeLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_TimeLabel, "11:59");
    lv_obj_set_style_text_font(ui_TimeLabel, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    // wifi Label
    lv_obj_t * ui_WifiLabel = lv_label_create(ui_HomeScreen);
    lv_obj_set_width(ui_WifiLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_WifiLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_WifiLabel, 130);
    lv_obj_set_y(ui_WifiLabel, 7);
    lv_obj_set_align(ui_WifiLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_WifiLabel, "");
    lv_obj_set_style_text_font(ui_WifiLabel, &ui_font_iconfont26, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_NoWifiLabel = lv_label_create(ui_HomeScreen);
    lv_obj_set_width(ui_NoWifiLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_NoWifiLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_NoWifiLabel, 130);
    lv_obj_set_y(ui_NoWifiLabel, 7);
    lv_obj_set_align(ui_NoWifiLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_NoWifiLabel, "");
    lv_obj_add_flag(ui_NoWifiLabel, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_set_style_text_font(ui_NoWifiLabel, &ui_font_iconfont26, LV_PART_MAIN | LV_STATE_DEFAULT);

    // page dots
    for(int i = 0; i < UI_HOME_CONTAINER_PAGES; i++)
    {
        int16_t start_pos;
        if(UI_HOME_CONTAINER_PAGES%2==0)
        {
            start_pos = -20*(UI_HOME_CONTAINER_PAGES/2) + 10;
        }
        else
        {
            start_pos = -20*(UI_HOME_CONTAINER_PAGES/2 + 1) + 20;
        }
        ui_ScrollDots[i] = lv_obj_create(ui_HomeScreen);
        lv_obj_set_width(ui_ScrollDots[i], 8);
        lv_obj_set_height(ui_ScrollDots[i], 8);
        lv_obj_set_x(ui_ScrollDots[i], start_pos + 20 * i);
        lv_obj_set_y(ui_ScrollDots[i], -10);
        lv_obj_set_align(ui_ScrollDots[i], LV_ALIGN_BOTTOM_MID);
        lv_obj_remove_flag(ui_ScrollDots[i], LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(ui_ScrollDots[i], 20, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ScrollDots[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ScrollDots[i], 96, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_ScrollDots[i], lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_ScrollDots[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    lv_obj_set_style_bg_opa(ui_ScrollDots[0], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // dropdown panel
    lv_obj_t * ui_DropdownPanel = lv_obj_create(ui_HomeScreen);
    lv_obj_set_width(ui_DropdownPanel, UI_HOME_WITDH);
    lv_obj_set_height(ui_DropdownPanel, UI_HOME_HEIGHT);
    lv_obj_set_x(ui_DropdownPanel, 0);
    lv_obj_set_y(ui_DropdownPanel, -UI_HOME_HEIGHT);
    lv_obj_set_align(ui_DropdownPanel, LV_ALIGN_TOP_MID);
    lv_obj_remove_flag(ui_DropdownPanel,
                       LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_scrollbar_mode(ui_DropdownPanel, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_dir(ui_DropdownPanel, LV_DIR_VER);
    lv_obj_set_style_bg_color(ui_DropdownPanel, lv_color_hex(0x323232), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DropdownPanel, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_DropdownPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_DropdownPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // top drag
    lv_obj_t * ui_TopDragPanel = lv_obj_create(ui_HomeScreen);
    lv_obj_set_width(ui_TopDragPanel, UI_HOME_WITDH);
    lv_obj_set_height(ui_TopDragPanel, 30);
    lv_obj_set_align(ui_TopDragPanel, LV_ALIGN_TOP_MID);
    lv_obj_remove_flag(ui_TopDragPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_TopDragPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_TopDragPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_TopDragPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_TopDragPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // top drag event
    lv_obj_add_event_cb(ui_TopDragPanel, ui_event_TopDrag, LV_EVENT_PRESSING, ui_DropdownPanel);
    lv_obj_add_event_cb(ui_TopDragPanel, ui_event_TopDrag, LV_EVENT_RELEASED, ui_DropdownPanel);


    // setting app
    lv_obj_t * ui_SettingBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_SettingBtn, 70);
    lv_obj_set_height(ui_SettingBtn, 70);
    lv_obj_set_x(ui_SettingBtn, 15);
    lv_obj_set_y(ui_SettingBtn, -45);
    lv_obj_set_align(ui_SettingBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_SettingBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_SettingBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_SettingBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SettingBtn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingBtn, 168, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_SettingIcon = lv_label_create(ui_SettingBtn);
    lv_obj_set_width(ui_SettingIcon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SettingIcon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_SettingIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingIcon, "");
    lv_obj_set_style_text_font(ui_SettingIcon, &ui_font_iconfont48, LV_PART_MAIN | LV_STATE_DEFAULT);


    // weather app
    lv_obj_t * ui_WeatherBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_WeatherBtn, 70);
    lv_obj_set_height(ui_WeatherBtn, 70);
    lv_obj_set_x(ui_WeatherBtn, 110);
    lv_obj_set_y(ui_WeatherBtn, -45);
    lv_obj_set_align(ui_WeatherBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_WeatherBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_WeatherBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_WeatherBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui_WeatherBtn, &ui_img_1946976022, LV_PART_MAIN | LV_STATE_DEFAULT);


    // calendar app
    lv_obj_t * ui_CalendarBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_CalendarBtn, 70);
    lv_obj_set_height(ui_CalendarBtn, 70);
    lv_obj_set_x(ui_CalendarBtn, 205);
    lv_obj_set_y(ui_CalendarBtn, -45);
    lv_obj_set_align(ui_CalendarBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_CalendarBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_CalendarBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_CalendarBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_CalendarBtn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CalendarBtn, 224, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui_CalendarBtn, &ui_img_320358418, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_DateLabel = lv_label_create(ui_CalendarBtn);
    lv_obj_set_width(ui_DateLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_DateLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_DateLabel, 0);
    lv_obj_set_y(ui_DateLabel, 8);
    lv_obj_set_align(ui_DateLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_DateLabel, "7");
    lv_obj_set_style_text_color(ui_DateLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_DateLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DateLabel, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    // calendar event
    lv_obj_add_event_cb(ui_CalendarBtn, ui_event_CalendarBtn, LV_EVENT_CLICKED, NULL);


    // memo app
    lv_obj_t * ui_MemoBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_MemoBtn, 70);
    lv_obj_set_height(ui_MemoBtn, 70);
    lv_obj_set_x(ui_MemoBtn, 15);
    lv_obj_set_y(ui_MemoBtn, 55);
    lv_obj_set_align(ui_MemoBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_MemoBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_MemoBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_MemoBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_MemoBtn, lv_color_hex(0xB83B5E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MemoBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui_MemoBtn, &ui_img_634722903, LV_PART_MAIN | LV_STATE_DEFAULT);

    // game muyu app
    lv_obj_t * ui_GameMuyuBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_GameMuyuBtn, 70);
    lv_obj_set_height(ui_GameMuyuBtn, 70);
    lv_obj_set_x(ui_GameMuyuBtn, 110);
    lv_obj_set_y(ui_GameMuyuBtn, 55);
    lv_obj_set_align(ui_GameMuyuBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_GameMuyuBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_GameMuyuBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_GameMuyuBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);


    // game 2048 app
    lv_obj_t * ui_Game2048Btn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_Game2048Btn, 70);
    lv_obj_set_height(ui_Game2048Btn, 70);
    lv_obj_set_x(ui_Game2048Btn, 205);
    lv_obj_set_y(ui_Game2048Btn, 55);
    lv_obj_set_align(ui_Game2048Btn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_Game2048Btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Game2048Btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Game2048Btn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);


    // AI Chat app
    lv_obj_t * ui_AIChatBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_AIChatBtn, 70);
    lv_obj_set_height(ui_AIChatBtn, 70);
    lv_obj_set_x(ui_AIChatBtn, 335);
    lv_obj_set_y(ui_AIChatBtn, -45);
    lv_obj_set_align(ui_AIChatBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_AIChatBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_AIChatBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_AIChatBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);


    // Camera app
    lv_obj_t * ui_CameraBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_CameraBtn, 70);
    lv_obj_set_height(ui_CameraBtn, 70);
    lv_obj_set_x(ui_CameraBtn, 430);
    lv_obj_set_y(ui_CameraBtn, -45);
    lv_obj_set_align(ui_CameraBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_CameraBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_CameraBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_CameraBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);


    // YOLO app
    lv_obj_t * ui_YOLOBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_YOLOBtn, 70);
    lv_obj_set_height(ui_YOLOBtn, 70);
    lv_obj_set_x(ui_YOLOBtn, 525);
    lv_obj_set_y(ui_YOLOBtn, -45);
    lv_obj_set_align(ui_YOLOBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_YOLOBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_YOLOBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_YOLOBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);


    // Game Memory app
    lv_obj_t * ui_GameMemBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_GameMemBtn, 70);
    lv_obj_set_height(ui_GameMemBtn, 70);
    lv_obj_set_x(ui_GameMemBtn, 335);
    lv_obj_set_y(ui_GameMemBtn, 55);
    lv_obj_set_align(ui_GameMemBtn, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_GameMemBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_GameMemBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_GameMemBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);


    // load page
    lv_scr_load_anim(ui_HomeScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);

}

///////////////////// SCREEN deinit ////////////////////

void ui_HomeScreen_screen_deinit(void)
{
    return;
}