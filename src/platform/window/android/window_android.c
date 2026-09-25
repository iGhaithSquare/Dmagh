#ifdef DMAGH_WINDOW_ANDROID
#include <android/native_window.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include "../../../core/events/window_events.h"
#include <stdlib.h>
#include "../../../core/dmagh_layer.h"
#ifdef DMAGH_RENDERER_OPENGLES3_2
#include <EGL/egl.h>
#include <EGL/eglext.h>
typedef struct awindow{
    EGLDisplay Display;
    EGLSurface Surface;
    EGLContext Context;
} awindow;
#else
#endif
static ANativeWindow* Native_Window=NULL;
struct android_app* APP=NULL;
static void temp_android_cmd_callback(struct android_app* app,int32_t cmd){
    if(cmd==APP_CMD_INIT_WINDOW){
        Native_Window=app->window;
    }
}
static void android_cmd_callback(struct android_app* app,int32_t cmd){
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            Native_Window=app->window;
            break;
        case APP_CMD_TERM_WINDOW:
            window_destroyed E;
            window_destroyed_init(&E);
            application_event_callback(&E.base);
            break;
        default:
            break;
    }
}
void android_main(struct android_app* app){
    app->onAppCmd=temp_android_cmd_callback;
    APP=app;
    while(!Native_Window){
        int events;
        struct android_poll_source* source;
        ALooper_pollOnce(-1,NULL,&events,(void**)&source);
        if(source)
            source->process(app,source);
    }
    application* mapp=create_gaven_application();
    mapp->Running=1;
    app->onAppCmd=android_cmd_callback;
    add_layer(mapp->Layer_Registry,create_dmagh_layer(mapp));
    run_application();
    destroy_application();
}
void error_callback(int error, const char* description){
    GAVEN_WARN("Error: %s\n", description);
}
void *create_window(int *width, int *height){
    GAVEN_ASSERT(Native_Window,"Failed to find native window");
    awindow* Win=(awindow*)malloc(sizeof(awindow));
    *width=ANativeWindow_getWidth(Native_Window);
    *height=ANativeWindow_getHeight(Native_Window);
    

    #ifdef DMAGH_RENDERER_OPENGLES3_2
    Win->Display=eglGetDisplay(EGL_DEFAULT_DISPLAY);
    GAVEN_ASSERT(Win->Display!=EGL_NO_DISPLAY,"Failed to get EGL display");
    GAVEN_ASSERT(eglInitialize(Win->Display,NULL,NULL),"Failed to initialize EGL display");
    eglBindAPI(EGL_OPENGL_ES_API);
    const EGLint ConfigAttribs[]={
        EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,EGL_OPENGL_ES3_BIT_KHR,
        EGL_RED_SIZE,8,
        EGL_GREEN_SIZE,8,
        EGL_BLUE_SIZE,8,
        EGL_ALPHA_SIZE,8,
        EGL_DEPTH_SIZE,24,
        EGL_NONE
    };
    EGLConfig Config;
    EGLint Config_Count;
    GAVEN_ASSERT(eglChooseConfig(Win->Display,ConfigAttribs,&Config,1,&Config_Count)&&Config_Count>0,"Failed to choose EGL config");
    Win->Surface=eglCreateWindowSurface(Win->Display,Config,(EGLNativeWindowType)Native_Window,NULL);
    GAVEN_ASSERT(Win->Surface!=EGL_NO_SURFACE,"Failed to create EGL surface");
    const EGLint Context_Attribs[]={
        EGL_CONTEXT_CLIENT_VERSION,3,EGL_NONE
    };
    Win->Context=eglCreateContext(Win->Display,Config,EGL_NO_CONTEXT,Context_Attribs);
    GAVEN_ASSERT(Win->Context!=EGL_NO_CONTEXT,"Failed to create EGL context");
    GAVEN_ASSERT(eglMakeCurrent(Win->Display,Win->Surface,Win->Surface,Win->Context),"Failed to make EGL context current");
    eglSwapInterval(Win->Display,1);
    #else
    #endif
    return Win;
}
void destroy_window(void *window){
    awindow* W=(awindow*)window;
    #ifdef DMAGH_RENDERER_OPENGLES3_2
    eglMakeCurrent(W->Display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
    eglDestroyContext(W->Display,W->Context);
    eglDestroySurface(W->Display,W->Surface);
    eglTerminate(W->Display);
    #else
    #endif
    free(W);
}
void poll_window(void *window){
    int events;
    struct android_poll_source* source;
    ALooper_pollOnce(0,NULL,&events,(void**)&source);
    if(source)
        source->process(APP,source);
}
void render_window(void *window){
    awindow* W=(awindow*)window;
    #ifdef DMAGH_RENDERER_OPENGLES3_2
    eglSwapBuffers(W->Display,W->Surface);
    #else
    #endif
}
void change_vsync_state(void* window,int enabled){
    awindow* W=(awindow*)window;
    #ifdef DMAGH_RENDERER_OPENGLES3_2
    eglSwapInterval(W->Display,enabled);
    #else
    #endif
}
#endif