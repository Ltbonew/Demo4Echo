#ifndef LV_LIB_CONF_H
#define LV_LIB_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../lvgl/lvgl.h"

/*=========================
   MODULE CONFIGURATION
 =========================*/

/* Enable or disable the Stack module */
#ifndef LV_USE_LIB_STACK
#define LV_USE_LIB_STACK 1  // 1: Enable, 0: Disable
#endif

/* Enable or disable the Page Manager module */
#ifndef LV_USE_LIB_PAGE_MANAGER
#define LV_USE_LIB_PAGE_MANAGER 1  // 1: Enable, 0: Disable
#endif

/*=========================
   DEPENDENCY MANAGEMENT
 =========================*/

/* If PAGE_MANAGER is enabled, ensure STACK is also enabled */
#if LV_USE_LIB_PAGE_MANAGER && !LV_USE_LIB_STACK
#undef LV_USE_LIB_STACK
#define LV_USE_LIB_STACK 1
#endif

/*=========================
   MODULE SPECIFIC CONFIGS
 =========================*/

/* Stack module configuration */
#if LV_USE_LIB_STACK
// Add specific configurations for STACK if needed
#endif

/* Page Manager module configuration */
#if LV_USE_LIB_PAGE_MANAGER
  // Add specific configurations for PAGE_MANAGER if needed
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LV_LIB_CONF_H
