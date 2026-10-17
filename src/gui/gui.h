#ifndef GUI_H
#define GUI_H
#include <stdlib.h>
enum gui_flags{
    GUI_BORDERLESS = 1,
    GUI_MOVABLE = 2,
    GUI_SCALABLE =4,
    GUI_NO_INPUT =8
};
enum gui_panel_layout{
    GUI_PAD_START = 1,
    GUI_PAD_MIDDLE = 2,
    GUI_PAD_END =3
};

struct gui_panel_element{
    int x,y;
    void (*Draw_Panel)(struct gui_panel_element* base,void* user_data);
};
struct gui_panel{
    size_t count;
    size_t cap;
    struct gui_panel_element* elements;
    const char* title;
    int x,y,width,height;
    enum gui_panel_layout layout;
    int pad_x,pad_y;
    enum gui_flags flags;
};
struct gui_ctx{
    struct gui_panel Panel;
    float theme_r,theme_g,theme_b;
    void (*Draw_Quad)(void* user_data, int x,int y, int width, int height,float red,float green,float blue,float alpha);
    void *User_Data;
};
typedef struct gui_ctx gui_ctx;
void start_gui_frame(gui_ctx* ctx);
void begin_panel(gui_ctx* ctx,const char* title,int x,int y,int width, int height,enum gui_flags flags);
void set_panel_layout(gui_ctx* ctx,enum gui_panel_layout panel_layout,int pad_x);
void end_panel(gui_ctx* ctx);
void destroy_gui(gui_ctx* ctx);
#endif