#include "ui_SettingPage.h"

///////////////////// VARIABLES ////////////////////
typedef enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
} lv_menu_builder_variant_t;

lv_obj_t * root_page;

///////////////////// ANIMATIONS ////////////////////


///////////////////// FUNCTIONS ////////////////////

static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
                              lv_menu_builder_variant_t builder_variant)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);

    lv_obj_t * icon_label = NULL;
    lv_obj_t * label = NULL;

    if(icon) {
        icon_label = lv_label_create(obj);
        lv_label_set_text(icon_label, icon);
        lv_obj_set_style_text_font(icon_label, &ui_font_iconfont20, LV_PART_MAIN);
    }

    if(txt) {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN);
    }

    if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
        lv_obj_add_flag(icon_label, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(icon_label, label);
    }

    return obj;
}

static lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,
                                int32_t val)
{
    lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

    lv_obj_t * slider = lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    if(icon == NULL) {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    return obj;
}

static lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk)
{
    lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t * sw = lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    return obj;
}

///////////////////// Call Back ////////////////////

static void back_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * menu = lv_event_get_user_data(e);

    if(lv_menu_back_button_is_root(menu, obj)) {
        lv_lib_pm_OpenPrePage(&page_manager);
        // lv_obj_t * mbox1 = lv_msgbox_create(NULL);
        // lv_msgbox_add_title(mbox1, "Hello");
        // lv_msgbox_add_text(mbox1, "Root back btn click.");
        // lv_msgbox_add_close_button(mbox1);
    }
    
}

static void switch_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * menu = lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
            lv_menu_set_page(menu, NULL);
            lv_menu_set_sidebar_page(menu, root_page);
            lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED,
                              NULL);
        }
        else {
            lv_menu_set_sidebar_page(menu, NULL);
            lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
            lv_menu_set_page(menu, root_page);
        }
    }
}

///////////////////// SCREEN init ////////////////////

void ui_SettingPage_init()
{
    lv_obj_t * SettingPage = lv_obj_create(NULL);
    lv_obj_t * menu = lv_menu_create(SettingPage);
    // lv_obj_set_style_bg_color(menu, lv_color_white(),  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);

    lv_menu_set_mode_root_back_button(menu, LV_MENU_ROOT_BACK_BUTTON_ENABLED);
    lv_obj_t * back_btn = lv_menu_get_main_header_back_button(menu);
    if (back_btn) {
        lv_obj_set_width(back_btn, 45);
        lv_obj_set_height(back_btn, 35);
        lv_obj_set_style_text_font(back_btn, &lv_font_montserrat_28,  LV_PART_MAIN);
    }
    lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);
    lv_obj_set_size(menu, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
    lv_obj_center(menu);
    /*Create a root page*/
    root_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_obj_t * root_section1 = lv_menu_section_create(root_page);
    lv_obj_set_style_bg_color(root_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);

    // common sub page
    lv_obj_t * sub_common_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_common_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_common_page);
    lv_obj_t * sub_common_section1 = lv_menu_section_create(sub_common_page);
    lv_obj_set_style_bg_color(sub_common_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    create_slider(sub_common_section1, "", "Brightness", 0, 100, 50);
    create_slider(sub_common_section1, "", "Sound", 0, 100, 50);
    // common root cont
    lv_obj_t * root_common_cont = create_text(root_section1, "", "Common", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, root_common_cont, sub_common_page);
    
    // time sub page
    lv_obj_t * sub_time_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_time_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_time_page);
    lv_obj_t * sub_time_section1 = lv_menu_section_create(sub_time_page);
    lv_obj_set_style_bg_color(sub_time_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t * sub_time_cont = create_switch(sub_time_section1, "", "Autosync Time", true);
    
    // time root cont
    lv_obj_t * root_time_cont = create_text(root_section1, "", "Time", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, root_time_cont, sub_time_page);
    
    
    lv_menu_set_sidebar_page(menu, NULL);
    lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
    lv_menu_set_page(menu, root_page);

    // load page
    lv_scr_load_anim(SettingPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_SettingPage_deinit()
{
    // deinit
}