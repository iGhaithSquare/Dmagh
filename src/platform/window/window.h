#ifndef WINDOW_H
#define WINDOW_H
void *create_window(int width, int height);
void destroy_window(void *window);
void poll_window(application* app,void *window);
void render_window(void *window);
void change_vsync_state(void* window,int enabled);
#endif