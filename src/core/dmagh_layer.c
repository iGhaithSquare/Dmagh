#include "dmagh_layer.h"
#include <stdlib.h>
#include <GL/glew.h>
#include "glfw/glfw3.h"
void error_callback(int error, const char* description){
    GAVEN_WARN("Error: %s\n", description);
}
typedef struct dmagh_layer_data{
    GLFWwindow* window;
    application* app;
} dmagh_layer_data;
void dmagh_on_attach(layer* self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    GAVEN_ASSERT(glfwInit(),"Initializing glfw failed");
    glfwSetErrorCallback(error_callback);
    Data->window=glfwCreateWindow(1280,720,"DMAGH",NULL,NULL);
    GAVEN_ASSERT(Data->window,"Window creation failed");
    glfwMakeContextCurrent(Data->window);
}
void dmagh_on_dettach(layer* self){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    glfwDestroyWindow(Data->window);
    glfwTerminate();
}
void polling_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    Data->app->Running=!glfwWindowShouldClose(Data->window);
    glfwPollEvents();
}
void update_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
}
void render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(Data->window);
}
void gui_render_callback(layer* self, void* ctx){
    dmagh_layer_data* Data = (dmagh_layer_data*)self->LayerData;
}

layer* create_dmagh_layer(application* app){
    layer* L=calloc(1,sizeof(layer));
    dmagh_layer_data *data = (dmagh_layer_data*)malloc(sizeof(dmagh_layer_data));
    bind_layer_phase(L,layer_phase_polling,polling_callback);
    bind_layer_phase(L,layer_phase_update_callback,update_callback);
    bind_layer_phase(L,layer_phase_render_callback,render_callback);
    bind_layer_phase(L,layer_phase_gui_render_callback,gui_render_callback);
    data->app=app;
    L->LayerData=data;
    L->OnAttach=dmagh_on_attach;
    L->OnDettach=dmagh_on_dettach;
    L->Name="Dmagh";
    return L;
}
