#ifndef GUI_H
#define GUI_H
#include <stdlib.h>
#include <cglm/cglm.h>
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
 struct gui_panel{
    const char* title;
    int x,y,width,height;
    enum gui_panel_layout layout;
    int pad_x,pad_y;
    enum gui_flags flags;
};
struct gui_ctx{
    void** Draw_List_Elements;
    size_t* Draw_List_Order;
    int* Draw_List_Types;
    size_t Draw_List_Count;
    size_t Draw_List_Cap;
    struct gui_panel Panels[256];
    int Panel_Count;
    vec3 Primary_Color;
    vec3 Secondary_Color;
    void (*Draw_Quad)(void* user_data, int x,int y,float z, int width, int height,float red,float green,float blue,float alpha);
    void (*Draw_Textured_Quad)(void* user_data,void* texture,int x,int y,float z, int width, int height,float red,float green,float blue,float alpha);
    void *User_Data;
    void *Framebuffer_Texture;
};
typedef struct gui_ctx gui_ctx;
void start_gui_frame(gui_ctx* ctx);
void begin_panel(gui_ctx* ctx,const char* title,int x,int y,int width, int height,enum gui_flags flags);
void set_viewport(gui_ctx* ctx);
void set_panel_layout(gui_ctx* ctx,enum gui_panel_layout panel_layout,int pad_x);
void end_gui_frame(gui_ctx* ctx);
void destroy_gui(gui_ctx* ctx);
#endif