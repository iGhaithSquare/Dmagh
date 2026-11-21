#ifndef GUI_PANEL_H
#define GUI_PANEL_H
#include "../gui.h"
void begin_panel(gui_ctx* ctx,const char* title,float x,float y,float width, float height);

struct gui_panel{
    struct gui_element base;
    const char* title;
    enum gui_panel_layout layout;
    float pad_x,pad_y;
};
#endif