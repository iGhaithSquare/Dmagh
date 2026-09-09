#ifdef DMAGH_WINDOW_GLFW 
#include "GLFW/glfw3.h"
#include <gaven.h>
void error_callback(int error, const char* description){
    GAVEN_WARN("Error: %s\n", description);
}
void *create_window(void){
    GLFWwindow* Win;
    GAVEN_ASSERT(glfwInit(),"Initializing glfw failed");
    glfwSetErrorCallback(error_callback);
    Win=glfwCreateWindow(1280,720,"DMAGH",NULL,NULL);
    GAVEN_ASSERT(Win,"Window creation failed");
    glfwMakeContextCurrent(Win);
    return Win;
}
void destroy_window(void *window){
    glfwDestroyWindow((GLFWwindow*)window);
    glfwTerminate();
}
void poll_window(application* app,void *window){
    app->Running=!glfwWindowShouldClose((GLFWwindow*)window);
    glfwPollEvents();
}
void render_window(void *window){
    glfwSwapBuffers((GLFWwindow*)window);
}
#endif