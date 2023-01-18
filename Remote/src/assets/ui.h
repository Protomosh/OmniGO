// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.3.3
// PROJECT: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

void BootLoading_Animation(lv_obj_t * TargetObject, int delay);
void ui_event_BootScreen(lv_event_t * e);
extern lv_obj_t * ui_BootScreen;
extern lv_obj_t * ui_BootLogo;
extern lv_obj_t * ui_BootSpinner;
extern lv_obj_t * ui_LoadingLabel;
extern lv_obj_t * ui_BootHeader;
extern lv_obj_t * ui_BootScreen_Panel1;
extern lv_obj_t * ui_BootScreenLoadingTextBox;
void ui_event_MainScreen(lv_event_t * e);
extern lv_obj_t * ui_MainScreen;
extern lv_obj_t * ui_BatImg1;
extern lv_obj_t * ui_Batbar1;
extern lv_obj_t * ui_SteerBar;
extern lv_obj_t * ui_SteerLabel;
extern lv_obj_t * ui_throttleBar;
extern lv_obj_t * ui_ThrottleLabel;
extern lv_obj_t * ui_Batbar2;
extern lv_obj_t * ui_BatImg2;
extern lv_obj_t * ui_MainScreen_Panel2;
extern lv_obj_t * ui_MainScreen_Label2;
extern lv_obj_t * ui_MainScreen_ImgButton2;
extern lv_obj_t * ui_radarPlaceholder;
void ui_event_controllerBtn(lv_event_t * e);
extern lv_obj_t * ui_controllerBtn;
void ui_event_pwrBtn(lv_event_t * e);
extern lv_obj_t * ui_pwrBtn;
void ui_event_DebugScreen(lv_event_t * e);
extern lv_obj_t * ui_DebugScreen;
extern lv_obj_t * ui_Screen1_Colorwheel1;
extern lv_obj_t * ui_Screen1_Button1;
extern lv_obj_t * ui_Screen1_Button2;
extern lv_obj_t * ui_Screen1_Button3;
extern lv_obj_t * ui_Screen1_Roller1;
extern lv_obj_t * ui_Screen1_TextArea1;

void ExternalControl(lv_event_t * e);
void LocalControl(lv_event_t * e);
void motorPowerOn(lv_event_t * e);
void powerOff(lv_event_t * e);

LV_IMG_DECLARE(ui_img_logo_smallest_png);    // assets\logo_smallest.png
LV_IMG_DECLARE(ui_img_687825122);    // assets\icons8-empty-battery-50.png
LV_IMG_DECLARE(ui_img_874557855);    // assets\power-off-button-64.png
LV_IMG_DECLARE(ui_img_1362995411);    // assets\power-off-button-64-red.png
LV_IMG_DECLARE(ui_img_1452075502);    // assets\icons8-radar-60.png
LV_IMG_DECLARE(ui_img_1851565394);    // assets\icons8-game-controller-50.png
LV_IMG_DECLARE(ui_img_240629768);    // assets\icons8-game-controller-50-RED.png




void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
