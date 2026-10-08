#include "input.h"
#include <stdlib.h>
#include "events/dmagh_events.h"
#include <string.h>
typedef struct input{
    int key_pressed[512];
    int key_released[512];
    float mouse_x;
    float mouse_y;
    float width;
    float height;
} input;
static input *Input=NULL;
void init_input_system(float width,float height){
    Input=calloc(1,sizeof(input));
    Input->width=width;
    Input->height=height;
}
void destroy_input_system(void){
    free(Input);
}
int is_key_pressed(int key){
    return Input->key_pressed[key];
}

int is_key_released(int key){
    return Input->key_released[key];
}
float get_mouse_x(void){
    return Input->mouse_x;
}
float get_mouse_y(void){
    return Input->mouse_y;
}
int input_key_released(key_released* Event){
    Input->key_released[Event->base.code]=1;
    return 1;
}
int input_on_mouse_moved(mouse_moved* E){
    Input->mouse_x=E->x;
    Input->mouse_y=Input->height-E->y;
    return 1;
}
int input_window_resize(window_resize* E){
    if(E->height&&E->width){
        Input->width=E->width;
        Input->height=E->height;
    }
    return 0;
}
void input_on_event(event* Event){
    EVENT_DISPATCH(Event,key_released,input_key_released);
    EVENT_DISPATCH(Event,mouse_moved,input_on_mouse_moved);
    EVENT_DISPATCH(Event,window_resize,input_window_resize);
}

void input_polling(void){
    memset(Input->key_pressed,0,sizeof(Input->key_pressed));
    memset(Input->key_released,0,sizeof(Input->key_released));
}