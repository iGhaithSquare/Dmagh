#ifdef DMAGH_RENDERER_OPENGL3_3
#include "renderer_opengl3.3.h"
#include <stdlib.h>
#include <gaven.h>
void* create_renderer(int width, int height,int Quad_Cap){
    int success;
    char Log[512];
    renderer_api* R=(renderer_api*)malloc(sizeof(renderer_api));
    R->Width=width;
    R->Height=height;

    R->Vertecies_Cap=Quad_Cap*4;
    R->Vertecies_Data=malloc(sizeof(vertex_data)*R->Vertecies_Cap);
    GAVEN_ASSERT(R->Vertecies_Data,"Couldnt allocate memory for opengl 3.3 renderer vertex array");
    R->Vertecies_Count=0;

    R->Indicies_Cap=Quad_Cap*6;
    R->Indicies=malloc(sizeof(int)*R->Indicies_Cap);
    GAVEN_ASSERT(R->Indicies,"Couldnt allocate memory for opengl 3.3 renderer index array");
    R->Indicies_Count=0;


    GLenum Result =glewInit();
    GAVEN_ASSERT(!Result,"GLEW initialization failed: %s",(const char*)glewGetErrorString(Result));
    
    glGenVertexArrays(1,&R->VAO);    
    glGenBuffers(1,&R->VBO);
    glGenBuffers(1,&R->EBO);

    glBindVertexArray(R->VAO);   
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data)*R->Vertecies_Count,R->Vertecies_Data,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,R->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*R->Indicies_Count,R->Indicies,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_data),NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(vertex_data),(void*)offsetof(vertex_data,r));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    const char *vs_src=  "#version 330 core\n"
                                    "layout(location = 0) in vec3 aPos;\n"
                                    "layout(location = 1) in vec4 aColor;\n"
                                    "out vec4 v_Color;\n"
                                    "void main(){\n"
                                    "   gl_Position=vec4(aPos,1.0);\n"
                                    "   v_Color=aColor;\n"
                                    "}";
    const char *fs_src=  "#version 330 core\n"
                                    "layout(location = 0) out vec4 color;\n"
                                    "in vec4 v_Color;\n"
                                    "void main(){\n"
                                    "   color=v_Color;\n"
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
    free(R->Indicies);
    free(R->Vertecies_Data);
    glDeleteProgram(R->ShaderProgram);
    glDeleteBuffers(1,&R->VBO);
    glDeleteBuffers(1,&R->EBO);
    glDeleteVertexArrays(1,&R->VAO);
    free(R);
}
void change_camera_attribs(void* renderer,int x,int y, double scale){
    return;
}
void draw_quad(void* renderer, int x,int y, int width, int height,float red,float green,float blue,float alpha){
    renderer_api* R=(renderer_api*)renderer;
    if(R->Indicies_Count==R->Indicies_Cap-5){
        R->Indicies_Cap*=2;
        R->Indicies=realloc(R->Indicies,sizeof(int)*R->Indicies_Cap);
        GAVEN_ASSERT(R->Indicies,"Couldnt allocate memory for opengl 3.3 renderer index array");
    }
    if(R->Vertecies_Count==R->Vertecies_Cap-3){
        R->Vertecies_Cap*=2;
        R->Vertecies_Data=realloc(R->Vertecies_Data,sizeof(vertex_data)*R->Vertecies_Cap);
        GAVEN_ASSERT(R->Vertecies_Data,"Couldnt allocate memory for opengl 3.3 renderer index array");
    }
    float QX=(float)x/(R->Width*0.5f);
    float QY=(float)y/(R->Height*0.5f);
    float QW=(float)width/(R->Width*0.5f);
    float QH=(float)height/(R->Height*0.5f);
    R->Indicies[R->Indicies_Count++] = R->Vertecies_Count;
    R->Indicies[R->Indicies_Count++] = R->Vertecies_Count+1;
    R->Indicies[R->Indicies_Count++] = R->Vertecies_Count+3;
    R->Indicies[R->Indicies_Count++] = R->Vertecies_Count+1;
    R->Indicies[R->Indicies_Count++] = R->Vertecies_Count+2;
    R->Indicies[R->Indicies_Count++] = R->Vertecies_Count+3;
    
    R->Vertecies_Data[R->Vertecies_Count++]=(vertex_data){QX+QW,QY+QH,0.0f,red,green,blue,alpha};
    R->Vertecies_Data[R->Vertecies_Count++]=(vertex_data){QX+QW,QY,0.0f,red,green,blue,alpha};
    R->Vertecies_Data[R->Vertecies_Count++]=(vertex_data){QX,QY,0.0f,red,green,blue,alpha};
    R->Vertecies_Data[R->Vertecies_Count++]=(vertex_data){QX,QY+QH,0.0f,red,green,blue,alpha};
}
void begin_frame(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    glClearColor(0.1f,0.1f,0.15f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    R->Vertecies_Count=0;
    R->Indicies_Count=0;
}
void end_frame(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    glUseProgram(R->ShaderProgram);
    glBindVertexArray(R->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data)*R->Vertecies_Count,R->Vertecies_Data,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,R->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*R->Indicies_Count,R->Indicies,GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES,R->Indicies_Count,GL_UNSIGNED_INT,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
void* get_framebuffer_texture(void){
    return NULL;
}
#endif