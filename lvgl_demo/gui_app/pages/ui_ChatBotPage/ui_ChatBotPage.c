#include "ui_ChatBotPage.h"

///////////////////// ANIMATIONS ////////////////////



///////////////////// FUNCTIONS ////////////////////


///////////////////// SCREEN init ////////////////////

void ui_ChatBotPage_init(void)
{
    lv_obj_t * ui_ChatBotPage = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_ChatBotPage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_t * ui_EyesPanel = lv_obj_create(ui_ChatBotPage);
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

    lv_obj_t * ui_EyesVerMovePanel = lv_obj_create(ui_EyesPanel);
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

    lv_obj_t * ui_QuestionImg = lv_image_create(ui_ChatBotPage);
    lv_image_set_src(ui_QuestionImg, &ui_img_364131572);
    lv_obj_set_width(ui_QuestionImg, LV_SIZE_CONTENT);   /// 60
    lv_obj_set_height(ui_QuestionImg, LV_SIZE_CONTENT);    /// 60
    lv_obj_set_x(ui_QuestionImg, 125);
    lv_obj_set_y(ui_QuestionImg, -80);
    lv_obj_set_align(ui_QuestionImg, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_QuestionImg, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_QuestionImg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_opa(ui_QuestionImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_thinkImg = lv_image_create(ui_ChatBotPage);
    lv_image_set_src(ui_thinkImg, &ui_img_519153304);
    lv_obj_set_width(ui_thinkImg, LV_SIZE_CONTENT);   /// 60
    lv_obj_set_height(ui_thinkImg, LV_SIZE_CONTENT);    /// 60
    lv_obj_set_x(ui_thinkImg, 120);
    lv_obj_set_y(ui_thinkImg, -80);
    lv_obj_set_align(ui_thinkImg, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_thinkImg, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_thinkImg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_opa(ui_thinkImg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // load page
    lv_scr_load_anim(ui_ChatBotPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);

}

/////////////////// SCREEN deinit ////////////////////

void ui_ChatBotPage_deinit(void)
{

}
