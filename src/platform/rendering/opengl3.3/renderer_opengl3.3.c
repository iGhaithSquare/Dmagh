#ifdef DMAGH_RENDERER_OPENGL3_3
#include "renderer_opengl3.3.h"
#include <stdlib.h>
#include <gaven.h>
static const float vertices[]={
    0.5f,0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    -0.5f,-0.5f,0.0f,
    -0.5f,0.5f,0.0f
};
static const unsigned int indices[]={
    0,1,3,
    1,2,3
};
void* create_renderer(int    width, int height){
    int success;
    char Log[512];
    renderer_api* R=(renderer_api*)malloc(sizeof(renderer_api));
    R->Width=width;
    R->Height=height;
    GLenum Result =glewInit();
    GAVEN_ASSERT(!Result,"GLEW initialization failed: %s",(const char*)glewGetErrorString(Result));
    
    glGenVertexArrays(1,&R->VAO);    
    glGenBuffers(1,&R->VBO);
    glGenBuffers(1,&R->EBO);

    glBindVertexArray(R->VAO);   
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,R->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    const char *vs_src=  "#version 330 core\n"
                                    "layout(location = 0) in vec3 aPos;\n"
                                    "out vec3 v_Color;\n"
                                    "void main(){\n"
                                    "   gl_Position=vec4(aPos,1.0);\n"
                                    "   v_Color=aPos*0.5+0.5;\n"
                                    "}";
    const char *fs_src=  "#version 330 core\n"
                                    "layout(location = 0) out vec4 color;\n"
                                    "in vec3 v_Color;\n"
                                    "void main(){\n"
                                    "   color=vec4(v_Color,1.0);\n"
                                    "}";
    
    GLuint vs =glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vs_src,NULL);
    glCompileShader(vs);
    glGetShaderiv(vs,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vs,sizeof(Log),NULL,Log);
        GAVEN_ASSERT(0,"Failed to compile vertex shader with error: %s",Log);
    }
    GLuint fs =glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fs_src,NULL);
    glCompileShader(fs);
    glGetShaderiv(fs,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fs,sizeof(Log),NULL,Log);
        GAVEN_ASSERT(0,"Failed to compile fragment shader with error: %s",Log);
    }

    R->ShaderProgram=glCreateProgram();
    glAttachShader(R->ShaderProgram,vs);
    glAttachShader(R->ShaderProgram,fs);
    glLinkProgram(R->ShaderProgram);
    glGetProgramiv(R->ShaderProgram,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(R->ShaderProgram,sizeof(Log),NULL,Log);
        GAVEN_ASSERT(0,"Failed to create shader program with error: %s",Log);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return R;
}
void destroy_renderer(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    glDeleteProgram(R->ShaderProgram);
    glDeleteBuffers(1,&R->VBO);
    glDeleteBuffers(1,&R->EBO);
    glDeleteVertexArrays(1,&R->VAO);
    free(R);
}
void change_camera_attribs(void* renderer,int x,int y, double scale){
    return;
}
void draw_quad(void* renderer, int x,int y, int width, int height){
    return;
}
void begin_frame(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    glClearColor(0.1f,0.1f,0.15f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(R->ShaderProgram);
    glBindVertexArray(R->VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}
void end_frame(void* renderer){
    return;
}
void* get_framebuffer_texture(void){
    return NULL;
}
#endif