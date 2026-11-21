#include "gui.h"
#include "elements/gui_panel.h"
#include <gaven.h>
gui_ctx *create_gui_ctx(int width, int height,vec3 Primary_Color,vec3 Secondary_Color,void* User_Data,
            void (*Draw_Quad)(void* user_data, int x,int y,float z, int width, int height,float red,float green,float blue,float alpha),
            void (*Draw_Textured_Quad)(void* user_data,void* texture,int x,int y,float z, int width, int height,float red,float green,float blue,float alpha),
            void *(*Get_FrameBuffer_Texture)(void* user_data)){
    gui_ctx* Ctx=(gui_ctx*)malloc(sizeof(gui_ctx));
    GAVEN_ASSERT(Ctx,"Couldnt Allocate Memory to Gui");
    Ctx->Draw_List_Cap=16;
    Ctx->Draw_List_Count=0;
    Ctx->Draw_List_Elements=malloc(sizeof(void*)*Ctx->Draw_List_Cap);
    GAVEN_ASSERT(Ctx->Draw_List_Elements,"Couldnt Allocate Memory to Draw List Elements");

    Ctx->Draw_Quad=Draw_Quad;
    Ctx->Draw_Textured_Quad=Draw_Textured_Quad;
    Ctx->Get_FrameBuffer_Texture=Get_FrameBuffer_Texture;
    Ctx->Framebuffer_Texture=Get_FrameBuffer_Texture(User_Data);
    glm_vec3_copy(Primary_Color,Ctx->Primary_Color);
    glm_vec3_copy(Secondary_Color,Ctx->Secondary_Color);
    Ctx->User_Data=User_Data;
    Ctx->width=width;
    Ctx->height=height;
    return Ctx;
}
int gui_window_resize(window_resize* E,gui_ctx* Ctx){
    if(E->height&&E->width){
        Ctx->Framebuffer_Texture=Ctx->Get_FrameBuffer_Texture(Ctx->User_Data);
        Ctx->width=E->width;
        Ctx->height=E->height;
    }
    return 0;
}
void gui_on_event(gui_ctx* Ctx, event* Event){
    EVENT_DISPATCH_V(Event,window_resize,gui_window_resize,Ctx);

} 
void test_and_grow_draw_list(gui_ctx* ctx){
    if(ctx->Draw_List_Count==ctx->Draw_List_Cap){
        ctx->Draw_List_Cap*=2;
        ctx->Draw_List_Elements=realloc(ctx->Draw_List_Elements,sizeof(void*)*ctx->Draw_List_Cap);
        GAVEN_ASSERT(ctx->Draw_List_Elements,"Couldnt Allocate Memory to Draw List Elements");
    }
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
        struct gui_element* Elt=ctx->Draw_List_Elements[i];
        if(Elt->Draw)
            Elt->Draw(Elt);
    }
}
void destroy_gui(gui_ctx* ctx){
    int i;
    for(i=0;i<ctx->Draw_List_Count;i++){
        struct gui_element* Elt=ctx->Draw_List_Elements[i];
        if(Elt->Destroy)
            Elt->Destroy(Elt);
        else free(Elt);
    }
    free(ctx->Draw_List_Elements);
}

void gui_update(gui_ctx* ctx,double dt){
    size_t i;
    for(i=0;i<ctx->Draw_List_Count;i++){
        struct gui_element* Elt=ctx->Draw_List_Elements[i];
        if(i!=Elt->draw_order){
            struct gui_panel *P=(struct gui_panel*)ctx->Draw_List_Elements[Elt->draw_order];
            Elt->x=Elt->lx+P->base.x;
            Elt->y=Elt->ly+P->base.y;
            Elt->width=Elt->lwidth*P->base.width;
            Elt->height=Elt->lheight*P->base.height;
        }
        if(Elt->Update)
            Elt->Update(Elt,dt);
    }
}