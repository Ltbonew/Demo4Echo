#include "ui_GameMuyuPage.h"

///////////////////// VARIABLES ////////////////////

lv_obj_t * ui_MuyuImg;
lv_obj_t * ui_LabelTolal;
lv_obj_t * ui_LabelAdd;

///////////////////// ANIMATIONS ////////////////////

static void _ui_anim_callback_set_scale(void * var, int32_t v)
{
    lv_image_set_scale(var, v);
}

static void _ui_user_Animation(lv_obj_t * TagetObj, uint16_t delay, uint16_t time, int16_t start_value, int16_t end_value,
                                uint16_t playback_delay, uint16_t playback_time, uint16_t repeat_delay, uint16_t repeat_count,
                                lv_anim_path_cb_t path_cb, lv_anim_exec_xcb_t exec_cb)
{
    lv_anim_t Animation;
    lv_anim_init(&Animation);
    lv_anim_set_var(&Animation, TagetObj);
    lv_anim_set_time(&Animation, time);
    lv_anim_set_values(&Animation, start_value, end_value);
    lv_anim_set_exec_cb(&Animation, exec_cb);
    lv_anim_set_path_cb(&Animation, path_cb);
    lv_anim_set_delay(&Animation, delay);
    lv_anim_set_playback_time(&Animation, playback_time);
    lv_anim_set_playback_delay(&Animation, playback_delay);
    lv_anim_set_repeat_count(&Animation, repeat_count);
    lv_anim_set_repeat_delay(&Animation, repeat_delay);
    lv_anim_set_early_apply(&Animation, false);
    lv_anim_start(&Animation);
}

static void _Click_Animation()
{
    int16_t MuyuScale_now = 256;

    _ui_user_Animation(ui_MuyuImg, 0, 200, MuyuScale_now, MuyuScale_now-20, 0, 0, 0, 0, lv_anim_path_ease_in_out, _ui_anim_callback_set_scale);


}

///////////////////// FUNCTIONS ////////////////////

void ui_event_click(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        LV_LOG_USER("clicked");
    }
}

///////////////////// SCREEN init ////////////////////

void ui_GameMuyuPage_init(void)
{
    lv_obj_t * ui_GameMuyuPage = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_GameMuyuPage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_MuyuImg = lv_image_create(ui_GameMuyuPage);
    lv_image_set_src(ui_MuyuImg, &ui_img_546892240);
    lv_obj_set_width(ui_MuyuImg, LV_SIZE_CONTENT);   /// 177
    lv_obj_set_height(ui_MuyuImg, LV_SIZE_CONTENT);    /// 128
    lv_obj_set_x(ui_MuyuImg, 0);
    lv_obj_set_y(ui_MuyuImg, 20);
    lv_obj_set_align(ui_MuyuImg, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_MuyuImg, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_MuyuImg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_scale(ui_MuyuImg, 256);

    ui_LabelTolal = lv_label_create(ui_GameMuyuPage);
    lv_obj_set_width(ui_LabelTolal, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTolal, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTolal, 75);
    lv_obj_set_y(ui_LabelTolal, -85);
    lv_obj_set_align(ui_LabelTolal, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelTolal, "今日功德 9999");
    lv_obj_set_style_text_color(ui_LabelTolal, lv_color_hex(0xF8BA14), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTolal, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelTolal, &ui_font_shuhei22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelAdd = lv_label_create(ui_GameMuyuPage);
    lv_obj_set_width(ui_LabelAdd, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelAdd, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelAdd, -70);
    lv_obj_set_y(ui_LabelAdd, -70);
    lv_obj_set_align(ui_LabelAdd, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelAdd, "功德 + 1");
    lv_obj_set_style_text_color(ui_LabelAdd, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelAdd, 208, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelAdd, &ui_font_shuhei22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_MainPanel = lv_obj_create(ui_GameMuyuPage);
    lv_obj_set_width(ui_MainPanel, 320);
    lv_obj_set_height(ui_MainPanel, 240);
    lv_obj_set_align(ui_MainPanel, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_MainPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_MainPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MainPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_MainPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_MainPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // event
    lv_obj_add_event_cb(ui_MainPanel, ui_event_click, LV_EVENT_CLICKED, NULL);

    // load page
    lv_scr_load_anim(ui_GameMuyuPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_GameMuyuPage_deinit(void)
{
    // deinit
}