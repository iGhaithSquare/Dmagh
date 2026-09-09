#ifndef DMAGH_LAYER_H
#define DMAGH_LAYER_H
#include <gaven.h>
create_layer_phase(polling_callback,0);
create_layer_phase(update_callback,1);
create_layer_phase(render_callback,2);
create_layer_phase(gui_render_callback,3);
layer* create_dmagh_layer(application* app);
#endif