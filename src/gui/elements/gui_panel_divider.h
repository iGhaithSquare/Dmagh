#ifndef GUI_PANEL_DIVIDER_H
#define GUI_PANEL_DIVIDER_H
#include "../gui.h"

enum panel_divider_flags{
    GUI_DIRECTION = 1,// 0 for horizontal 1 for vertical
    GUI_MOVABLE = 2
};

void add_panel_divider(gui_ctx* Ctx,enum panel_divider_flags FLAGS,float value,float min_value,float max_value,const char *Name);

struct gui_panel_divider{
    struct gui_element base;
    float value,min_value,max_value;
    float p;
    enum panel_divider_flags FLAGS;
    uint8_t is_clicked;
    int P1_Order,P2_Order;
};
#endif