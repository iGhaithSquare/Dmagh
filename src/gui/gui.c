#include "gui.h"
#include <gaven.h>
void add_text_to_panel(gui_ctx* ctx){

}

void start_gui_frame(gui_ctx* ctx){
    if(!ctx->Panel.cap){
        ctx->Panel.cap=128;
        ctx->Panel.elements=malloc(sizeof(ctx->Panel.cap));
    }
}
void begin_panel(gui_ctx* ctx,const char* title,int x,int y,int width, int height,enum gui_flags flags){
    ctx->Panel=(struct gui_panel){0,ctx->Panel.cap,NULL,title,x,y,width,height,flags};
}
void end_panel(gui_ctx* ctx){
    struct gui_panel P=ctx->Panel;
    ctx->Draw_Quad(ctx->User_Data,P.x,P.y,P.width,P.height,ctx->theme_r,ctx->theme_g,ctx->theme_b,1.0f);
}

void set_panel_layout(gui_ctx* ctx,enum gui_panel_layout panel_layout,int pad_x){
    ctx->Panel.layout=panel_layout;
    /*
    switch (panel_layout){
    case GUI_PAD_START:
        ctx->Panel.pad_x=pad_x;
        break;
    case GUI_PAD_MIDDLE:
        ctx->Panel.pad_x=ctx->Panel.width-
        break;
    case GUI_PAD_END:
    default:
        GAVEN_WARN("Unsupported Gui panel layout :%d",panel_layout);
        break;
    }
    */
}
void end_gui_frame(gui_ctx* ctx){/*
    int i=0;
    while(i<ctx->Count){
        gui_quad quad=ctx->Quad_Array[i];
        ctx->Draw_Quad(ctx->User_Data,quad.x,quad.y,quad.width,quad.height,quad.r,quad.g,quad.b,quad.a);
        i++;
    }
        */
    return;
}
void destroy_gui(gui_ctx* ctx){
    return;
}