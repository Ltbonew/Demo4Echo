#include "lv_lib_animation.h"

void lv_lib_anim_callback_set_x(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

void lv_lib_anim_callback_set_y(void * var, int32_t v)
{
    lv_obj_set_y(var, v);
}

void lv_lib_anim_callback_set_hight(void * var, int32_t v)
{
    lv_obj_set_height(var, v);
}

void lv_lib_anim_callback_set_width(void * var, int32_t v)
{
    lv_obj_set_width(var, v);
}

void lv_lib_anim_callback_set_image_angle(void * var, int32_t v)
{
    lv_image_set_rotation(var, v);
}

void lv_lib_anim_callback_set_scale(void * var, int32_t v)
{
    lv_image_set_scale(var, v);
}

void lv_lib_anim_callback_set_opacity(void * var, int32_t v)
{
    lv_obj_set_style_opa(var, v, LV_PART_MAIN | LV_STATE_DEFAULT);
}



