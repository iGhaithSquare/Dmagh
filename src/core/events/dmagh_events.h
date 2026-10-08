#ifndef DMAGH_EVENTS_H
#define DMAGH_EVENTS_H
#include <gaven.h>
create_event_category(window,1);
create_event_category(input,2);


/* WINDOW EVENTS */
typedef struct window_destroyed{
    event base;
}window_destroyed;
create_event_type(window_destroyed,10);
void window_destroyed_init(window_destroyed *Event);

typedef struct window_resize{
    event base;
    float width,height;
}window_resize;
create_event_type(window_resize,11);
void window_resize_init(window_resize *Event,float width, float height);

typedef struct window_pause{
    event base;
    uint8_t state;
}window_pause;
create_event_type(window_pause,12);
void window_pause_init(window_pause *Event,uint8_t state);

/* Input events */
typedef struct input_event{
    event base;
    int code;
}input_event;
create_event_type(input_event,20);
void input_event_init(input_event *Event, int code);

typedef struct key_pressed{
    input_event base;
    int repeat_count;
}key_pressed;
create_event_type(key_pressed,21);
void key_pressed_init(key_pressed *Event,int code,int repeat_count);

typedef struct key_released{
    input_event base;
}key_released;
create_event_type(key_released,22);
void key_released_init(key_released *Event,int code);

typedef struct key_typed{
    input_event base;
}key_typed;
create_event_type(key_typed,23);
void key_typed_init(key_typed *Event,int code);

typedef struct mouse_moved{
    event base;
    float x;
    float y;
}mouse_moved;
create_event_type(mouse_moved,24);
void mouse_moved_init(mouse_moved *Event,float x,float y);

typedef struct mouse_scrolled{
    event base;
    float dx;
    float dy;
}mouse_scrolled;
create_event_type(mouse_scrolled,25);
void mouse_scrolled_init(mouse_scrolled *Event,float dx,float dy);
#endif
