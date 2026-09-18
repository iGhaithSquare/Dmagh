#ifdef DMAGH_RENDERER_OPENGL3_3
#ifndef QUAD_RENDERER_OPENGL3_3_H
#define QUAD_RENDERER_OPENGL3_3_H
#include "renderer_opengl3.3.h"
typedef struct quad_vertex_data{
    float x,y,z;
    float r,g,b,a;
    float u,v;
} quad_vertex_data;
struct quad_renderer{
    
    renderer_api* Renderer_api;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    shader* Shader;

    GLint Uniform_Projection_Location;

    GLint Uniform_View_Location;

    int* Indices;
    int Indices_Count;
    int Indices_Cap;

    quad_vertex_data* Vertices_Data;
    int Vertices_Count;
    int Vertices_Cap;
};
quad_renderer* create_quad_renderer(renderer_api* renderer,int Quad_Cap);
void destroy_quad_renderer(quad_renderer* renderer);
void begin_frame_quad_renderer(quad_renderer* R);
void end_frame_quad_renderer(quad_renderer* R);
#endif
#endif