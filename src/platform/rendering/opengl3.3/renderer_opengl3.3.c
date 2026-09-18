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

    R->Vertices_Cap=Quad_Cap*4;
    R->Vertices_Data=malloc(sizeof(vertex_data)*R->Vertices_Cap);
    GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl 3.3 renderer vertex array");
    R->Vertices_Count=0;

    R->Indices_Cap=Quad_Cap*6;
    R->Indices=malloc(sizeof(int)*R->Indices_Cap);
    GAVEN_ASSERT(R->Indices,"Couldnt allocate memory for opengl 3.3 renderer index array");
    R->Indices_Count=0;

    glm_ortho(0.0f,R->Width,0.0f,R->Height,-0.1f,100.0f,R->Uniform_Projection);
    glm_mat4_identity(R->Uniform_View);

    GLenum Result =glewInit();
    GAVEN_ASSERT(!Result,"GLEW initialization failed: %s",(const char*)glewGetErrorString(Result));
    
    glGenVertexArrays(1,&R->VAO);    
    glGenBuffers(1,&R->VBO);
    glGenBuffers(1,&R->EBO);

    glBindVertexArray(R->VAO);   
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data)*R->Vertices_Count,R->Vertices_Data,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,R->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*R->Indices_Count,R->Indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_data),NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(vertex_data),(void*)offsetof(vertex_data,r));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(vertex_data),(void*)offsetof(vertex_data,u));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    const char *vs_src=  "#version 330 core\n"
                                    "layout(location = 0) in vec3 aPos;\n"
                                    "layout(location = 1) in vec4 aColor;\n"
                                    "layout(location = 2) in vec2 aUV;\n"
                                    "out vec4 v_Color;\n"
                                    "out vec2 v_UV;\n"
                                    "uniform mat4 projection;\n"
                                    "uniform mat4 view;\n"
                                    "void main(){\n"
                                    "   gl_Position=projection*view*vec4(aPos,1.0);\n"
                                    "   v_Color=aColor;\n"
                                    "   v_UV=aUV;\n"
                                    "}";
    const char *fs_src=  "#version 330 core\n"
                                    "layout(location = 0) out vec4 color;\n"
                                    "in vec4 v_Color;\n"
                                    "in vec2 v_UV;\n"
                                    "void main(){\n"
                                    "   color=vec4(v_UV,0.0,1.0);\n"
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
    R->Uniform_Projection_Location=glGetUniformLocation(R->ShaderProgram,"projection");
    R->Uniform_View_Location=glGetUniformLocation(R->ShaderProgram,"view");

    glEnable(GL_DEPTH_TEST);
    return R;
}
void destroy_renderer(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    free(R->Indices);
    free(R->Vertices_Data);
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
    if(R->Indices_Count==R->Indices_Cap-5){
        R->Indices_Cap*=2;
        R->Indices=realloc(R->Indices,sizeof(int)*R->Indices_Cap);
        GAVEN_ASSERT(R->Indices,"Couldnt allocate memory for opengl 3.3 renderer index array");
    }
    if(R->Vertices_Count==R->Vertices_Cap-3){
        R->Vertices_Cap*=2;
        R->Vertices_Data=realloc(R->Vertices_Data,sizeof(vertex_data)*R->Vertices_Cap);
        GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl 3.3 renderer index array");
    }

    R->Indices[R->Indices_Count++] = R->Vertices_Count;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+1;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+3;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+1;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+2;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+3;
    
    R->Vertices_Data[R->Vertices_Count++]=(vertex_data){x+width,y+height,0.0f,red,green,blue,alpha,1.0f,1.0f};
    R->Vertices_Data[R->Vertices_Count++]=(vertex_data){x+width,y,0.0f,red,green,blue,alpha,1.0f,0.0f};
    R->Vertices_Data[R->Vertices_Count++]=(vertex_data){x,y,0.0f,red,green,blue,alpha,0.0f,0.0f};
    R->Vertices_Data[R->Vertices_Count++]=(vertex_data){x,y+height,0.0f,red,green,blue,alpha,0.0f,1.0f};
}
void begin_frame(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    glClearColor(0.1f,0.1f,0.15f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    R->Vertices_Count=0;
    R->Indices_Count=0;
}
void end_frame(void* renderer){
    renderer_api* R=(renderer_api*)renderer;
    glUseProgram(R->ShaderProgram);
    glUniformMatrix4fv(R->Uniform_Projection_Location,1,GL_FALSE,(float*)R->Uniform_Projection);
    glUniformMatrix4fv(R->Uniform_View_Location,1,GL_FALSE,(float*)R->Uniform_View);
    glBindVertexArray(R->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data)*R->Vertices_Count,R->Vertices_Data,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,R->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*R->Indices_Count,R->Indices,GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES,R->Indices_Count,GL_UNSIGNED_INT,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
void* get_framebuffer_texture(void){
    return NULL;
}
#endif