#ifndef LV_LIB_ANIMATION_H
#define LV_LIB_ANIMATION_H

#include "../lv_lib_conf.h"

#if LV_USE_LIB_ANIMATION 

void lv_lib_anim_callback_set_x(void * var, int32_t v);
void lv_lib_anim_callback_set_y(void * var, int32_t v);
void lv_lib_anim_callback_set_hight(void * var, int32_t v);
void lv_lib_anim_callback_set_width(void * var, int32_t v);
void lv_lib_anim_callback_set_image_angle(void * var, int32_t v);
void lv_lib_anim_callback_set_scale(void * var, int32_t v);
void lv_lib_anim_callback_set_opacity(void * var, int32_t v);

#endif  /*LV_USE_LIB_TEMPLATE*/

#endif // LV_LIB_TEMPLATE_H