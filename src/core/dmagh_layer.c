#include "dmagh_layer.h"
#include <stdlib.h>
#include "../platform/window/window.h"
#include "../platform/rendering/renderer.h"
typedef struct dmagh_layer_data{
    void* Renderer;
    void* Window;
    application* App;
    curve Sample_Curve;
    curve_array* Curve_Array;
} dmagh_layer_data;
void dmagh_on_attach(layer* self){
    int width=1280,height=720;
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    Data->Curve_Array=create_curve_array();
    Data->Window=create_window(width,height);
    Data->Renderer=create_renderer(width,height,128);

    
    
    begin_curve(Data->Curve_Array,(curve_point){{40.0f,340.0f},{40.0f,340.0f},{160.0f,400.0f}});
    end_curve(Data->Curve_Array,&Data->Sample_Curve,(curve_point){{200.0f,400.0f},{320.0f,340.0f},{0.0f,0.0f}});
}
void dmagh_on_dettach(layer* self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    destroy_window(Data->Window);
    destroy_renderer(Data->Renderer);
}
void polling_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    poll_window(Data->App,Data->Window);
}
void update_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
}
void render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    begin_frame(Data->Renderer);
    draw_quad(Data->Renderer,0,238,128,256,1.0f,0.0f,0.0f,1.0f);
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
    L->LayerData=data;
    L->OnAttach=dmagh_on_attach;
    L->OnDettach=dmagh_on_dettach;
    L->Name="Dmagh";
    return L;
}