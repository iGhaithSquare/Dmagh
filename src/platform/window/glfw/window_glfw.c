#ifdef DMAGH_WINDOW_GLFW 
#include "GLFW/glfw3.h"
#include "../../../core/events/dmagh_events.h"
void window_destroy_callback(void){
    window_destroyed E;
    window_destroyed_init(&E);
    application_event_callback(&E.base);
}
void window_error_callback(int error, const char* description){
    GAVEN_WARN("Error: %s\n", description);
}
void window_resize_callback(GLFWwindow* window,int width,int height){
    window_resize E;
    window_resize_init(&E,(float)width,(float)height);
    application_event_callback(&E.base);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch (action){
    case (GLFW_PRESS):{
        key_pressed E;
        key_pressed_init(&E,key,0);
        application_event_callback(&E.base.base);
        break;
    }
    case (GLFW_RELEASE):{
        key_released E;
        key_released_init(&E,key);
        application_event_callback(&E.base.base);
        break;
    }
    case (GLFW_REPEAT):{
        key_pressed E;
        key_pressed_init(&E,key,1);
        application_event_callback(&E.base.base);
        break;
    }
    default:
        break;
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    switch (action){
    case (GLFW_PRESS):{
        key_pressed E;
        key_pressed_init(&E,button,0);
        application_event_callback(&E.base.base);
        break;
    }
    case (GLFW_RELEASE):{
        key_released E;
        key_released_init(&E,button);
        application_event_callback(&E.base.base);
        break;
    }
    case (GLFW_REPEAT):{
        key_pressed E;
        key_pressed_init(&E,button,1);
        application_event_callback(&E.base.base);
        break;
    }
    default:
        break;
    }
}
void char_callback(GLFWwindow* window, unsigned int key){
    key_typed E;
    key_typed_init(&E,key);
    application_event_callback(&E.base.base);
}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
    mouse_scrolled E;
    mouse_scrolled_init(&E,xOffset,yOffset);
    application_event_callback(&E.base);
}
void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos){
    mouse_moved E;
    mouse_moved_init(&E,xPos,yPos);
    application_event_callback(&E.base);
}

void *create_window(int *width, int *height){
    GLFWwindow* Win;
    GAVEN_ASSERT(glfwInit(),"Initializing glfw failed");
    glfwSetErrorCallback(window_error_callback);
    

    #ifdef DMAGH_RENDERER_OPENGL3_3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    #endif

    Win=glfwCreateWindow(*width,*height,"DMAGH",NULL,NULL);
    GAVEN_ASSERT(Win,"Window creation failed");
    glfwMakeContextCurrent(Win);
    glfwSwapInterval(1);


    glfwSetFramebufferSizeCallback(Win,window_resize_callback);
    glfwSetKeyCallback(Win,key_callback);
    glfwSetCharCallback(Win,char_callback);
    glfwSetMouseButtonCallback(Win,mouse_button_callback);
    glfwSetCursorPosCallback(Win,cursor_pos_callback);
    glfwSetScrollCallback(Win,scroll_callback);

    return Win;
}
void destroy_window(void *window){
    glfwDestroyWindow((GLFWwindow*)window);
    glfwTerminate();
}
void poll_window(void **window){
    GLFWwindow* win = *(GLFWwindow**)window;
    if(glfwWindowShouldClose(win))
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