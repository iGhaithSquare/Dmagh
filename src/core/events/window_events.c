#include "window_events.h"
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