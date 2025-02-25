#include "app_WeatherPage.h"
#include "ui_WeatherPage.h"

void ui_weatherpage_test()
{
    LocationInfo location = {"", ""};
    if(get_location_info(&location) != 0) {
        printf("Failed to get location info.\n");
        return -1;
    }

    printf("City: %s, Adcode: %s\n", location.city, location.adcode);

    WeatherInfo weather_info = { "", "", "", "" };
    if(get_weather_info_by_adcode(location.adcode, &weather_info) != 0) {
        printf("Failed to get weather info.\n");
    } else {
        printf("Weather: %s, Temperature: %s, Humidity: %s, Wind Power: %s\n",
               weather_info.weather, weather_info.temperature, weather_info.humidity, weather_info.windpower);
    }

}

///////////////////// VARIABLES ////////////////////


///////////////////// ANIMATIONS ////////////////////


///////////////////// FUNCTIONS ////////////////////


///////////////////// SCREEN init ////////////////////

void ui_WeatherPage_init(void)
{
    lv_obj_t * ui_WeatherPage = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_WeatherPage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_t * ui_ImgSun = lv_image_create(ui_WeatherPage);
    lv_image_set_src(ui_ImgSun, &ui_img_sun_png);
    lv_obj_set_width(ui_ImgSun, LV_SIZE_CONTENT);   /// 164
    lv_obj_set_height(ui_ImgSun, LV_SIZE_CONTENT);    /// 306
    lv_obj_set_x(ui_ImgSun, 95);
    lv_obj_set_y(ui_ImgSun, -45);
    lv_obj_set_align(ui_ImgSun, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImgSun, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_ImgSun, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_scale(ui_ImgSun, 208);

    lv_obj_t * ui_ImgCloud = lv_image_create(ui_WeatherPage);
    lv_image_set_src(ui_ImgCloud, &ui_img_clouds_png);
    lv_obj_set_width(ui_ImgCloud, LV_SIZE_CONTENT);   /// 168
    lv_obj_set_height(ui_ImgCloud, LV_SIZE_CONTENT);    /// 192
    lv_obj_set_x(ui_ImgCloud, 105);
    lv_obj_set_y(ui_ImgCloud, -60);
    lv_obj_set_align(ui_ImgCloud, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImgCloud, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_ImgCloud, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_scale(ui_ImgCloud, 206);

    lv_obj_t * ui_LabelsPanel = lv_obj_create(ui_WeatherPage);
    lv_obj_set_width(ui_LabelsPanel, 200);
    lv_obj_set_height(ui_LabelsPanel, 240);
    lv_obj_set_x(ui_LabelsPanel, -50);
    lv_obj_set_y(ui_LabelsPanel, 0);
    lv_obj_set_align(ui_LabelsPanel, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_LabelsPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_LabelsPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LabelsPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_LabelsPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_LabelsPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_LabelCity = lv_label_create(ui_LabelsPanel);
    lv_obj_set_width(ui_LabelCity, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelCity, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelCity, -10);
    lv_obj_set_y(ui_LabelCity, 0);
    lv_obj_set_align(ui_LabelCity, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_LabelCity, "深圳");
    lv_obj_set_style_text_font(ui_LabelCity, &ui_font_heiti22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_LabelDate = lv_label_create(ui_LabelsPanel);
    lv_obj_set_width(ui_LabelDate, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelDate, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelDate, -10);
    lv_obj_set_y(ui_LabelDate, 30);
    lv_obj_set_align(ui_LabelDate, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_LabelDate, "02.24 Monday");
    lv_obj_set_style_text_color(ui_LabelDate, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelDate, 128, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelDate, &ui_font_heiti22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_LabelTemp = lv_label_create(ui_LabelsPanel);
    lv_obj_set_width(ui_LabelTemp, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTemp, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTemp, 0);
    lv_obj_set_y(ui_LabelTemp, 5);
    lv_obj_set_align(ui_LabelTemp, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelTemp, "25°");
    lv_obj_set_style_text_font(ui_LabelTemp, &ui_font_NuberBig90, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_LabelWeather = lv_label_create(ui_LabelsPanel);
    lv_obj_set_width(ui_LabelWeather, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelWeather, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelWeather, -10);
    lv_obj_set_y(ui_LabelWeather, 150);
    lv_obj_set_align(ui_LabelWeather, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_LabelWeather, "多云");
    lv_obj_set_style_text_color(ui_LabelWeather, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelWeather, 128, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelWeather, &ui_font_heiti22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_BtmLabelsPanel = lv_obj_create(ui_WeatherPage);
    lv_obj_set_width(ui_BtmLabelsPanel, 320);
    lv_obj_set_height(ui_BtmLabelsPanel, 50);
    lv_obj_set_x(ui_BtmLabelsPanel, 0);
    lv_obj_set_y(ui_BtmLabelsPanel, 95);
    lv_obj_set_align(ui_BtmLabelsPanel, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_BtmLabelsPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtmLabelsPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtmLabelsPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtmLabelsPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtmLabelsPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_IconWind = lv_label_create(ui_BtmLabelsPanel);
    lv_obj_set_width(ui_IconWind, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconWind, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconWind, -70);
    lv_obj_set_y(ui_IconWind, -2);
    lv_obj_set_align(ui_IconWind, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_IconWind, "");
    lv_obj_set_style_text_color(ui_IconWind, lv_color_hex(0xEEB27D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_IconWind, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_IconWind, &ui_font_iconfont30, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_LabelWind = lv_label_create(ui_BtmLabelsPanel);
    lv_obj_set_width(ui_LabelWind, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelWind, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelWind, -35);
    lv_obj_set_y(ui_LabelWind, 2);
    lv_obj_set_align(ui_LabelWind, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelWind, "≤3");
    lv_obj_set_style_text_color(ui_LabelWind, lv_color_hex(0xEEB27D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelWind, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelWind, &ui_font_heiti22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_IconHumi = lv_label_create(ui_BtmLabelsPanel);
    lv_obj_set_width(ui_IconHumi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconHumi, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconHumi, 35);
    lv_obj_set_y(ui_IconHumi, 0);
    lv_obj_set_align(ui_IconHumi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_IconHumi, "");
    lv_obj_set_style_text_color(ui_IconHumi, lv_color_hex(0x42D2F4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_IconHumi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_IconHumi, &ui_font_iconfont30, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_LabelHumi = lv_label_create(ui_BtmLabelsPanel);
    lv_obj_set_width(ui_LabelHumi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHumi, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHumi, 75);
    lv_obj_set_y(ui_LabelHumi, 2);
    lv_obj_set_align(ui_LabelHumi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHumi, "75%");
    lv_obj_set_style_text_color(ui_LabelHumi, lv_color_hex(0x42D2F4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHumi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHumi, &ui_font_heiti22, LV_PART_MAIN | LV_STATE_DEFAULT);

    // load page
    lv_scr_load_anim(ui_WeatherPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_WeatherPage_deinit()
{
    // deinit
}