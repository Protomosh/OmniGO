// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.3.3
// PROJECT: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"
#include "ui_comp.h"

///////////////////// VARIABLES ////////////////////
void BootLoading_Animation(lv_obj_t * TargetObject, int delay);
void ui_event_BootScreen(lv_event_t * e);
lv_obj_t * ui_BootScreen;
lv_obj_t * ui_BootLogo;
lv_obj_t * ui_BootSpinner;
lv_obj_t * ui_LoadingLabel;
lv_obj_t * ui_BootHeader;
lv_obj_t * ui_BootScreen_Panel1;
lv_obj_t * ui_BootScreenLoadingTextBox;
void ui_event_MainScreen(lv_event_t * e);
lv_obj_t * ui_MainScreen;
lv_obj_t * ui_BatImg1;
lv_obj_t * ui_Batbar1;
lv_obj_t * ui_SteerBar;
lv_obj_t * ui_SteerLabel;
lv_obj_t * ui_throttleBar;
lv_obj_t * ui_ThrottleLabel;
lv_obj_t * ui_Batbar2;
lv_obj_t * ui_BatImg2;
lv_obj_t * ui_MainScreen_Panel2;
lv_obj_t * ui_MainScreen_Label2;
lv_obj_t * ui_MainScreen_ImgButton2;
lv_obj_t * ui_radarPlaceholder;
void ui_event_controllerBtn(lv_event_t * e);
lv_obj_t * ui_controllerBtn;
void ui_event_pwrBtn(lv_event_t * e);
lv_obj_t * ui_pwrBtn;
void ui_event_DebugScreen(lv_event_t * e);
lv_obj_t * ui_DebugScreen;
lv_obj_t * ui_Screen1_Colorwheel1;
lv_obj_t * ui_Screen1_Button1;
lv_obj_t * ui_Screen1_Button2;
lv_obj_t * ui_Screen1_Button3;
lv_obj_t * ui_Screen1_Roller1;
lv_obj_t * ui_Screen1_TextArea1;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
void BootLoading_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 2000);
    lv_anim_set_user_data(&PropertyAnimation_0, TargetObject);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
    lv_anim_start(&PropertyAnimation_0);

}

///////////////////// FUNCTIONS ////////////////////
void ui_event_BootScreen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        _ui_screen_change(ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 0, 3000);
    }
}
void ui_event_MainScreen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        _ui_screen_change(ui_DebugScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0);
    }
}
void ui_event_controllerBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED &&  lv_obj_has_state(target, LV_STATE_CHECKED)) {
        ExternalControl(e);
    }
    if(event_code == LV_EVENT_VALUE_CHANGED &&  !lv_obj_has_state(target, LV_STATE_CHECKED)) {
        LocalControl(e);
    }
}
void ui_event_pwrBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED &&  lv_obj_has_state(target, LV_STATE_CHECKED)) {
        motorPowerOn(e);
    }
    if(event_code == LV_EVENT_VALUE_CHANGED &&  !lv_obj_has_state(target, LV_STATE_CHECKED)) {
        powerOff(e);
    }
}
void ui_event_DebugScreen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        _ui_screen_change(ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0);
    }
}

///////////////////// SCREENS ////////////////////
void ui_BootScreen_screen_init(void)
{
    ui_BootScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_BootScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_BootLogo = lv_img_create(ui_BootScreen);
    lv_img_set_src(ui_BootLogo, &ui_img_logo_smallest_png);
    lv_obj_set_width(ui_BootLogo, LV_SIZE_CONTENT);   /// 100
    lv_obj_set_height(ui_BootLogo, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_align(ui_BootLogo, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BootLogo, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_BootLogo, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_BootLogo, 220);

    ui_BootSpinner = lv_spinner_create(ui_BootScreen, 1000, 90);
    lv_obj_set_width(ui_BootSpinner, 260);
    lv_obj_set_height(ui_BootSpinner, 260);
    lv_obj_set_align(ui_BootSpinner, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_BootSpinner, LV_OBJ_FLAG_CLICKABLE);      /// Flags

    lv_obj_set_style_arc_color(ui_BootSpinner, lv_color_hex(0x4040FF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_BootSpinner, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_BootSpinner, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_LoadingLabel = lv_label_create(ui_BootScreen);
    lv_obj_set_width(ui_LoadingLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LoadingLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LoadingLabel, 0);
    lv_obj_set_y(ui_LoadingLabel, 150);
    lv_obj_set_align(ui_LoadingLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LoadingLabel, "Loading...");
    lv_obj_set_style_text_font(ui_LoadingLabel, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BootHeader = lv_label_create(ui_BootScreen);
    lv_obj_set_width(ui_BootHeader, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BootHeader, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_BootHeader, 0);
    lv_obj_set_y(ui_BootHeader, -180);
    lv_obj_set_align(ui_BootHeader, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BootHeader, "Mobile robot\n   platform");
    lv_obj_set_style_text_font(ui_BootHeader, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BootScreen_Panel1 = lv_obj_create(ui_BootScreen);
    lv_obj_set_width(ui_BootScreen_Panel1, 100);
    lv_obj_set_height(ui_BootScreen_Panel1, 50);
    lv_obj_set_x(ui_BootScreen_Panel1, 278);
    lv_obj_set_y(ui_BootScreen_Panel1, 68);
    lv_obj_set_align(ui_BootScreen_Panel1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_BootScreen_Panel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_BootScreenLoadingTextBox = lv_textarea_create(ui_BootScreen);
    lv_obj_set_width(ui_BootScreenLoadingTextBox, 300);
    lv_obj_set_height(ui_BootScreenLoadingTextBox, LV_SIZE_CONTENT);    /// 60
    lv_obj_set_x(ui_BootScreenLoadingTextBox, 0);
    lv_obj_set_y(ui_BootScreenLoadingTextBox, 200);
    lv_obj_set_align(ui_BootScreenLoadingTextBox, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_BootScreenLoadingTextBox, "Placeholder...");
    lv_textarea_set_one_line(ui_BootScreenLoadingTextBox, true);

    lv_obj_add_event_cb(ui_BootScreen, ui_event_BootScreen, LV_EVENT_ALL, NULL);

}
void ui_MainScreen_screen_init(void)
{
    ui_MainScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_MainScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_BatImg1 = lv_img_create(ui_MainScreen);
    lv_img_set_src(ui_BatImg1, &ui_img_687825122);
    lv_obj_set_width(ui_BatImg1, LV_SIZE_CONTENT);   /// 50
    lv_obj_set_height(ui_BatImg1, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_x(ui_BatImg1, -130);
    lv_obj_set_y(ui_BatImg1, 190);
    lv_obj_set_align(ui_BatImg1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BatImg1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_BatImg1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Batbar1 = lv_bar_create(ui_MainScreen);
    lv_bar_set_range(ui_Batbar1, 0, 100);
    lv_bar_set_value(ui_Batbar1, 10, LV_ANIM_OFF);
    lv_obj_set_width(ui_Batbar1, 28);
    lv_obj_set_height(ui_Batbar1, 16);
    lv_obj_set_x(ui_Batbar1, -130);
    lv_obj_set_y(ui_Batbar1, 190);
    lv_obj_set_align(ui_Batbar1, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_Batbar1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Batbar1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Batbar1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_Batbar1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Batbar1, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Batbar1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_SteerBar = lv_bar_create(ui_MainScreen);
    lv_bar_set_range(ui_SteerBar, -1000, 1000);
    lv_obj_set_width(ui_SteerBar, 250);
    lv_obj_set_height(ui_SteerBar, 20);
    lv_obj_set_x(ui_SteerBar, 1);
    lv_obj_set_y(ui_SteerBar, -200);
    lv_obj_set_align(ui_SteerBar, LV_ALIGN_CENTER);
    lv_bar_set_mode(ui_SteerBar, LV_BAR_MODE_SYMMETRICAL);

    lv_obj_set_style_bg_color(ui_SteerBar, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SteerBar, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_SteerLabel = lv_label_create(ui_MainScreen);
    lv_obj_set_width(ui_SteerLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SteerLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_SteerLabel, 0);
    lv_obj_set_y(ui_SteerLabel, -220);
    lv_obj_set_align(ui_SteerLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SteerLabel, "Steer");

    ui_throttleBar = lv_bar_create(ui_MainScreen);
    lv_bar_set_range(ui_throttleBar, 0, 1000);
    lv_obj_set_width(ui_throttleBar, 150);
    lv_obj_set_height(ui_throttleBar, 20);
    lv_obj_set_x(ui_throttleBar, 0);
    lv_obj_set_y(ui_throttleBar, -160);
    lv_obj_set_align(ui_throttleBar, LV_ALIGN_CENTER);

    lv_obj_set_style_bg_color(ui_throttleBar, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_throttleBar, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_ThrottleLabel = lv_label_create(ui_MainScreen);
    lv_obj_set_width(ui_ThrottleLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ThrottleLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ThrottleLabel, 0);
    lv_obj_set_y(ui_ThrottleLabel, -180);
    lv_obj_set_align(ui_ThrottleLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ThrottleLabel, "Throttle");

    ui_Batbar2 = lv_bar_create(ui_MainScreen);
    lv_bar_set_range(ui_Batbar2, 0, 100);
    lv_bar_set_value(ui_Batbar2, 10, LV_ANIM_OFF);
    lv_obj_set_width(ui_Batbar2, 28);
    lv_obj_set_height(ui_Batbar2, 16);
    lv_obj_set_x(ui_Batbar2, 130);
    lv_obj_set_y(ui_Batbar2, 190);
    lv_obj_set_align(ui_Batbar2, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_Batbar2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Batbar2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Batbar2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_Batbar2, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Batbar2, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Batbar2, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_BatImg2 = lv_img_create(ui_MainScreen);
    lv_img_set_src(ui_BatImg2, &ui_img_687825122);
    lv_obj_set_width(ui_BatImg2, LV_SIZE_CONTENT);   /// 50
    lv_obj_set_height(ui_BatImg2, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_x(ui_BatImg2, 130);
    lv_obj_set_y(ui_BatImg2, 190);
    lv_obj_set_align(ui_BatImg2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BatImg2, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_BatImg2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_MainScreen_Panel2 = lv_obj_create(ui_MainScreen);
    lv_obj_set_width(ui_MainScreen_Panel2, 300);
    lv_obj_set_height(ui_MainScreen_Panel2, 30);
    lv_obj_set_x(ui_MainScreen_Panel2, 0);
    lv_obj_set_y(ui_MainScreen_Panel2, 220);
    lv_obj_set_align(ui_MainScreen_Panel2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_MainScreen_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_MainScreen_Panel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MainScreen_Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_MainScreen_Label2 = lv_label_create(ui_MainScreen);
    lv_obj_set_width(ui_MainScreen_Label2, 280);
    lv_obj_set_height(ui_MainScreen_Label2, 20);
    lv_obj_set_x(ui_MainScreen_Label2, 0);
    lv_obj_set_y(ui_MainScreen_Label2, 220);
    lv_obj_set_align(ui_MainScreen_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_MainScreen_Label2, "Placeholder");
    lv_obj_set_style_text_color(ui_MainScreen_Label2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_MainScreen_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_MainScreen_ImgButton2 = lv_imgbtn_create(ui_MainScreen);
    lv_imgbtn_set_src(ui_MainScreen_ImgButton2, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_874557855, NULL);
    lv_imgbtn_set_src(ui_MainScreen_ImgButton2, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_1362995411, NULL);
    lv_imgbtn_set_src(ui_MainScreen_ImgButton2, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_1362995411, NULL);
    lv_imgbtn_set_src(ui_MainScreen_ImgButton2, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_1362995411, NULL);
    lv_obj_set_width(ui_MainScreen_ImgButton2, LV_SIZE_CONTENT);   /// 50
    lv_obj_set_height(ui_MainScreen_ImgButton2, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_align(ui_MainScreen_ImgButton2, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_MainScreen_ImgButton2, LV_STATE_CHECKED);       /// States
    lv_obj_add_flag(ui_MainScreen_ImgButton2, LV_OBJ_FLAG_CHECKABLE);     /// Flags
    lv_obj_clear_flag(ui_MainScreen_ImgButton2,
                      LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags

    ui_radarPlaceholder = lv_img_create(ui_MainScreen);
    lv_img_set_src(ui_radarPlaceholder, &ui_img_1452075502);
    lv_obj_set_width(ui_radarPlaceholder, LV_SIZE_CONTENT);   /// 60
    lv_obj_set_height(ui_radarPlaceholder, LV_SIZE_CONTENT);    /// 60
    lv_obj_set_x(ui_radarPlaceholder, 0);
    lv_obj_set_y(ui_radarPlaceholder, -30);
    lv_obj_set_align(ui_radarPlaceholder, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_radarPlaceholder, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_radarPlaceholder, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_radarPlaceholder, 700);

    ui_controllerBtn = lv_imgbtn_create(ui_MainScreen);
    lv_imgbtn_set_src(ui_controllerBtn, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1851565394, NULL);
    lv_imgbtn_set_src(ui_controllerBtn, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_240629768, NULL);
    lv_imgbtn_set_src(ui_controllerBtn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_1851565394, NULL);
    lv_imgbtn_set_src(ui_controllerBtn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_240629768, NULL);
    lv_obj_set_width(ui_controllerBtn, 50);
    lv_obj_set_height(ui_controllerBtn, 50);
    lv_obj_set_x(ui_controllerBtn, 0);
    lv_obj_set_y(ui_controllerBtn, 85);
    lv_obj_set_align(ui_controllerBtn, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_controllerBtn, LV_STATE_CHECKED);       /// States
    lv_obj_add_flag(ui_controllerBtn, LV_OBJ_FLAG_CHECKABLE);     /// Flags
    lv_obj_clear_flag(ui_controllerBtn, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                      LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_scrollbar_mode(ui_controllerBtn, LV_SCROLLBAR_MODE_OFF);

    ui_pwrBtn = lv_imgbtn_create(ui_MainScreen);
    lv_imgbtn_set_src(ui_pwrBtn, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_874557855, NULL);
    lv_imgbtn_set_src(ui_pwrBtn, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_874557855, NULL);
    lv_imgbtn_set_src(ui_pwrBtn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_874557855, NULL);
    lv_imgbtn_set_src(ui_pwrBtn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_1362995411, NULL);
    lv_obj_set_width(ui_pwrBtn, 64);
    lv_obj_set_height(ui_pwrBtn, 64);
    lv_obj_set_x(ui_pwrBtn, 0);
    lv_obj_set_y(ui_pwrBtn, 160);
    lv_obj_set_align(ui_pwrBtn, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_pwrBtn, LV_STATE_CHECKED);       /// States
    lv_obj_add_flag(ui_pwrBtn, LV_OBJ_FLAG_CHECKABLE);     /// Flags
    lv_obj_clear_flag(ui_pwrBtn, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                      LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_scrollbar_mode(ui_pwrBtn, LV_SCROLLBAR_MODE_OFF);

    lv_obj_add_event_cb(ui_controllerBtn, ui_event_controllerBtn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_pwrBtn, ui_event_pwrBtn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_MainScreen, ui_event_MainScreen, LV_EVENT_ALL, NULL);

}
void ui_DebugScreen_screen_init(void)
{
    ui_DebugScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_DebugScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Colorwheel1 = lv_colorwheel_create(ui_DebugScreen, true);
    lv_obj_set_width(ui_Screen1_Colorwheel1, 150);
    lv_obj_set_height(ui_Screen1_Colorwheel1, 150);
    lv_obj_set_x(ui_Screen1_Colorwheel1, 55);
    lv_obj_set_y(ui_Screen1_Colorwheel1, -50);
    lv_obj_set_align(ui_Screen1_Colorwheel1, LV_ALIGN_CENTER);
    lv_obj_set_style_arc_width(ui_Screen1_Colorwheel1, 15, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Screen1_Button1 = lv_btn_create(ui_DebugScreen);
    lv_obj_set_width(ui_Screen1_Button1, 100);
    lv_obj_set_height(ui_Screen1_Button1, 50);
    lv_obj_set_x(ui_Screen1_Button1, -80);
    lv_obj_set_y(ui_Screen1_Button1, -180);
    lv_obj_set_align(ui_Screen1_Button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen1_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Screen1_Button1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Button2 = lv_btn_create(ui_DebugScreen);
    lv_obj_set_width(ui_Screen1_Button2, 100);
    lv_obj_set_height(ui_Screen1_Button2, 50);
    lv_obj_set_x(ui_Screen1_Button2, 80);
    lv_obj_set_y(ui_Screen1_Button2, -180);
    lv_obj_set_align(ui_Screen1_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen1_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Screen1_Button2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Button3 = lv_btn_create(ui_DebugScreen);
    lv_obj_set_width(ui_Screen1_Button3, 100);
    lv_obj_set_height(ui_Screen1_Button3, 50);
    lv_obj_set_x(ui_Screen1_Button3, 80);
    lv_obj_set_y(ui_Screen1_Button3, 85);
    lv_obj_set_align(ui_Screen1_Button3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen1_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Screen1_Button3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Roller1 = lv_roller_create(ui_DebugScreen);
    lv_roller_set_options(ui_Screen1_Roller1, "Mode 1\nMode 2\nMode 3", LV_ROLLER_MODE_INFINITE);
    lv_obj_set_height(ui_Screen1_Roller1, 100);
    lv_obj_set_width(ui_Screen1_Roller1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_Screen1_Roller1, -85);
    lv_obj_set_y(ui_Screen1_Roller1, -80);
    lv_obj_set_align(ui_Screen1_Roller1, LV_ALIGN_CENTER);

    ui_Screen1_TextArea1 = lv_textarea_create(ui_DebugScreen);
    lv_obj_set_width(ui_Screen1_TextArea1, 300);
    lv_obj_set_height(ui_Screen1_TextArea1, 100);
    lv_obj_set_x(ui_Screen1_TextArea1, 0);
    lv_obj_set_y(ui_Screen1_TextArea1, 175);
    lv_obj_set_align(ui_Screen1_TextArea1, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_Screen1_TextArea1, "Debug data...");

    lv_obj_add_event_cb(ui_DebugScreen, ui_event_DebugScreen, LV_EVENT_ALL, NULL);

}

void ui_init(void)
{
    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_BootScreen_screen_init();
    ui_MainScreen_screen_init();
    ui_DebugScreen_screen_init();
    lv_disp_load_scr(ui_BootScreen);
}
