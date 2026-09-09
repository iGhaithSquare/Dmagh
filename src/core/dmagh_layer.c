#include "dmagh_layer.h"
#include <stdlib.h>
#include "../platform/window/window.h"
#include "GL/glew.h"
typedef struct dmagh_layer_data{
    void* Window;
    application* App;
} dmagh_layer_data;
void dmagh_on_attach(layer* self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    Data->Window=create_window();
}
void dmagh_on_dettach(layer* self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    destroy_window(Data->Window);
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
    glClear(GL_COLOR_BUFFER_BIT);
}
void gui_render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
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
