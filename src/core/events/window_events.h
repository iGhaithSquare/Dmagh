#include <gaven.h>
create_event_category(window,1);
typedef struct window_destroyed{
    event base;
}window_destroyed;
create_event_type(window_destroyed,10);
void window_destroyed_init(window_destroyed *Event);