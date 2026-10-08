#include "dmagh_layer.h"
#include <stdlib.h>
#include "../platform/window/window.h"
#include "../platform/rendering/renderer.h"
#include "events/dmagh_events.h"
#include "input.h"
typedef struct dmagh_layer_data{
    void* Renderer;
    void* Window;
    application* App;
    curve Sample_Curve;
    curve_array* Curve_Array;
    int i;
    uint8_t Paused;
} dmagh_layer_data;

int dmagh_on_window_destroyed(window_destroyed* E,application* app){
    app->Running=0;
    return 1;
}
int dmagh_on_window_pause(window_pause* E,dmagh_layer_data* Data){
    Data->Paused=E->state;
    return 1;
}
void dmagh_on_event(layer* self, event* Event){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    EVENT_DISPATCH_V(Event,window_destroyed,dmagh_on_window_destroyed,Data->App);
    EVENT_DISPATCH_V(Event,window_pause,dmagh_on_window_pause,Data);
    input_on_event(Event);
    renderer_onEvent(Data->Renderer,Event);
} 
void dmagh_on_attach(layer* self){
    int width=1280,height=720;
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    init_input_system(width,height);
    Data->Curve_Array=create_curve_array();
    Data->Window=create_window(&width,&height);
    Data->Renderer=create_renderer(width,height,128);
    Data->i=0;
    begin_curve(Data->Curve_Array,(curve_point){{0.0f,0.0f},{100.0f,850.0f},50,{1.0f,0.0f,0.5f,1.0f},{300.0f,100.0f}});
    end_curve(Data->Curve_Array,&Data->Sample_Curve,(curve_point){{700.0f,100.0f},{800.0f,850.0f},1,{0.0f,0.5f,1.0f,1.0f},{0.0f,0.0f}});
}
void dmagh_on_dettach(layer* self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    destroy_window(Data->Window);
    destroy_renderer(Data->Renderer);
    destroy_input_system();
}
void polling_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    do {
        poll_window(&Data->Window);
    }while(Data->Paused);
    input_polling();
}
void update_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    rotate_camera(Data->Renderer,0.0f,0.0f,(float)(Data->i)*0.01f);
    
}
void render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    begin_frame(Data->Renderer);
    int x=get_mouse_x();
    int y=get_mouse_y();
    draw_quad(Data->Renderer,x,y,128,256,1.0f,0.0f,0.0f,1.0f);
    draw_quad(Data->Renderer,202,402,256,128,0.0f,1.0f,1.0f,1.0f);
    draw_curve_array(Data->Renderer,Data->Curve_Array);
}
void gui_render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
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