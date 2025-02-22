#include "ui_ChatBotPage.h"

///////////////////// VARIABLES ////////////////////

lv_obj_t * ui_EyesPanel;
lv_obj_t * ui_EyesVerMovePanel;
lv_obj_t * ui_QuestionImg;
lv_obj_t * ui_thinkImg;
lv_obj_t * ui_HandImg;
lv_obj_t * ui_EyeRight;
lv_obj_t * ui_EyeLeft;
lv_obj_t * ui_MouthPanel;
lv_obj_t * ui_Mouth;

///////////////////// ANIMATIONS ////////////////////

static void _IdleMove1_Animation(void)
{
    int16_t y_pos_now = -25;
    int16_t x_pos_now = 0;
    int16_t hight_now = 80;
    lv_lib_anim_user_animation(ui_EyesPanel, 0, 500, y_pos_now, y_pos_now-20, 0, 0, 0, 0, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_y);
    y_pos_now -= 20;
    lv_lib_anim_user_animation(ui_EyesPanel, 0, 500, x_pos_now, x_pos_now-20, 0, 0, 0, 0, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_x);
    x_pos_now -= 20;
    lv_lib_anim_user_animation(ui_EyesPanel, 1000, 100, hight_now, 10, 0, 100, 0, 2, lv_anim_path_ease_out, lv_lib_anim_callback_set_hight);
    // hight_now += 0;
    lv_lib_anim_user_animation(ui_EyesPanel, 1500, 500, x_pos_now, x_pos_now+20, 0, 0, 0, 0, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_x);
    x_pos_now += 20;
    lv_lib_anim_user_animation(ui_EyesPanel, 2000, 100, hight_now, 10, 0, 100, 0, 2, lv_anim_path_ease_out, lv_lib_anim_callback_set_hight);
    // hight_now += 0;
    lv_lib_anim_user_animation(ui_EyesPanel, 3000, 500, x_pos_now, x_pos_now+40, 0, 0, 0, 0, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_x);
    x_pos_now += 40;
    lv_lib_anim_user_animation(ui_EyesPanel, 3000, 500, y_pos_now, y_pos_now+40, 0, 0, 0, 0, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_y);
    y_pos_now += 40;
    lv_lib_anim_user_animation(ui_EyesPanel, 4000, 100, hight_now, 10, 0, 100, 0, 2, lv_anim_path_ease_out, lv_lib_anim_callback_set_hight);
    // hight_now += 0;
    lv_lib_anim_user_animation(ui_EyesPanel, 5000, 500, x_pos_now, x_pos_now-40, 0, 0, 0, 0, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_x);
    x_pos_now -= 40;
    lv_lib_anim_user_animation(ui_EyesPanel, 5000, 500, y_pos_now, y_pos_now-20, 0, 0, 0, 0, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_y);
    y_pos_now -= 20;
}

static void _IdleMove2_Animation(void)
{
    int16_t VerPanel_y_pos_now = 0;
    int16_t MainPanel_y_pos_now = -25;
    lv_lib_anim_user_animation(ui_EyesVerMovePanel, 0, 500, VerPanel_y_pos_now, VerPanel_y_pos_now-20, 0, 0, 0, 0, lv_anim_path_ease_out, lv_lib_anim_callback_set_y);
    VerPanel_y_pos_now -= 20;
    lv_lib_anim_user_animation(ui_EyesVerMovePanel, 2500, 500, VerPanel_y_pos_now, VerPanel_y_pos_now+20, 0, 0, 0, 0, lv_anim_path_ease_out, lv_lib_anim_callback_set_y);
    VerPanel_y_pos_now += 20;

    lv_lib_anim_user_animation(ui_EyesPanel, 0, 500, MainPanel_y_pos_now, MainPanel_y_pos_now+20, 0, 0, 0, 0, lv_anim_path_ease_out, lv_lib_anim_callback_set_y);
    MainPanel_y_pos_now += 20;
    lv_lib_anim_user_animation(ui_EyesPanel, 2500, 500, MainPanel_y_pos_now, MainPanel_y_pos_now-20, 0, 0, 0, 0, lv_anim_path_ease_out, lv_lib_anim_callback_set_y);
    MainPanel_y_pos_now -= 20;

    lv_lib_anim_user_animation(ui_thinkImg, 0, 500, -100, 100, 0, 750, 0, 3, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_image_angle);
    lv_lib_anim_user_animation(ui_thinkImg, 0, 500, 0, 255, 0, 0, 0, 0, lv_anim_path_linear, lv_lib_anim_callback_set_opacity);
    lv_lib_anim_user_animation(ui_thinkImg, 3000, 500, 255, 0, 0, 0, 0, 0, lv_anim_path_linear, lv_lib_anim_callback_set_opacity);
}

static void _ListenMove_Animation(void)
{
    int16_t eye_width_now = 80;
    int16_t eye_hight_now = 80;
    lv_lib_anim_user_animation(ui_EyeRight, 0, 100, eye_width_now, eye_width_now-30, 0, 0, 0, 0, lv_anim_path_ease_out, lv_lib_anim_callback_set_width);
    lv_lib_anim_user_animation(ui_EyeLeft, 0, 100, eye_width_now, eye_width_now-30, 0, 0, 0, 0, lv_anim_path_ease_out, lv_lib_anim_callback_set_width);
    eye_width_now -= 30;
    lv_lib_anim_user_animation(ui_QuestionImg, 0, 500, 0, 255, 0, 0, 0, 0, lv_anim_path_linear, lv_lib_anim_callback_set_opacity);
    lv_lib_anim_user_animation(ui_QuestionImg, 0, 500, -100, 100, 0, 750, 0, 4, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_image_angle);

    lv_lib_anim_user_animation(ui_EyeRight, 1000, 100, eye_width_now, eye_width_now+30, 0, 100, 0, 1, lv_anim_path_ease_out, lv_lib_anim_callback_set_width);
    lv_lib_anim_user_animation(ui_EyeLeft, 1000, 100, eye_width_now, eye_width_now+30, 0, 100, 0, 1, lv_anim_path_ease_out, lv_lib_anim_callback_set_width);
    lv_lib_anim_user_animation(ui_EyeRight, 1000, 100, eye_hight_now, eye_hight_now-70, 0, 100, 0, 1, lv_anim_path_ease_out, lv_lib_anim_callback_set_hight);
    lv_lib_anim_user_animation(ui_EyeLeft, 1000, 100, eye_hight_now, eye_hight_now-70, 0, 100, 0, 1, lv_anim_path_ease_out, lv_lib_anim_callback_set_hight);

    lv_lib_anim_user_animation(ui_EyeRight, 2000, 100, eye_width_now, eye_width_now+30, 0, 100, 0, 1, lv_anim_path_ease_out, lv_lib_anim_callback_set_width);
    lv_lib_anim_user_animation(ui_EyeLeft, 2000, 100, eye_width_now, eye_width_now+30, 0, 100, 0, 1, lv_anim_path_ease_out, lv_lib_anim_callback_set_width);
    lv_lib_anim_user_animation(ui_EyeRight, 2000, 100, eye_hight_now, eye_hight_now-70, 0, 100, 0, 1, lv_anim_path_ease_out, lv_lib_anim_callback_set_hight);
    lv_lib_anim_user_animation(ui_EyeLeft, 2000, 100, eye_hight_now, eye_hight_now-70, 0, 100, 0, 1, lv_anim_path_ease_out, lv_lib_anim_callback_set_hight);

    lv_lib_anim_user_animation(ui_HandImg, 2000, 500, 0, 255, 0, 0, 0, 0, lv_anim_path_linear, lv_lib_anim_callback_set_opacity);
    lv_lib_anim_user_animation(ui_HandImg, 2000, 500, -250, -150, 0, 750, 0, 2, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_image_angle);

    lv_lib_anim_user_animation(ui_QuestionImg, 3500, 500, 255, 0, 0, 0, 0, 0, lv_anim_path_linear, lv_lib_anim_callback_set_opacity);
    lv_lib_anim_user_animation(ui_HandImg, 3500, 500, 255, 0, 0, 0, 0, 0, lv_anim_path_linear, lv_lib_anim_callback_set_opacity);

    lv_lib_anim_user_animation(ui_EyeRight, 4000, 100, eye_width_now, eye_width_now+30, 0, 0, 0, 0, lv_anim_path_ease_out, lv_lib_anim_callback_set_width);
    lv_lib_anim_user_animation(ui_EyeLeft, 4000, 100, eye_width_now, eye_width_now+30, 0, 0, 0, 0, lv_anim_path_ease_out, lv_lib_anim_callback_set_width);
    eye_width_now += 30;
}

static void _SpeakMove_Animation(void)
{
    int16_t mouth_panel_y_pos_now = 95;
    int16_t mouth_y_pos_now = -40;
    int16_t eye_panel_hight_now = 80;
    lv_obj_set_style_bg_opa(ui_Mouth, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_lib_anim_user_animation(ui_Mouth, 0, 150, mouth_y_pos_now, mouth_y_pos_now-10, 0, 150, 0, 6, lv_anim_path_ease_out, lv_lib_anim_callback_set_y);
    lv_lib_anim_user_animation(ui_MouthPanel, 0, 150, mouth_panel_y_pos_now, mouth_panel_y_pos_now+10, 0, 150, 0, 6, lv_anim_path_ease_out, lv_lib_anim_callback_set_y);
    // blink
    lv_lib_anim_user_animation(ui_EyesPanel, 500, 200, eye_panel_hight_now, 10, 0, 200, 0, 1, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_hight);
    lv_lib_anim_user_animation(ui_EyesPanel, 2000, 200, eye_panel_hight_now, 10, 0, 200, 0, 1, lv_anim_path_ease_in_out, lv_lib_anim_callback_set_hight);

    lv_lib_anim_user_animation(ui_Mouth, 1500, 150, mouth_y_pos_now, mouth_y_pos_now-10, 0, 150, 0, 4, lv_anim_path_ease_out, lv_lib_anim_callback_set_y);
    lv_lib_anim_user_animation(ui_MouthPanel, 1500, 150, mouth_panel_y_pos_now, mouth_panel_y_pos_now+10, 0, 150, 0, 4, lv_anim_path_ease_out, lv_lib_anim_callback_set_y);

}

///////////////////// FUNCTIONS ////////////////////

void ui_ChatBotPage_Objs_reinit(void)
{
    lv_obj_set_width(ui_EyesPanel, 210);
    lv_obj_set_height(ui_EyesPanel, 80);
    lv_obj_set_x(ui_EyesPanel, 0);
    lv_obj_set_y(ui_EyesPanel, -25);

    lv_obj_set_width(ui_EyesVerMovePanel, 210);
    lv_obj_set_height(ui_EyesVerMovePanel, 80);
    lv_obj_set_x(ui_EyesVerMovePanel, 0);
    lv_obj_set_y(ui_EyesVerMovePanel, 0);

    lv_obj_set_width(ui_EyeRight, 80);
    lv_obj_set_height(ui_EyeRight, 80);
    lv_obj_set_x(ui_EyeRight, 60);
    lv_obj_set_y(ui_EyeRight, 0);

    lv_obj_set_width(ui_EyeLeft, 80);
    lv_obj_set_height(ui_EyeLeft, 80);
    lv_obj_set_x(ui_EyeLeft, -60);
    lv_obj_set_y(ui_EyeLeft, 0);

    lv_obj_set_width(ui_EyeLeft, 80);
    lv_obj_set_height(ui_EyeLeft, 80);
    lv_obj_set_x(ui_EyeLeft, -60);
    lv_obj_set_y(ui_EyeLeft, 0);

    lv_obj_set_width(ui_Mouth, 60);
    lv_obj_set_height(ui_Mouth, 60);
    lv_obj_set_x(ui_Mouth, 0);
    lv_obj_set_y(ui_Mouth, -40);

    lv_obj_set_style_bg_opa(ui_Mouth, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_opa(ui_QuestionImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_opa(ui_thinkImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_opa(ui_HandImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void ui_event_ChatBotPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_active()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_active());
        // _IdleMove1_Animation();
        // _IdleMove2_Animation();
        // _ListenMove_Animation();
        _SpeakMove_Animation();
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_active()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_active());
        lv_anim_delete_all();
        ui_ChatBotPage_Objs_reinit();
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
    lv_obj_remove_flag(ui_EyesPanel, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_EyesPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_EyesPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_EyesPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_EyesPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_EyesVerMovePanel = lv_obj_create(ui_EyesPanel);
    lv_obj_set_width(ui_EyesVerMovePanel, 210);
    lv_obj_set_height(ui_EyesVerMovePanel, 80);
    lv_obj_set_align(ui_EyesVerMovePanel, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_EyesVerMovePanel, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_EyesVerMovePanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_EyesVerMovePanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_EyesVerMovePanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_EyesVerMovePanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_EyeRight = lv_button_create(ui_EyesVerMovePanel);
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

    ui_EyeLeft = lv_button_create(ui_EyesVerMovePanel);
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

    ui_MouthPanel = lv_obj_create(ui_ChatBotPage);
    lv_obj_set_width(ui_MouthPanel, 80);
    lv_obj_set_height(ui_MouthPanel, 80);
    lv_obj_set_x(ui_MouthPanel, 0);
    lv_obj_set_y(ui_MouthPanel, 95);
    lv_obj_set_align(ui_MouthPanel, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_MouthPanel, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_MouthPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MouthPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_MouthPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_MouthPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Mouth = lv_button_create(ui_MouthPanel);
    lv_obj_set_width(ui_Mouth, 60);
    lv_obj_set_height(ui_Mouth, 60);
    lv_obj_set_x(ui_Mouth, 0);
    lv_obj_set_y(ui_Mouth, -40);
    lv_obj_set_align(ui_Mouth, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Mouth, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Mouth, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Mouth, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Mouth, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Mouth, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_QuestionImg = lv_image_create(ui_ChatBotPage);
    lv_image_set_src(ui_QuestionImg, &ui_img_364131572);
    lv_obj_set_width(ui_QuestionImg, LV_SIZE_CONTENT);   /// 60
    lv_obj_set_height(ui_QuestionImg, LV_SIZE_CONTENT);    /// 60
    lv_obj_set_x(ui_QuestionImg, 125);
    lv_obj_set_y(ui_QuestionImg, -80);
    lv_obj_set_align(ui_QuestionImg, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_QuestionImg, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_obj_set_style_opa(ui_QuestionImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_thinkImg = lv_image_create(ui_ChatBotPage);
    lv_image_set_src(ui_thinkImg, &ui_img_519153304);
    lv_obj_set_width(ui_thinkImg, LV_SIZE_CONTENT);   /// 60
    lv_obj_set_height(ui_thinkImg, LV_SIZE_CONTENT);    /// 60
    lv_obj_set_x(ui_thinkImg, 120);
    lv_obj_set_y(ui_thinkImg, -80);
    lv_obj_set_align(ui_thinkImg, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_thinkImg, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_obj_set_style_opa(ui_thinkImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_HandImg = lv_image_create(ui_ChatBotPage);
    lv_image_set_src(ui_HandImg, &ui_img_1829120061);
    lv_obj_set_width(ui_HandImg, LV_SIZE_CONTENT);   /// 64
    lv_obj_set_height(ui_HandImg, LV_SIZE_CONTENT);    /// 64
    lv_obj_set_x(ui_HandImg, 0);
    lv_obj_set_y(ui_HandImg, 55);
    lv_obj_set_align(ui_HandImg, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_HandImg, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_image_set_rotation(ui_HandImg, -350);
    lv_obj_set_style_opa(ui_HandImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ChatBotPage, ui_event_ChatBotPage, LV_EVENT_ALL, NULL);

    // load page
    lv_scr_load_anim(ui_ChatBotPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);

}

/////////////////// SCREEN deinit ////////////////////

void ui_ChatBotPage_deinit(void)
{

}
