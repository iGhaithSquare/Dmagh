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
#include "../window.h"
typedef struct awindow{
    EGLDisplay Display;
    EGLSurface Surface;
    EGLContext Context;
    EGLConfig Config;
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

void destroy_window_surface(void *window){
    awindow* W=(awindow*)window;
    #ifdef DMAGH_RENDERER_OPENGLES3_2
    eglMakeCurrent(W->Display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
    eglDestroySurface(W->Display,W->Surface);
    W->Surface=EGL_NO_SURFACE;
    #else
    #endif
}
void create_window_surface(void* window){
    awindow* Win=(awindow*)window;
    GAVEN_ASSERT(Native_Window,"Failed to find native window");
    #ifdef DMAGH_RENDERER_OPENGLES3_2
    Win->Surface=eglCreateWindowSurface(Win->Display,Win->Config,(EGLNativeWindowType)Native_Window,NULL);
    GAVEN_ASSERT(Win->Surface!=EGL_NO_SURFACE,"Failed to create EGL surface");
    GAVEN_ASSERT(eglMakeCurrent(Win->Display,Win->Surface,Win->Surface,Win->Context),"Failed to make EGL context current");
    eglSwapInterval(Win->Display,1);
    #else
    #endif
}
static void android_cmd_callback(struct android_app* app,int32_t cmd){
    void** win=(void**)app->userData;
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:{
            Native_Window=app->window;
            if(win){
                create_window_surface(*win);
                window_pause E;
                window_pause_init(&E,0);
                application_event_callback(&E.base);
            }
            break;
        }
        case APP_CMD_DESTROY:{
            window_destroyed E;
            window_destroyed_init(&E);
            application_event_callback(&E.base);
            break;
        }
        case APP_CMD_TERM_WINDOW:
            Native_Window=NULL;
            destroy_window_surface(*win);
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:{
            window_pause E;
            window_pause_init(&E,1);
            application_event_callback(&E.base);
            break;
        }
        case APP_CMD_START:
        case APP_CMD_RESUME:
            break;
        default:
            break;
    }
}
void hide_android_ui(ANativeActivity* activity){
    JNIEnv* env=activity->env;
    jclass activity_class=(*env)->GetObjectClass(env,activity->clazz);
    jmethodID get_window=(*env)->GetMethodID(env,activity_class,"getWindow","()Landroid/view/Window;");
    jobject window=(*env)->CallObjectMethod(env,activity->clazz,get_window);
    jclass window_class=(*env)->GetObjectClass(env,window);
    jmethodID get_decor_view=(*env)->GetMethodID(env,window_class,"getDecorView","()Landroid/view/View;");
    jobject decor_view=(*env)->CallObjectMethod(env,window,get_decor_view);
    jclass view_class=(*env)->GetObjectClass(env,decor_view);
    jmethodID set_system_ui_visibility=(*env)->GetMethodID(env,view_class,"setSystemUiVisibility","(I)V");
    jint flags=2|4|256|512|1024|4096;
    (*env)->CallVoidMethod(env,decor_view,set_system_ui_visibility,flags);
    (*env)->DeleteLocalRef(env,view_class);
    (*env)->DeleteLocalRef(env,decor_view);
    (*env)->DeleteLocalRef(env,window_class);
    (*env)->DeleteLocalRef(env,window);
    (*env)->DeleteLocalRef(env,activity_class);
}
static void android_window_focus_callback(ANativeActivity* activity,int has_focus){
    if(has_focus)
        hide_android_ui(activity);
}
void android_main(struct android_app* app){
    app->onAppCmd=temp_android_cmd_callback;
    app->userData=NULL;
    APP=app;
    app->activity->callbacks->onWindowFocusChanged=android_window_focus_callback;
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
    if(width)
        *width=ANativeWindow_getWidth(Native_Window);
    if(height)
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
    EGLint Config_Count;
    GAVEN_ASSERT(eglChooseConfig(Win->Display,ConfigAttribs,&Win->Config,1,&Config_Count)&&Config_Count>0,"Failed to choose EGL config");
    Win->Surface=eglCreateWindowSurface(Win->Display,Win->Config,(EGLNativeWindowType)Native_Window,NULL);
    GAVEN_ASSERT(Win->Surface!=EGL_NO_SURFACE,"Failed to create EGL surface");
    const EGLint Context_Attribs[]={
        EGL_CONTEXT_CLIENT_VERSION,3,EGL_NONE
    };
    Win->Context=eglCreateContext(Win->Display,Win->Config,EGL_NO_CONTEXT,Context_Attribs);
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
void poll_window(void **window){
    APP->userData=window;
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