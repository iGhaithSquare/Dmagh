#ifdef DMAGH_RENDERER_OPENGL3_3
#ifndef renderer_opengl3_3_h
#define renderer_opengl3_3_h
#include "GL/glew.h"
#include "cglm/cglm.h"
typedef struct vertex_data{
    float x,y,z;
    float r,g,b,a;
    float u,v;
} vertex_data;
typedef struct renderer_api{
    int Width;
    int Height;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint ShaderProgram;

    GLuint Uniform_Projection_Location;
    mat4 Uniform_Projection;

    GLuint Uniform_View_Location;
    mat4 Uniform_View;

    int* Indices;
    int Indices_Count;
    int Indices_Cap;

    vertex_data* Vertices_Data;
    int Vertices_Count;
    int Vertices_Cap;

} renderer_api;
#endif
#endif