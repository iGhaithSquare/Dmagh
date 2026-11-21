#include "gui_panel.h"
#include <gaven.h>
#include <string.h>

void Draw_Gui_Panel(struct gui_element* Element){
    gui_ctx* ctx=Element->gui_ctx;
    struct gui_panel* P=(struct gui_panel*)Element;
    ctx->Draw_Quad(ctx->User_Data,Element->x*ctx->width,Element->y*ctx->height,Element->z,Element->width*ctx->width,Element->height*ctx->height,ctx->Primary_Color[0],ctx->Primary_Color[1],ctx->Primary_Color[2],1.0f);
    if(Element->width>0.02f&&Element->height>0.02f)
        ctx->Draw_Quad(ctx->User_Data,(Element->x)*ctx->width+10,(Element->y)*ctx->height+10,Element->z+0.1f,(Element->width)*ctx->width-20,(Element->height)*ctx->height-20,ctx->Secondary_Color[0],ctx->Secondary_Color[1],ctx->Secondary_Color[2],1.0f);
}
void Destroy_Gui_Panel(struct gui_element* Element){
    struct gui_panel* P=(struct gui_panel*)Element;
    free((void*)P->title);
    free(P);
}
void begin_panel(gui_ctx* ctx,const char* title,float x,float y,float width, float height){
    struct gui_panel* P;
    P=(struct gui_panel*)malloc(sizeof(struct gui_panel));
    GAVEN_ASSERT(P,"Couldnt allocate data for gui panel");
    test_and_grow_draw_list(ctx);

    P->base.Draw=Draw_Gui_Panel;
    P->base.Update=NULL;
    P->base.Destroy=Destroy_Gui_Panel;
    P->base.gui_ctx=ctx;
    P->base.x=x;
    P->base.y=y;
    P->base.z=(ctx->Draw_List_Count+1);
    P->base.width=width;
    P->base.height=height;
    P->base.draw_order=ctx->Draw_List_Count;
    ctx->Draw_List_Elements[ctx->Draw_List_Count]=&(P->base);
    ctx->Draw_List_Count++;
    P->title=strdup(title);
    P->pad_x=0.01f;
    P->pad_y=0.01f;
}