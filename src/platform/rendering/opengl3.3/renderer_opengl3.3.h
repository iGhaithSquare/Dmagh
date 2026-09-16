#ifdef DMAGH_RENDERER_OPENGL3_3
#ifndef renderer_opengl3_3_h
#define renderer_opengl3_3_h
#include "GL/glew.h"
#include "cglm/cglm.h"
typedef struct vertex_data{
    float x,y,z;
    float r,g,b,a;
} vertex_data;
typedef struct renderer_api{
    int Width;
    int Height;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint ShaderProgram;

    GLuint Camera_Transform_Location;
    mat4 Camera_Transform;

    int* Indices;
    int Indices_Count;
    int Indices_Cap;

    vertex_data* Vertices_Data;
    int Vertices_Count;
    int Vertices_Cap;

} renderer_api;
#endif
#endif