#include <gaven.h>
create_event_category(window,1);
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