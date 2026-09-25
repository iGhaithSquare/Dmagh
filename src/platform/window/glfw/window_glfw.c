#ifdef DMAGH_WINDOW_GLFW 
#include "GLFW/glfw3.h"
#include "../../../core/events/window_events.h"
void window_destroy_callback(void){
    window_destroyed E;
    window_destroyed_init(&E);
    application_event_callback(&E.base);
}
void error_callback(int error, const char* description){
    GAVEN_WARN("Error: %s\n", description);
}
void *create_window(int *width, int *height){
    GLFWwindow* Win;
    GAVEN_ASSERT(glfwInit(),"Initializing glfw failed");
    glfwSetErrorCallback(error_callback);
    

    #ifdef DMAGH_RENDERER_OPENGL3_3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    #endif

    Win=glfwCreateWindow(*width,*height,"DMAGH",NULL,NULL);
    GAVEN_ASSERT(Win,"Window creation failed");
    glfwMakeContextCurrent(Win);
    glfwSwapInterval(1);
    return Win;
}
void destroy_window(void *window){
    glfwDestroyWindow((GLFWwindow*)window);
    glfwTerminate();
}
void poll_window(void *window){
    if(glfwWindowShouldClose((GLFWwindow*)window))
        window_destroy_callback();
    glfwPollEvents();
}
void render_window(void *window){
    glfwSwapBuffers((GLFWwindow*)window);
}
void change_vsync_state(void* window,int enabled){
    glfwSwapInterval(enabled);
}
#endif