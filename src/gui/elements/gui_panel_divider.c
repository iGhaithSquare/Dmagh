#include "gui_panel_divider.h"
#include <gaven.h>
#include "gui_panel.h"
#include "../../core/input.h"
#include "../../core/key_codes.h"
void panel_divider_update(struct gui_element* Element,double delta_time){
    float mx=get_mouse_x(),my=get_mouse_y(),delta;
    struct gui_panel* P1,*P2;
    gui_ctx* Ctx=Element->gui_ctx;
    mx/=Ctx->width;
    my/=Ctx->height;
    struct gui_panel_divider *Div=(struct gui_panel_divider*)Element;
    if(Div->FLAGS&2){
        if(Div->is_clicked){
            P1=(struct gui_panel*)Ctx->Draw_List_Elements[Div->P1_Order];
            P2=(struct gui_panel*)Ctx->Draw_List_Elements[Div->P2_Order];
            if(Div->FLAGS&1){
                delta=my-Div->p;
                Div->p=my;
                P2->base.y+=delta;
                P2->base.height-=delta;
                P1->base.height+=delta;
                Div->value+=delta;
                Div->base.ly=P1->base.height-10.0f/(Ctx->height);
            }
            else{
                delta=mx-Div->p;
                Div->p=mx;
                P2->base.x+=delta;
                P2->base.width-=delta;
                P1->base.width+=delta;
                Div->value+=delta;
                Div->base.lx=P1->base.width-10.0f/(Ctx->width);
            }
        }
        if(is_key_pressed(DMAGH_MOUSE_BUTTON_LEFT)){
            if (mx>=Element->x&&mx<=Element->width+Element->x&&my>=Element->y&&my<=Element->height+Element->y){
                Div->is_clicked=1;
                Div->p=(Div->FLAGS&1)?my:mx;
            }
        }
        if(Div->is_clicked&&is_key_released(DMAGH_MOUSE_BUTTON_LEFT))
            Div->is_clicked=0;
    }
}


void add_panel_divider(gui_ctx* Ctx,enum panel_divider_flags FLAGS,float value,float min_value,float max_value,const char *Name){
    struct gui_panel_divider* Div;
    struct gui_panel *P;
    GAVEN_ASSERT(Ctx->Draw_List_Count>0,"To set an Element you need to first begin a Panel");
    Div=(struct gui_panel_divider*)malloc(sizeof(struct gui_panel_divider));
    GAVEN_ASSERT(Div,"Couldnt allocate data for gui panel divider");
    Div->P1_Order=Ctx->Draw_List_Elements[Ctx->Draw_List_Count-1]->draw_order;
    P= (struct gui_panel*)Ctx->Draw_List_Elements[Div->P1_Order];
    test_and_grow_draw_list(Ctx);

    Div->base.Draw=NULL;
    Div->base.Update=panel_divider_update;
    Div->base.Destroy=NULL;
    Div->base.gui_ctx=Ctx;
    Div->base.z=(Ctx->Draw_List_Count+1);
    Div->base.draw_order=Div->P1_Order;


    Ctx->Draw_List_Elements[Ctx->Draw_List_Count]=&(Div->base);
    Ctx->Draw_List_Count++;

    Div->FLAGS=FLAGS;
    Div->value=value;
    Div->max_value=max_value;
    Div->min_value=min_value;
    Div->is_clicked=0;
    Div->p=0.0f;

    if(FLAGS&1){
        begin_panel(Ctx,Name,P->base.x,P->base.height*Div->value+P->base.y,P->base.width,P->base.height-P->base.height*Div->value);
        P->base.height=P->base.height*Div->value;
        Div->base.width=Div->base.lwidth=1.0f;
        Div->base.height=Div->base.lheight=20.0f/(P->base.height*Ctx->height);
        Div->base.x=Div->base.lx=0.0f;
        Div->base.y=Div->base.ly=P->base.height-10.0f/(Ctx->height);
    }
    else{
        begin_panel(Ctx,Name,P->base.width*Div->value+P->base.x,P->base.y,P->base.width-P->base.width*Div->value,P->base.height);
        P->base.width=P->base.width*Div->value;
        Div->base.width=Div->base.lwidth=20.0f/(P->base.width*Ctx->width);
        Div->base.height=Div->base.lheight=1.0f;
        Div->base.x=Div->base.lx=P->base.width-10.0f/(Ctx->width);
        Div->base.y=Div->base.ly=0.0f;
    }
    Div->P2_Order=Ctx->Draw_List_Elements[Ctx->Draw_List_Count-1]->draw_order;
}