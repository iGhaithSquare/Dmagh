#ifdef DMAGH_RENDERER_OPENGL3_3
#ifndef RENDERER_OPENGL3_3_H
#define RENDERER_OPENGL3_3_H
#include "GL/glew.h"
#include "cglm/cglm.h"
typedef struct quad_renderer quad_renderer;
typedef struct{
    GLuint Program;
} shader;
typedef struct renderer_api{
    int Width;
    int Height;
    quad_renderer* Quad_Renderer;
    mat4 Uniform_View;
    mat4 Uniform_Projection;
} renderer_api;
shader* create_shader(const char* vertex_shader_char,const char* fragment_shader_char,const char* geometry_shader_char);
void destroy_shader(shader* Shader);
void use_shader(shader* Shader);
#endif
#endif