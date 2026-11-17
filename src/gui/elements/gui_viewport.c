#include "gui_viewport.h"
#include <gaven.h>
#include "gui_panel.h"
void Draw_Gui_Viewport(struct gui_element* Element){
    gui_ctx* ctx=Element->gui_ctx;
    ctx->Draw_Textured_Quad(ctx->User_Data,ctx->Framebuffer_Texture,Element->x*ctx->width,Element->y*ctx->height,Element->z,Element->width*ctx->width,Element->height*ctx->height,1.0f,1.0f,1.0f,1.0f);
}
void add_gui_viewport(gui_ctx* Ctx){
    struct gui_element* View;
    int P_Order;
    struct gui_panel *P;
    GAVEN_ASSERT(Ctx->Draw_List_Count>1,"To set an Element you need to first begin a Panel");
    P_Order=Ctx->Draw_List_Elements[Ctx->Draw_List_Count-1]->draw_order;
    P= (struct gui_panel*)Ctx->Draw_List_Elements[P_Order];
    View=(struct gui_element*)malloc(sizeof(struct gui_element));
    GAVEN_ASSERT(View,"Couldnt allocate data for gui viewport");
    test_and_grow_draw_list(Ctx);

    View->Draw=Draw_Gui_Viewport;
    View->Update=NULL;
    View->Destroy=NULL;
    View->gui_ctx=Ctx;
    View->x=P->base.x+P->pad_x;
    View->y=P->base.y+P->pad_y;
    View->z=(Ctx->Draw_List_Count+1);
    View->width=P->base.width-P->pad_x*2;
    View->height=P->base.height-P->pad_y*2;
    View->draw_order=P_Order;

    Ctx->Draw_List_Elements[Ctx->Draw_List_Count]=View;
    Ctx->Draw_List_Count++;
    P->pad_y+=P->base.height;

}