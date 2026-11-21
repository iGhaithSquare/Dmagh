#include "dmagh_layer.h"
#include <stdlib.h>
#include "../platform/window/window.h"
#include "../platform/rendering/renderer.h"
#include "events/dmagh_events.h"
#include "input.h"
#include "../gui/elements/gui_panel.h"
#include "../gui/elements/gui_viewport.h"
#include "../gui/elements/gui_panel_divider.h"
#include "key_codes.h"
typedef struct dmagh_layer_data{
    void* Renderer;
    void* Window;
    application* App;
    curve Sample_Curve;
    curve_array* Curve_Array;
    int i;
    uint8_t Paused;
    gui_ctx *Ctx;
    int Width,Height;
} dmagh_layer_data;

int dmagh_on_window_destroyed(window_destroyed* E,application* app){
    app->Running=0;
    return 1;
}
int dmagh_on_window_pause(window_pause* E,dmagh_layer_data* Data){
    Data->Paused=E->state;
    return 1;
}
int dmagh_window_resize(window_resize* E,dmagh_layer_data* Data){
    if(E->height&&E->width){
        Data->Width=E->width;
        Data->Height=E->height;
    }
    return 0;
}
void dmagh_on_event(layer* self, event* Event){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    EVENT_DISPATCH_V(Event,window_destroyed,dmagh_on_window_destroyed,Data->App);
    EVENT_DISPATCH_V(Event,window_pause,dmagh_on_window_pause,Data);
    EVENT_DISPATCH_V(Event,window_resize,dmagh_window_resize,Data);
    input_on_event(Event);
    renderer_onEvent(Data->Renderer,Event);
    gui_on_event(Data->Ctx,Event);

} 
void dmagh_on_attach(layer* self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    float third=1.0f/3.0f;
    Data->Width=1280;
    Data->Height=720;
    init_input_system(Data->Width,Data->Height);
    Data->Curve_Array=create_curve_array();
    Data->Window=create_window(&Data->Width,&Data->Height);
    Data->Renderer=create_renderer(Data->Width,Data->Height,128);
    Data->i=0;
    begin_curve(Data->Curve_Array,(curve_point){{0.0f,0.0f},{100.0f,850.0f},50,{1.0f,0.0f,0.5f,1.0f},{300.0f,100.0f}});
    end_curve(Data->Curve_Array,&Data->Sample_Curve,(curve_point){{700.0f,100.0f},{800.0f,850.0f},1,{0.0f,0.5f,1.0f,1.0f},{0.0f,0.0f}});
    Data->Ctx=create_gui_ctx(Data->Width,Data->Height,(vec3){0.09f,0.09f,0.13f},(vec3){0.1f,0.1f,0.15f},Data->Renderer,draw_quad,draw_textured_quad,get_framebuffer_texture);
    
    begin_panel(Data->Ctx,"Bottom",0.0f,0.0f,1.0f,1.0f);
    add_panel_divider(Data->Ctx,1+2,0.3,0.1,0.4,"Left");
    add_panel_divider(Data->Ctx,2,0.3,0.1,0.4,"Viewport");
    add_gui_viewport(Data->Ctx);
    add_panel_divider(Data->Ctx,2,0.7,0.6,0.9,"Right");

    //dont forget splits verticle horizontal, easy panels.
    //begin_panel(Data->Ctx,"Left",0.0f,third,0.25f,2*third);
    //begin_panel(Data->Ctx,"Right",0.75f,0.0f,0.25f,1.0f);
    //begin_panel(Data->Ctx,"ViewPort",0.25f,third,0.5f,2*third);
    //add_gui_viewport(Data->Ctx);
    //begin_panel(Data->Ctx,"Bottom",0.0f,0.0f,0.75f,third);
}
void dmagh_on_dettach(layer* self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    destroy_gui(Data->Ctx);
    destroy_window(Data->Window);
    destroy_renderer(Data->Renderer);
    destroy_input_system();
}
void polling_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    input_polling();
    do {
        poll_window(&Data->Window);
    }while(Data->Paused);
}
void update_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    double dt=0.0f;
    gui_update(Data->Ctx,dt);
}
void render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    begin_frame(Data->Renderer);
    int x=get_mouse_x();
    int y=get_mouse_y();
    draw_curve_array(Data->Renderer,Data->Curve_Array);
}
void gui_render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    end_gui_frame(Data->Ctx);
    end_frame(Data->Renderer);
    render_window(Data->Window);
}

layer* create_dmagh_layer(application* app){
    layer* L=calloc(1,sizeof(layer));
    dmagh_layer_data *data = (dmagh_layer_data*)malloc(sizeof(dmagh_layer_data));
    bind_layer_phase(L,layer_phase_polling,polling_callback);
    bind_layer_phase(L,layer_phase_update_callback,update_callback);
    bind_layer_phase(L,layer_phase_render_callback,render_callback);
    bind_layer_phase(L,layer_phase_gui_render_callback,gui_render_callback);
    data->App=app;
    data->Paused=0;
    L->LayerData=data;
    L->OnAttach=dmagh_on_attach;
    L->OnDettach=dmagh_on_dettach;
    L->OnEvent=dmagh_on_event;
    L->Name="Dmagh";
    return L;
}