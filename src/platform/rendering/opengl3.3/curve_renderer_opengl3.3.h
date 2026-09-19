#ifdef DMAGH_RENDERER_OPENGL3_3
#ifndef CURVE_RENDERER_OPENGL3_3_H
#define CURVE_RENDERER_OPENGL3_3_H
#include "renderer_opengl3.3.h"
#include "../../../core/scene/curve.h"
struct curve_renderer{    
    renderer_api* Renderer_api;
    GLuint VAO;
    GLuint VBO;
    shader* Shader;
    GLint Uniform_Projection_Location;
    GLint Uniform_View_Location;

    curve_point* Vertices_Data;
    int Vertices_Count;
    int Vertices_Cap;
};
curve_renderer* create_curve_renderer(renderer_api* renderer);
void destroy_curve_renderer(curve_renderer* renderer);
void begin_frame_curve_renderer(curve_renderer* R);
void end_frame_curve_renderer(curve_renderer* R);
#endif
#endif