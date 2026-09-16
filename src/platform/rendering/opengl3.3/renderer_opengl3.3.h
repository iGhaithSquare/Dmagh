#ifdef DMAGH_RENDERER_OPENGL3_3
#ifndef renderer_opengl3_3_h
#define renderer_opengl3_3_h
#include "GL/glew.h"
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

    int* Indicies;
    int Indicies_Count;
    int Indicies_Cap;

    vertex_data* Vertecies_Data;
    int Vertecies_Count;
    int Vertecies_Cap;

} renderer_api;
#endif
#endif