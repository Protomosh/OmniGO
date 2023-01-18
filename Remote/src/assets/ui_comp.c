// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.3.3
// PROJECT: SquareLine_Project


#include "ui.h"
#include "ui_helpers.h"
#include "ui_comp.h"

uint32_t LV_EVENT_GET_COMP_CHILD;

typedef struct {
    uint32_t child_idx;
    lv_obj_t * child;
} ui_comp_get_child_t;

lv_obj_t * ui_comp_get_child(lv_obj_t * comp, uint32_t child_idx)
{
    ui_comp_get_child_t info;
    info.child = NULL;
    info.child_idx = child_idx;
    lv_event_send(comp, LV_EVENT_GET_COMP_CHILD, &info);
    return info.child;
}

void get_component_child_event_cb(lv_event_t * e)
{
    lv_obj_t ** c = lv_event_get_user_data(e);
    ui_comp_get_child_t * info = lv_event_get_param(e);
    info->child = c[info->child_idx];
}

void del_component_child_event_cb(lv_event_t * e)
{
    lv_obj_t ** c = lv_event_get_user_data(e);
    lv_mem_free(c);
}


// COMPONENT Mode

lv_obj_t * ui_Mode_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_Mode;
    cui_Mode = lv_textarea_create(comp_parent);
    lv_obj_set_width(cui_Mode, 93);
    lv_obj_set_height(cui_Mode, LV_SIZE_CONTENT);    /// 70
    lv_obj_set_x(cui_Mode, -174);
    lv_obj_set_y(cui_Mode, -115);
    lv_obj_set_align(cui_Mode, LV_ALIGN_CENTER);
    lv_textarea_set_text(cui_Mode, "Mode:");
    lv_textarea_set_placeholder_text(cui_Mode, "Placeholder...");
    lv_textarea_set_one_line(cui_Mode, true);

    lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_MODE_NUM);
    children[UI_COMP_MODE_MODE] = cui_Mode;
    lv_obj_add_event_cb(cui_Mode, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_Mode, del_component_child_event_cb, LV_EVENT_DELETE, children);
    return cui_Mode;
}



// COMPONENT Versio_number

lv_obj_t * ui_Versio_number_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_Versio_number;
    cui_Versio_number = lv_label_create(comp_parent);
    lv_obj_set_width(cui_Versio_number, LV_SIZE_CONTENT);   /// 2
    lv_obj_set_height(cui_Versio_number, LV_SIZE_CONTENT);    /// 2
    lv_obj_set_x(cui_Versio_number, 207);
    lv_obj_set_y(cui_Versio_number, 146);
    lv_obj_set_align(cui_Versio_number, LV_ALIGN_CENTER);
    lv_label_set_text(cui_Versio_number, "Test 0.1v");

    lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_VERSIO_NUMBER_NUM);
    children[UI_COMP_VERSIO_NUMBER_VERSIO_NUMBER] = cui_Versio_number;
    lv_obj_add_event_cb(cui_Versio_number, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_Versio_number, del_component_child_event_cb, LV_EVENT_DELETE, children);
    return cui_Versio_number;
}

