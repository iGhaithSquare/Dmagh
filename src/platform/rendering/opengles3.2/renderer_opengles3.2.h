#ifdef DMAGH_RENDERER_OPENGLES3_2
#ifndef RENDERER_OPENGLES3_2_H
#define RENDERER_OPENGLES3_2_H
#include "GLES3/gl32.h"
#include "cglm/cglm.h"
typedef struct quad_renderer quad_renderer;
typedef struct curve_renderer curve_renderer;
typedef struct{
    GLuint Program;
} shader;
typedef struct renderer_api{
    int Width;
    int Height;
    quad_renderer* Quad_Renderer;
    curve_renderer* Curve_Renderer;
    mat4 Uniform_View;
    mat4 Uniform_Projection;
    
    float cx,cy,cz;
    float crx,cry,crz;
    float czoom;
    uint8_t c_ortho;
} renderer_api;
shader* create_shader(const char* vertex_shader_char,const char* fragment_shader_char,const char* geometry_shader_char);
void destroy_shader(shader* Shader);
void use_shader(shader* Shader);
#endif
#endif