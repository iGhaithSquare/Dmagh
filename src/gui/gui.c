#include "gui.h"
#include <gaven.h>
enum GUI_DRAW_TYPES{
    GUI_DRAW_PANEL = 1,
    GUI_DRAW_VIEWPORT= 2
};

void add_text_to_panel(gui_ctx* ctx){

}
void start_gui_frame(gui_ctx* ctx){
    if(ctx->Draw_List_Cap==0){
        ctx->Draw_List_Cap=1024;
        ctx->Draw_List_Elements=malloc(sizeof(void*)*ctx->Draw_List_Cap);
        GAVEN_ASSERT(ctx->Draw_List_Elements,"Couldnt Allocate Memory to Draw List Elements");
        ctx->Draw_List_Order=malloc(sizeof(size_t)*ctx->Draw_List_Cap);
        GAVEN_ASSERT(ctx->Draw_List_Order,"Couldnt Allocate Memory to Draw List Order");
        ctx->Draw_List_Types=malloc(sizeof(int)*ctx->Draw_List_Cap);
        GAVEN_ASSERT(ctx->Draw_List_Types,"Couldnt Allocate Memory to Draw List Types");
    }
    ctx->Panel_Count=0;
    ctx->Draw_List_Count=0;
}
void test_and_grow_draw_list(gui_ctx* ctx){
    if(ctx->Draw_List_Count==ctx->Draw_List_Cap){
        ctx->Draw_List_Cap*=2;
        ctx->Draw_List_Elements=realloc(ctx->Draw_List_Elements,sizeof(void*)*ctx->Draw_List_Cap);
        GAVEN_ASSERT(ctx->Draw_List_Elements,"Couldnt Allocate Memory to Draw List Elements");
        ctx->Draw_List_Order=realloc(ctx->Draw_List_Order,sizeof(size_t)*ctx->Draw_List_Cap);
        GAVEN_ASSERT(ctx->Draw_List_Order,"Couldnt Allocate Memory to Draw List Order");
        ctx->Draw_List_Types=realloc(ctx->Draw_List_Types,sizeof(int)*ctx->Draw_List_Cap);
        GAVEN_ASSERT(ctx->Draw_List_Types,"Couldnt Allocate Memory to Draw List Types");
    }
}
void set_viewport(gui_ctx* ctx){
    test_and_grow_draw_list(ctx);
    ctx->Draw_List_Elements[ctx->Draw_List_Count]=NULL;
    GAVEN_ASSERT(ctx->Draw_List_Count>1,"To set an Element you need to first begin a Panel");
    ctx->Draw_List_Order[ctx->Draw_List_Count]=ctx->Draw_List_Order[ctx->Draw_List_Count-1];
    ctx->Draw_List_Types[ctx->Draw_List_Count++]=GUI_DRAW_VIEWPORT;
}
void begin_panel(gui_ctx* ctx,const char* title,int x,int y,int width, int height,enum gui_flags flags){
    test_and_grow_draw_list(ctx);
    ctx->Panels[ctx->Panel_Count]=(struct gui_panel){title,x,y,width,height,GUI_PAD_START,0,0,flags};
    ctx->Draw_List_Elements[ctx->Draw_List_Count]=&ctx->Panels[ctx->Panel_Count++];
    ctx->Draw_List_Order[ctx->Draw_List_Count]=ctx->Draw_List_Count;
    ctx->Draw_List_Types[ctx->Draw_List_Count++]=GUI_DRAW_PANEL;
} 
void set_panel_layout(gui_ctx* ctx,enum gui_panel_layout panel_layout,int pad_x){
    //ctx->Panel.layout=panel_layout;
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
void end_gui_frame(gui_ctx* ctx){
    size_t i;
    for(i=0;i<ctx->Draw_List_Count;i++){
        size_t current=ctx->Draw_List_Order[i];
        switch (ctx->Draw_List_Types[i]){
        case GUI_DRAW_PANEL:{
            struct gui_panel *P=ctx->Draw_List_Elements[current];
            ctx->Draw_Quad(ctx->User_Data,P->x,P->y,i+0.5f,P->width,P->height,ctx->Primary_Color[0],ctx->Primary_Color[1],ctx->Primary_Color[2],1.0f);
            if(P->width>20&&P->height>20){
                ctx->Draw_Quad(ctx->User_Data,P->x+10,P->y+10,i+1.0f,P->width-20,P->height-20,ctx->Secondary_Color[0],ctx->Secondary_Color[1],ctx->Secondary_Color[2],1.0f);
            }
            P->pad_x=10;
            P->pad_y=10;
            break;
        }
        case GUI_DRAW_VIEWPORT:{
            struct gui_panel *P= ctx->Draw_List_Elements[current];
            if(P->pad_y<P->height-10&&P->pad_x<P->width-10){
                ctx->Draw_Textured_Quad(ctx->User_Data,ctx->Framebuffer_Texture,P->x+P->pad_x,P->y+P->pad_y,i+1.0f,P->width-P->pad_x-10,P->height-P->pad_y-10,1.0f,1.0f,1.0f,1.0f);
                P->pad_y+=P->height-10;
            }
            break;
        }

        default:
            GAVEN_WARN("Unsupported gui panel type %d",ctx->Draw_List_Types[current]);
            break;
        }
    }
}
void destroy_gui(gui_ctx* ctx){
    free(ctx->Draw_List_Elements);
    free(ctx->Draw_List_Order);
    free(ctx->Draw_List_Types);
}