#ifndef RENDERER_H
#define RENDERER_H
#include "../../core/scene/curve.h"
void* create_renderer(int width, int height, int Quad_Cap);
void destroy_renderer(void* renderer);
void change_camera_attribs(void* renderer,int x,int y, double scale);
void draw_quad(void* renderer, int x,int y, int width, int height,float red,float green,float blue,float alpha);
void draw_curve_array(void* renderer,curve_array *Array);
void begin_frame(void* renderer);
void end_frame(void* renderer);
void* get_framebuffer_texture(void);
#endif