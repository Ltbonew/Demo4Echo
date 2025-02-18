#include "ui_ChatBotPage.h"

///////////////////// VARIABLES ////////////////////

lv_obj_t * ui_EyesPanel;
lv_obj_t * ui_EyesVerMovePanel;
lv_obj_t * ui_QuestionImg;
lv_obj_t * ui_thinkImg;

///////////////////// ANIMATIONS ////////////////////

void _ui_anim_callback_set_y(void * var, int32_t v)
{
    lv_obj_set_y(var, v);
}

void _ui_anim_callback_set_image_angle(void * var, int32_t v)
{
    lv_image_set_rotation(var, v);
}

void _ui_anim_callback_set_opacity(void * var, int32_t v)
{
    lv_obj_set_style_opa(var, v, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void IdleMove2_Animation(void)
{
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_var(&PropertyAnimation_0, ui_EyesVerMovePanel);
    lv_anim_set_time(&PropertyAnimation_0, 500);
    lv_anim_set_values(&PropertyAnimation_0, 0, -20);
    lv_anim_set_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);
    lv_anim_set_delay(&PropertyAnimation_0, 0 + 0);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

    lv_anim_t PropertyAnimation_1;
    lv_anim_init(&PropertyAnimation_1);
    lv_anim_set_var(&PropertyAnimation_1, ui_EyesVerMovePanel);
    lv_anim_set_time(&PropertyAnimation_1, 500);
    lv_anim_set_values(&PropertyAnimation_1, -20, 0);
    lv_anim_set_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_y);
    lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_ease_out);
    lv_anim_set_delay(&PropertyAnimation_1, 0 + 2500);
    lv_anim_set_playback_time(&PropertyAnimation_1, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
    lv_anim_set_early_apply(&PropertyAnimation_1, false);
    lv_anim_start(&PropertyAnimation_1);

    lv_anim_t PropertyAnimation_2;
    lv_anim_init(&PropertyAnimation_2);
    lv_anim_set_var(&PropertyAnimation_2, ui_EyesPanel);
    lv_anim_set_time(&PropertyAnimation_2, 500);
    lv_anim_set_values(&PropertyAnimation_2, -25, -25+20);
    lv_anim_set_exec_cb(&PropertyAnimation_2, _ui_anim_callback_set_y);
    lv_anim_set_path_cb(&PropertyAnimation_2, lv_anim_path_ease_out);
    lv_anim_set_delay(&PropertyAnimation_2, 0 + 0);
    lv_anim_set_playback_time(&PropertyAnimation_2, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_2, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_2, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_2, 0);
    lv_anim_set_early_apply(&PropertyAnimation_2, false);
    lv_anim_start(&PropertyAnimation_2);

    lv_anim_t PropertyAnimation_3;
    lv_anim_init(&PropertyAnimation_3);
    lv_anim_set_var(&PropertyAnimation_3, ui_EyesPanel);
    lv_anim_set_time(&PropertyAnimation_3, 500);
    lv_anim_set_values(&PropertyAnimation_3, -25+20, -25);
    lv_anim_set_exec_cb(&PropertyAnimation_3, _ui_anim_callback_set_y);
    lv_anim_set_path_cb(&PropertyAnimation_3, lv_anim_path_ease_out);
    lv_anim_set_delay(&PropertyAnimation_3, 0 + 2500);
    lv_anim_set_playback_time(&PropertyAnimation_3, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_3, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_3, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_3, 0);
    lv_anim_set_early_apply(&PropertyAnimation_3, false);
    lv_anim_start(&PropertyAnimation_3);

    lv_anim_t PropertyAnimation_4;
    lv_anim_init(&PropertyAnimation_4);
    lv_anim_set_var(&PropertyAnimation_4, ui_thinkImg);
    lv_anim_set_time(&PropertyAnimation_4, 500);
    lv_anim_set_values(&PropertyAnimation_4, -100, 100);
    lv_anim_set_exec_cb(&PropertyAnimation_4, _ui_anim_callback_set_image_angle);
    lv_anim_set_path_cb(&PropertyAnimation_4, lv_anim_path_ease_in_out);
    lv_anim_set_delay(&PropertyAnimation_4, 0 + 0);
    lv_anim_set_playback_time(&PropertyAnimation_4, 750);
    lv_anim_set_playback_delay(&PropertyAnimation_4, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_4, 3);
    lv_anim_set_repeat_delay(&PropertyAnimation_4, 0);
    lv_anim_set_early_apply(&PropertyAnimation_4, false);
    lv_anim_start(&PropertyAnimation_4);

    lv_anim_t PropertyAnimation_5;
    lv_anim_init(&PropertyAnimation_5);
    lv_anim_set_var(&PropertyAnimation_5, ui_thinkImg);
    lv_anim_set_time(&PropertyAnimation_5, 500);
    lv_anim_set_values(&PropertyAnimation_5, 0, 255);
    lv_anim_set_exec_cb(&PropertyAnimation_5, _ui_anim_callback_set_opacity);
    lv_anim_set_path_cb(&PropertyAnimation_5, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_5, 0 + 0);
    lv_anim_set_playback_time(&PropertyAnimation_5, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_5, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_5, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_5, 0);
    lv_anim_set_early_apply(&PropertyAnimation_5, false);
    lv_anim_start(&PropertyAnimation_5);

    lv_anim_t PropertyAnimation_6;
    lv_anim_init(&PropertyAnimation_6);
    lv_anim_set_var(&PropertyAnimation_6, ui_thinkImg);
    lv_anim_set_time(&PropertyAnimation_6, 500);
    lv_anim_set_values(&PropertyAnimation_6, 255, 0);
    lv_anim_set_exec_cb(&PropertyAnimation_6, _ui_anim_callback_set_opacity);
    lv_anim_set_path_cb(&PropertyAnimation_6, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_6, 0 + 3000);
    lv_anim_set_playback_time(&PropertyAnimation_6, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_6, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_6, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_6, 0);
    lv_anim_set_early_apply(&PropertyAnimation_6, false);
    lv_anim_start(&PropertyAnimation_6);
}

///////////////////// FUNCTIONS ////////////////////

void ui_event_ChatBotPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_active()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_active());
        // EyesIdleMove_Animation(ui_EyesPanel, 0);
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_active()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_active());
        IdleMove2_Animation();
    }
}

///////////////////// SCREEN init ////////////////////

void ui_ChatBotPage_init(void)
{
    lv_obj_t * ui_ChatBotPage = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_ChatBotPage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_EyesPanel = lv_obj_create(ui_ChatBotPage);
    lv_obj_set_width(ui_EyesPanel, 210);
    lv_obj_set_height(ui_EyesPanel, 80);
    lv_obj_set_x(ui_EyesPanel, 0);
    lv_obj_set_y(ui_EyesPanel, -25);
    lv_obj_set_align(ui_EyesPanel, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_EyesPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_EyesPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_EyesPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_EyesPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_EyesPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_EyesVerMovePanel = lv_obj_create(ui_EyesPanel);
    lv_obj_set_width(ui_EyesVerMovePanel, 210);
    lv_obj_set_height(ui_EyesVerMovePanel, 80);
    lv_obj_set_align(ui_EyesVerMovePanel, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_EyesVerMovePanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_EyesVerMovePanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_EyesVerMovePanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_EyesVerMovePanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_EyesVerMovePanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_EyeRight = lv_button_create(ui_EyesVerMovePanel);
    lv_obj_set_width(ui_EyeRight, 80);
    lv_obj_set_height(ui_EyeRight, 80);
    lv_obj_set_x(ui_EyeRight, 60);
    lv_obj_set_y(ui_EyeRight, 0);
    lv_obj_set_align(ui_EyeRight, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_EyeRight, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_EyeRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_EyeRight, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_EyeRight, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_EyeRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_EyeLeft = lv_button_create(ui_EyesVerMovePanel);
    lv_obj_set_width(ui_EyeLeft, 80);
    lv_obj_set_height(ui_EyeLeft, 80);
    lv_obj_set_x(ui_EyeLeft, -60);
    lv_obj_set_y(ui_EyeLeft, 0);
    lv_obj_set_align(ui_EyeLeft, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_EyeLeft, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_EyeLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_EyeLeft, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_EyeLeft, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_EyeLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_QuestionImg = lv_image_create(ui_ChatBotPage);
    lv_image_set_src(ui_QuestionImg, &ui_img_364131572);
    lv_obj_set_width(ui_QuestionImg, LV_SIZE_CONTENT);   /// 60
    lv_obj_set_height(ui_QuestionImg, LV_SIZE_CONTENT);    /// 60
    lv_obj_set_x(ui_QuestionImg, 125);
    lv_obj_set_y(ui_QuestionImg, -80);
    lv_obj_set_align(ui_QuestionImg, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_QuestionImg, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_QuestionImg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_opa(ui_QuestionImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_thinkImg = lv_image_create(ui_ChatBotPage);
    lv_image_set_src(ui_thinkImg, &ui_img_519153304);
    lv_obj_set_width(ui_thinkImg, LV_SIZE_CONTENT);   /// 60
    lv_obj_set_height(ui_thinkImg, LV_SIZE_CONTENT);    /// 60
    lv_obj_set_x(ui_thinkImg, 120);
    lv_obj_set_y(ui_thinkImg, -80);
    lv_obj_set_align(ui_thinkImg, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_thinkImg, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_thinkImg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_opa(ui_thinkImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ChatBotPage, ui_event_ChatBotPage, LV_EVENT_ALL, NULL);

    // load page
    lv_scr_load_anim(ui_ChatBotPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);

}

/////////////////// SCREEN deinit ////////////////////

void ui_ChatBotPage_deinit(void)
{

}
