#ifndef GUI_H
#define GUI_H
#include <stdlib.h>
#include <cglm/cglm.h>
#include "../core/events/dmagh_events.h"
enum gui_panel_layout{
    GUI_PAD_START = 1,
    GUI_PAD_MIDDLE = 2,
    GUI_PAD_END =3
};
struct gui_element{
    int draw_order;
    float lx,ly,lwidth,lheight;
    float x,y,width,height;
    float z;
    struct gui_ctx *gui_ctx;
    void (*Draw)(struct gui_element* Element);
    void (*Update)(struct gui_element* Element,double delta_time);
    void (*Destroy)(struct gui_element* Element);
};
void test_and_grow_draw_list(struct gui_ctx* ctx);
struct gui_ctx{
    int width,height;
    struct gui_element** Draw_List_Elements;
    size_t Draw_List_Count;
    size_t Draw_List_Cap;
    vec3 Primary_Color;
    vec3 Secondary_Color;
    void (*Draw_Quad)(void* user_data, int x,int y,float z, int width, int height,float red,float green,float blue,float alpha);
    void (*Draw_Textured_Quad)(void* user_data,void* texture,int x,int y,float z, int width, int height,float red,float green,float blue,float alpha);
    void *(*Get_FrameBuffer_Texture)(void* user_data);
    void *User_Data;
    void *Framebuffer_Texture;
};
typedef struct gui_ctx gui_ctx;
gui_ctx *create_gui_ctx(int width,int height,vec3 Primary_Color,vec3 Secondary_Color,void* User_Data,
            void (*Draw_Quad)(void* user_data, int x,int y,float z, int width, int height,float red,float green,float blue,float alpha),
            void (*Draw_Textured_Quad)(void* user_data,void* texture,int x,int y,float z, int width, int height,float red,float green,float blue,float alpha),
            void *(*Get_FrameBuffer_Texture)(void* user_data));
void set_panel_layout(gui_ctx* ctx,enum gui_panel_layout panel_layout,int pad_x);
void end_gui_frame(gui_ctx* ctx);
void destroy_gui(gui_ctx* ctx);
void gui_on_event(gui_ctx* ctx, event* Event);
void gui_update(gui_ctx* ctx,double dt);
#endif