#ifndef WINDOW_H
#define WINDOW_H
void *create_window(void);
void destroy_window(void *window);
void poll_window(application* app,void *window);
void render_window(void *window);
#endif