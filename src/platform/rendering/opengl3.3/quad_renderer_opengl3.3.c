#ifdef DMAGH_RENDERER_OPENGL3_3
#include "quad_renderer_opengl3.3.h"
#include <stdlib.h>
#include <gaven.h>
quad_renderer* create_quad_renderer(renderer_api* renderer,int Quad_Cap){
    quad_renderer* R=(quad_renderer*)malloc(sizeof(quad_renderer));
    R->Renderer_api=renderer;
    R->Vertices_Cap=Quad_Cap*4;
    R->Vertices_Data=malloc(sizeof(quad_vertex_data)*R->Vertices_Cap);
    GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl 3.3 renderer vertex array");
    R->Vertices_Count=0;

    R->Indices_Cap=Quad_Cap*6;
    R->Indices=malloc(sizeof(int)*R->Indices_Cap);
    GAVEN_ASSERT(R->Indices,"Couldnt allocate memory for opengl 3.3 renderer index array");
    R->Indices_Count=0;

    
    glGenVertexArrays(1,&R->VAO);    
    glGenBuffers(1,&R->VBO);
    glGenBuffers(1,&R->EBO);

    glBindVertexArray(R->VAO);   
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quad_vertex_data)*R->Vertices_Count,R->Vertices_Data,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,R->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*R->Indices_Count,R->Indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(quad_vertex_data),NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(quad_vertex_data),(void*)offsetof(quad_vertex_data,r));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(quad_vertex_data),(void*)offsetof(quad_vertex_data,u));
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
    R->Shader = create_shader(vs_src,fs_src,NULL);
    R->Uniform_Projection_Location=glGetUniformLocation(R->Shader->Program,"projection");
    R->Uniform_View_Location=glGetUniformLocation(R->Shader->Program,"view");
    return R;
}
void destroy_quad_renderer(quad_renderer* renderer){
    free(renderer->Indices);
    free(renderer->Vertices_Data);
    destroy_shader(renderer->Shader);
    glDeleteBuffers(1,&renderer->VBO);
    glDeleteBuffers(1,&renderer->EBO);
    glDeleteVertexArrays(1,&renderer->VAO);
    free(renderer);
}
void draw_quad(void* renderer, int x,int y, int width, int height,float red,float green,float blue,float alpha){
    renderer_api* main=(renderer_api*)renderer;
    quad_renderer *R=main->Quad_Renderer;
    if(R->Indices_Count==R->Indices_Cap-5){
        R->Indices_Cap*=2;
        R->Indices=realloc(R->Indices,sizeof(int)*R->Indices_Cap);
        GAVEN_ASSERT(R->Indices,"Couldnt allocate memory for opengl 3.3 quad renderer index array");
    }
    if(R->Vertices_Count==R->Vertices_Cap-3){
        R->Vertices_Cap*=2;
        R->Vertices_Data=realloc(R->Vertices_Data,sizeof(quad_vertex_data)*R->Vertices_Cap);
        GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl 3.3 quad renderer index array");
    }

    R->Indices[R->Indices_Count++] = R->Vertices_Count;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+1;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+3;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+1;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+2;
    R->Indices[R->Indices_Count++] = R->Vertices_Count+3;
    
    R->Vertices_Data[R->Vertices_Count++]=(quad_vertex_data){x+width,y+height,-1.0f,red,green,blue,alpha,1.0f,1.0f};
    R->Vertices_Data[R->Vertices_Count++]=(quad_vertex_data){x+width,y,-1.0f,red,green,blue,alpha,1.0f,0.0f};
    R->Vertices_Data[R->Vertices_Count++]=(quad_vertex_data){x,y,-1.0f,red,green,blue,alpha,0.0f,0.0f};
    R->Vertices_Data[R->Vertices_Count++]=(quad_vertex_data){x,y+height,-1.0f,red,green,blue,alpha,0.0f,1.0f};
}
void begin_frame_quad_renderer(quad_renderer* R){
    R->Vertices_Count=0;
    R->Indices_Count=0;
}
void end_frame_quad_renderer(quad_renderer* R){
    use_shader(R->Shader);
    glUniformMatrix4fv(R->Uniform_Projection_Location,1,GL_FALSE,(float*)R->Renderer_api->Uniform_Projection);
    glUniformMatrix4fv(R->Uniform_View_Location,1,GL_FALSE,(float*)R->Renderer_api->Uniform_View);
    glBindVertexArray(R->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quad_vertex_data)*R->Vertices_Count,R->Vertices_Data,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,R->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*R->Indices_Count,R->Indices,GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES,R->Indices_Count,GL_UNSIGNED_INT,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
#endif