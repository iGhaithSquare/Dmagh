#ifndef INPUT_H
#define INPUT_H
#include <gaven.h>
int is_key_pressed(int key);
int is_key_released(int key);
float get_mouse_x(void);
float get_mouse_y(void);
void init_input_system(float width,float height);
void destroy_input_system(void);
void input_polling(void);
void input_on_event(event* Event);
#endif