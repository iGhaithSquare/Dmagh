#ifdef DMAGH_RENDERER_OPENGL3_3
#include "curve_renderer_opengl3.3.h"
#include <stdlib.h>
#include <gaven.h>
#include <string.h>
curve_renderer* create_curve_renderer(renderer_api* renderer){
    curve_renderer* R=(curve_renderer*)malloc(sizeof(curve_renderer));
    R->Renderer_api=renderer;
    R->Vertices_Cap=128;
    R->Vertices_Data=malloc(sizeof(curve_point)*R->Vertices_Cap);
    GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl 3.3 renderer vertex array");
    R->Vertices_Count=0;
    glGenVertexArrays(1,&R->VAO);    
    glGenBuffers(1,&R->VBO);

    glBindVertexArray(R->VAO);   
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(curve_point)*R->Vertices_Count,R->Vertices_Data,GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)offsetof(curve_point,Position));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)offsetof(curve_point,HandleOut));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)(sizeof(curve_point)+offsetof(curve_point,HandleIn)));
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)(sizeof(curve_point)+offsetof(curve_point,Position)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    const char *vs_src=  "#version 330 core\n"
                                    "layout(location = 0) in vec2 aPos1;\n"
                                    "layout(location = 1) in vec2 aHandOut;\n"
                                    "layout(location = 2) in vec2 aHandIn;\n"
                                    "layout(location = 3) in vec2 aPos2;\n"
                                    "flat out vec2 vPos1;\n"
                                    "flat out vec2 vHandOut;\n"
                                    "flat out vec2 vHandIn;\n"
                                    "flat out vec2 vPos2;\n"
                                    "void main(){\n"
                                        "vPos1=aPos1;\n"
                                        "vHandOut=aHandOut;\n"
                                        "vHandIn=aHandIn;\n"
                                        "vPos2=aPos2;\n"
                                        "gl_Position=vec4(0.0);\n"
                                    "}";
    const char *fs_src= "#version 330 core\n"
                                    "out vec4 color;\n"
                                    "void main(){\n"
                                        "color=vec4(1.0,0.0,0.0,1.0);\n"
                                    "}";
    const char *gs_src=  "#version 330 core\n"
                                    "layout(points) in;\n"
                                    "layout(line_strip, max_vertices = 256) out;\n"
                                    "flat in vec2 vPos1[];\n"
                                    "flat in vec2 vHandOut[];\n"
                                    "flat in vec2 vHandIn[];\n"
                                    "flat in vec2 vPos2[];\n"
                                    "uniform mat4 projection;\n"
                                    "uniform mat4 view;\n"
                                    "void main(){\n"
                                        "if (vHandOut[0]==vec2(0,0))\n"
                                            "return;\n"
                                        "gl_Position=projection*view*vec4(vPos1[0],0.0,1.0);\n"
                                        "EmitVertex();\n"
                                        "gl_Position=projection*view*vec4(vPos2[0],0.0,1.0);\n"
                                        "EmitVertex();\n"
                                        "EndPrimitive();\n"
                                    "}";
    R->Shader = create_shader(vs_src,fs_src,gs_src);
    use_shader(R->Shader);
    R->Uniform_Projection_Location=glGetUniformLocation(R->Shader->Program,"projection");
    R->Uniform_View_Location=glGetUniformLocation(R->Shader->Program,"view");

    return R;
}
void destroy_curve_renderer(curve_renderer* renderer){
    free(renderer->Vertices_Data);
    destroy_shader(renderer->Shader);
    glDeleteBuffers(1,&renderer->VBO);
    glDeleteVertexArrays(1,&renderer->VAO);
    free(renderer);
}
void draw_curve_array(void* renderer,curve_array *Array){
    renderer_api* main=(renderer_api*)renderer;
    curve_renderer *R=main->Curve_Renderer;
    if(R->Vertices_Count+Array->Point_Count>=R->Vertices_Cap){
        do{
            R->Vertices_Cap*=2;
        } while(R->Vertices_Count+Array->Point_Count>=R->Vertices_Cap);
        R->Vertices_Data=realloc(R->Vertices_Data,sizeof(curve_point)*R->Vertices_Cap);
        GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl 3.3 curve renderer index array");
    }
    memcpy(R->Vertices_Data+R->Vertices_Count,Array->Points,sizeof(curve_point)*Array->Point_Count);
    R->Vertices_Count+=Array->Point_Count;
}
void begin_frame_curve_renderer(curve_renderer* R){
    R->Vertices_Count=0;
}
void end_frame_curve_renderer(curve_renderer* R){
    use_shader(R->Shader);
    glUniformMatrix4fv(R->Uniform_Projection_Location,1,GL_FALSE,(float*)R->Renderer_api->Uniform_Projection);
    glUniformMatrix4fv(R->Uniform_View_Location,1,GL_FALSE,(float*)R->Renderer_api->Uniform_View);
    glBindVertexArray(R->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(curve_point)*R->Vertices_Count,R->Vertices_Data,GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS,0,R->Vertices_Count-1);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}
#endif