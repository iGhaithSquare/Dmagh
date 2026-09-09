#include "dmagh_layer.h"
#include <stdlib.h>
typedef struct dmagh_layer_data{
    int a;
} dmagh_layer_data;

void dmagh_on_dettach(layer*self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
}
void polling_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    GAVEN_INFO("POLL");
}
void update_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    GAVEN_INFO("UPDATE");
}
void render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    GAVEN_INFO("RENDER");
}
void gui_render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    GAVEN_INFO("GUI RENDER");
}

layer* create_dmagh_layer(void){
    layer* L=calloc(1,sizeof(layer));
    dmagh_layer_data *data = (dmagh_layer_data*)malloc(sizeof(dmagh_layer_data));
    bind_layer_phase(L,layer_phase_polling,polling_callback);
    bind_layer_phase(L,layer_phase_update_callback,update_callback);
    bind_layer_phase(L,layer_phase_render_callback,render_callback);
    bind_layer_phase(L,layer_phase_gui_render_callback,gui_render_callback);
    
    L->LayerData=data;
    L->OnDettach=dmagh_on_dettach;
    L->Name="Dmagh";
    return L;
}
