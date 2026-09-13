#ifdef DMAGH_RENDERER_OPENGL3_3
#ifndef renderer_opengl3_3_h
#define renderer_opengl3_3_h
#include "GL/glew.h"
typedef struct renderer_api{
    int Width;
    int Height;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint ShaderProgram;
} renderer_api;
#endif
#endif