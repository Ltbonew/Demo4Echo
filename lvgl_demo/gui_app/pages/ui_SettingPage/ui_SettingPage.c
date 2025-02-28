#include "ui_SettingPage.h"

///////////////////// VARIABLES ////////////////////

typedef enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
} lv_menu_builder_variant_t;

lv_obj_t * root_page;
lv_obj_t * sub_time_set_cont;
lv_obj_t * sub_date_set_cont;
lv_obj_t * time_roller_hour;
lv_obj_t * time_roller_minute;
lv_obj_t * time_roller_second;
// start year is '2025'
lv_obj_t * date_roller_year ;
lv_obj_t * date_roller_month;
lv_obj_t * date_roller_day;

ui_system_para_t ui_temp_sys_para; // template system paras

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
        // open pre page & set system hardware paras

        lv_lib_pm_OpenPrePage(&page_manager);
        // lv_obj_t * mbox1 = lv_msgbox_create(NULL);
        // lv_msgbox_add_title(mbox1, "Hello");
        // lv_msgbox_add_text(mbox1, "Root back btn click.");
        // lv_msgbox_add_close_button(mbox1);
    }
    
}

static void auto_time_switch_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * sub_time_section2 = lv_event_get_user_data(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
            lv_obj_remove_flag(sub_time_set_cont, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_remove_flag(sub_date_set_cont, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_opa(sub_time_section2, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(sub_time_section2, 0, LV_STATE_DEFAULT);
        }
        else {
            lv_obj_add_flag(sub_time_set_cont, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(sub_date_set_cont, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_opa(sub_time_section2, 255, LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(sub_time_section2, 255, LV_STATE_DEFAULT);
        }
    }
}

static void sidebar_mode_handler(lv_event_t * e)
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
    // root section1 
    // create_text(root_page, NULL, "Main", LV_MENU_ITEM_BUILDER_VARIANT_1);
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
    
    // time set sub page in time sub page
    lv_obj_t * sub_time_set_page = lv_menu_page_create(menu, NULL);
    lv_obj_t * sub_time_set_section1 = lv_menu_section_create(sub_time_set_page);
    lv_obj_set_style_bg_color(sub_time_set_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t * time_roller_cont = lv_menu_cont_create(sub_time_set_section1);
    time_roller_hour = lv_roller_create(time_roller_cont);
    lv_obj_set_width(time_roller_hour, 95);
    lv_obj_set_height(time_roller_hour, 120);
    lv_obj_set_style_text_color(time_roller_hour, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(time_roller_hour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(time_roller_hour, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_roller_set_options(time_roller_hour,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23",
                          LV_ROLLER_MODE_INFINITE);
    time_roller_minute = lv_roller_create(time_roller_cont);
    lv_obj_set_width(time_roller_minute, 95);
    lv_obj_set_height(time_roller_minute, 120);
    lv_obj_set_style_text_color(time_roller_minute, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(time_roller_minute, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(time_roller_minute, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_roller_set_options(time_roller_minute,
                          "00\n01\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_INFINITE);
    time_roller_second = lv_roller_create(time_roller_cont);
    lv_obj_set_width(time_roller_second, 95);
    lv_obj_set_height(time_roller_second, 120);
    lv_obj_set_style_text_color(time_roller_second, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(time_roller_second, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(time_roller_second, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_roller_set_options(time_roller_second,
                          "00\n01\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_INFINITE);
    // date set sub page in time sub page
    lv_obj_t * sub_date_set_page = lv_menu_page_create(menu, NULL);
    lv_obj_t * sub_date_set_section1 = lv_menu_section_create(sub_date_set_page);
    lv_obj_set_style_bg_color(sub_date_set_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t * date_roller_cont = lv_menu_cont_create(sub_date_set_section1);
    date_roller_year = lv_roller_create(date_roller_cont);
    lv_obj_set_width(date_roller_year, 95);
    lv_obj_set_height(date_roller_year, 120);
    lv_obj_set_style_text_color(date_roller_year, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(date_roller_year, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(date_roller_year, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_roller_set_options(date_roller_year,
                          "2025\n2026\n2027\n2028\n2029\n2030\n2031\n2032\n2033\n2034\n2035\n2036\n2037\n2038\n2039\n2040",
                          LV_ROLLER_MODE_INFINITE);
    date_roller_month = lv_roller_create(date_roller_cont);
    lv_obj_set_width(date_roller_month, 95);
    lv_obj_set_height(date_roller_month, 120);
    lv_obj_set_style_text_color(date_roller_month, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(date_roller_month, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(date_roller_month, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_roller_set_options(date_roller_month,
                          "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12",
                          LV_ROLLER_MODE_INFINITE);
    date_roller_day = lv_roller_create(date_roller_cont);
    lv_obj_set_width(date_roller_day, 95);
    lv_obj_set_height(date_roller_day, 120);
    lv_obj_set_style_text_color(date_roller_day, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(date_roller_day, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(date_roller_day, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_roller_set_options(date_roller_day,
                          "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31",
                          LV_ROLLER_MODE_INFINITE);

    // time sub page
    lv_obj_t * sub_time_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_time_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_time_page);
    lv_obj_t * sub_time_section1 = lv_menu_section_create(sub_time_page);
    lv_obj_set_style_bg_color(sub_time_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t * sub_auto_time_switch_cont = create_switch(sub_time_section1, "", "Auto update", true);
    create_text(sub_time_page, NULL, " ", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_obj_t * sub_time_section2 = lv_menu_section_create(sub_time_page);
    lv_obj_set_style_bg_color(sub_time_section2, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    sub_time_set_cont = create_text(sub_time_section2, "", "Time Set", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, sub_time_set_cont, sub_time_set_page);
    sub_date_set_cont = create_text(sub_time_section2, "", "Date Set", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, sub_date_set_cont, sub_date_set_page);
    lv_obj_remove_flag(sub_time_set_cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(sub_date_set_cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(sub_time_section2, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(sub_time_section2, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(lv_obj_get_child(sub_auto_time_switch_cont,2), auto_time_switch_event_cb, LV_EVENT_VALUE_CHANGED, sub_time_section2);
    // time root cont
    lv_obj_t * root_time_cont = create_text(root_section1, "", "Time", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, root_time_cont, sub_time_page);

    // location sub page
    lv_obj_t * sub_location_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_location_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_location_page);
    lv_obj_t * sub_location_section1 = lv_menu_section_create(sub_location_page);
    lv_obj_set_style_bg_color(sub_location_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t * sub_location_cont = create_switch(sub_location_section1, "", "IP Locating", false);
    // location root cont
    lv_obj_t * root_location_cont = create_text(root_section1, "", "Location", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, root_location_cont, sub_location_page);

    
    // root section2
    create_text(root_page, NULL, "Others", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_obj_t * root_section2 = lv_menu_section_create(root_page);
    lv_obj_set_style_bg_color(root_section2, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);

    // software sub page in about sub page
    lv_obj_t * sub_software_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_software_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_software_page);
    lv_obj_t * sub_software_cont = create_text(sub_software_page, NULL, "Software version is V1.1", LV_MENU_ITEM_BUILDER_VARIANT_1);
    // legal sub page in about sub page
    lv_obj_t * sub_legal_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_legal_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_legal_page);
    lv_obj_t * sub_legal_cont = create_text(sub_legal_page, NULL, 
                                            "Legal Info:", 
                                            LV_MENU_ITEM_BUILDER_VARIANT_1);

    // about sub page
    lv_obj_t * sub_about_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_about_page);
    lv_obj_t * sub_about_section1 = lv_menu_section_create(sub_about_page);
    lv_obj_set_style_bg_color(sub_about_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t * sub_about_cont = create_text(sub_about_section1, "", "Software Info", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, sub_about_cont, sub_software_page);
    sub_about_cont = create_text(sub_about_section1, "", "Legal Info", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, sub_about_cont, sub_legal_page);

    // about root cont
    lv_obj_t * root_about_cont = create_text(root_section2, "", "About", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, root_about_cont, sub_about_page);
    
    // menu mode sub page
    lv_obj_t * sub_mode_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_mode_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_mode_page);
    lv_obj_t * sub_mode_section1 = lv_menu_section_create(sub_mode_page);
    lv_obj_set_style_bg_color(sub_mode_section1, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t * sub_mode_cont = create_switch(sub_mode_section1, "", "Sidebar enable", false);
    lv_obj_add_event_cb(lv_obj_get_child(sub_mode_cont, 2), sidebar_mode_handler, LV_EVENT_VALUE_CHANGED, menu);

    // menu mode root cont
    lv_obj_t * root_mode_cont = create_text(root_section2, "", "Menu mode", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, root_mode_cont, sub_mode_page);

    lv_menu_set_sidebar_page(menu, NULL);
    lv_menu_clear_history(menu);
    lv_menu_set_page(menu, root_page);

    // load page
    lv_scr_load_anim(SettingPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_SettingPage_deinit()
{
    // deinit
}