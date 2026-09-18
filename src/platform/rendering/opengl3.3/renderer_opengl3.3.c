#ifdef DMAGH_RENDERER_OPENGL3_3
#include "quad_renderer_opengl3.3.h"
#include <stdlib.h>
#include <gaven.h>
shader* create_shader(const char* vertex_shader_char,const char* fragment_shader_char,const char* geometry_shader_char){
    int success;
    char Log[512];    
    GLuint vs,fs,gs;
    shader* Shader=malloc(sizeof(shader));
    Shader->Program=glCreateProgram();
    if(vertex_shader_char){
        vs =glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs,1,&vertex_shader_char,NULL);
        glCompileShader(vs);
        glAttachShader(Shader->Program,vs);
        glGetShaderiv(vs,GL_COMPILE_STATUS,&success);
        if(!success){
            glGetShaderInfoLog(vs,sizeof(Log),NULL,Log);
            GAVEN_ASSERT(0,"Failed to compile vertex shader with error: %s",Log);
        }
        glDeleteShader(vs);
    }
    if(fragment_shader_char){
        fs =glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs,1,&fragment_shader_char,NULL);
        glCompileShader(fs);
        glAttachShader(Shader->Program,fs);
        glGetShaderiv(fs,GL_COMPILE_STATUS,&success);
        if(!success){
            glGetShaderInfoLog(fs,sizeof(Log),NULL,Log);
            GAVEN_ASSERT(0,"Failed to compile fragment shader with error: %s",Log);
        }
        glDeleteShader(fs);
    }
    if(geometry_shader_char){
        gs =glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gs,1,&geometry_shader_char,NULL);
        glCompileShader(gs);
        glAttachShader(Shader->Program,gs);
        glGetShaderiv(gs,GL_COMPILE_STATUS,&success);
        if(!success){
            glGetShaderInfoLog(gs,sizeof(Log),NULL,Log);
            GAVEN_ASSERT(0,"Failed to compile geometry shader with error: %s",Log);
        }
        glDeleteShader(gs);
    }
    glLinkProgram(Shader->Program);
    glGetProgramiv(Shader->Program,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(Shader->Program,sizeof(Log),NULL,Log);
        GAVEN_ASSERT(0,"Failed to create shader program with error: %s",Log);
    }
    return Shader;
}
void destroy_shader(shader* Shader){
    glDeleteProgram(Shader->Program);
    free(Shader);
}
void use_shader(shader* Shader){
    glUseProgram(Shader->Program);
}
void* create_renderer(int width, int height,int Quad_Cap){
    renderer_api* R=(renderer_api*)malloc(sizeof(renderer_api));
    R->Width=width;
    R->Height=height;
    R->Quad_Renderer = create_quad_renderer(R,Quad_Cap);
    glm_ortho(0.0f,R->Width,0.0f,R->Height,-0.1f,100.0f,R->Uniform_Projection);
    glm_mat4_identity(R->Uniform_View);
    return R;
}
void destroy_renderer(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    destroy_quad_renderer(R->Quad_Renderer);
    free(R);
}
void change_camera_attribs(void* renderer,int x,int y, double scale){
    return;
}
void begin_frame(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    glClearColor(0.1f,0.1f,0.15f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    begin_frame_quad_renderer(R->Quad_Renderer);
}
void end_frame(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    end_frame_quad_renderer(R->Quad_Renderer);
}
void* get_framebuffer_texture(void){
    return NULL;
}
#endif