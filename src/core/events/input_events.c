#include "dmagh_events.h"
#include <stdio.h>
void input_event_init(input_event *Event,int code){
    if(!Event) return;
    Event->base.Category_Flags = event_category_input;
    Event->base.Handled = 0;
    Event->base.To_String = NULL;
    Event->code=code;
}

static inline void key_pressed_to_string(event *Event, char* buffer, size_t buffer_size){
    if (!buffer) return;
    key_pressed* E=(key_pressed*)(Event);
    snprintf(buffer, buffer_size, "Key Pressed %d, %d times",E->base.code,E->repeat_count);
}
void key_pressed_init(key_pressed *Event,int code,int repeat_count){
    if(!Event) return;
    input_event_init(&Event->base,code);
    event* Base= &Event->base.base;
    Base->Type=event_type_key_pressed;
    Base->To_String=key_pressed_to_string;
    Base->Name="Key Pressed";
    Event->repeat_count=repeat_count;
}

static inline void key_released_to_string(event *Event, char* buffer, size_t buffer_size){
    if (!buffer) return;
    key_released* E=(key_released*)(Event);
    snprintf(buffer, buffer_size, "Key Released: %d",E->base.code);
}
void key_released_init(key_released *Event,int code){
    if(!Event) return;
    input_event_init(&Event->base,code);
    event* Base= &Event->base.base;
    Base->Type=event_type_key_released;
    Base->To_String=key_released_to_string;
    Base->Name="Key Released";
}

static inline void key_typed_to_string(event *Event, char* buffer, size_t buffer_size){
    if (!buffer) return;
    key_typed* E=(key_typed*)(Event);
    snprintf(buffer, buffer_size, "Key Typed: %d",E->base.code);
}
void key_typed_init(key_typed *Event,int code){
    if(!Event) return;
    input_event_init(&Event->base,code);
    event* Base= &Event->base.base;
    Base->Type=event_type_key_typed;
    Base->To_String=key_typed_to_string;
    Base->Name="Key Typed";
}

static inline void mouse_moved_to_string(event *Event, char* buffer, size_t buffer_size){
    if (!buffer) return;
    mouse_moved* E=(mouse_moved*)(Event);
    snprintf(buffer, buffer_size, "Mouse moved to: (%f,%f)",E->x,E->y);
}
void mouse_moved_init(mouse_moved *Event,float x,float y){
    if(!Event) return;
    Event->base.Category_Flags = event_category_input;
    Event->base.Handled = 0;
    Event->base.To_String = NULL;
    Event->base.Type=event_type_mouse_moved;
    Event->base.Name="Mouse Moved";
    Event->x=x;
    Event->y=y;
    Event->base.To_String=mouse_moved_to_string;
}

static inline void mouse_scrolled_to_string(event *Event, char* buffer, size_t buffer_size){
    if (!buffer) return;
    mouse_scrolled* E=(mouse_scrolled*)(Event);
    snprintf(buffer, buffer_size, "Mouse scrolled: (%f,%f)",E->dx,E->dy);
}
void mouse_scrolled_init(mouse_scrolled *Event,float dx,float dy){
    if(!Event) return;
    Event->base.Category_Flags = event_category_input;
    Event->base.Handled = 0;
    Event->base.To_String = NULL;
    Event->base.Type=event_type_mouse_scrolled;
    Event->base.To_String=mouse_scrolled_to_string;
    Event->base.Name="Mouse Scrolled";
    Event->dx=dx;
    Event->dy=dy;
}
