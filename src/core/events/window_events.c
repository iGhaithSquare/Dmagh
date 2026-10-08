#include "dmagh_events.h"
#include <stdio.h>
static inline void window_destroyed_to_string(event *Event, char* buffer, size_t buffer_size){
    if (!buffer) return;
    snprintf(buffer, buffer_size, "Window Destroyed");
}
void window_destroyed_init(window_destroyed *Event){
    if(!Event) return;
    Event->base.Category_Flags = event_category_window;
    Event->base.Handled = 0;
    Event->base.Name = "Window Destroyed";
    Event->base.To_String = window_destroyed_to_string;
    Event->base.Type = event_type_window_destroyed;
}

static inline void window_resize_to_string(event *Event, char* buffer, size_t buffer_size){
    if (!buffer) return;
    window_resize* E=(window_resize*)(Event);
    snprintf(buffer, buffer_size, "Window Resized: x:%f\ty:%f",E->width,E->height);
}
void window_resize_init(window_resize *Event,float width, float height){
    if(!Event) return;
    Event->base.Category_Flags = event_category_window;
    Event->base.Handled = 0;
    Event->base.Name = "Window Resize";
    Event->base.To_String = window_resize_to_string;
    Event->base.Type = event_type_window_resize;
    Event->width=width;
    Event->height=height;
}

static inline void window_pause_to_string(event *Event, char* buffer, size_t buffer_size){
    if (!buffer) return;
    window_pause* E=(window_pause*)(Event);
    if(E->state)
        snprintf(buffer, buffer_size, "Window Paused");
    else
        snprintf(buffer, buffer_size, "Window Unpaused");
}
void window_pause_init(window_pause *Event,uint8_t state){
    if(!Event) return;
    Event->base.Category_Flags = event_category_window;
    Event->base.Handled = 0;
    Event->base.Name = "Window Pause";
    Event->base.To_String = window_pause_to_string;
    Event->base.Type = event_type_window_pause;
    Event->state=state;
}