#ifdef DMAGH_RENDERER_OPENGLES3_2
#include "curve_renderer_opengles3.2.h"
#include <stdlib.h>
#include <gaven.h>
#include <string.h>
curve_renderer* create_curve_renderer(renderer_api* renderer){
    curve_renderer* R=(curve_renderer*)malloc(sizeof(curve_renderer));
    R->Renderer_api=renderer;
    R->Vertices_Cap=128;
    R->Vertices_Data=malloc(sizeof(curve_point)*R->Vertices_Cap);
    GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl es 3.2 renderer vertex array");
    R->Vertices_Count=0;
    glGenVertexArrays(1,&R->VAO);    
    glGenBuffers(1,&R->VBO);

    glBindVertexArray(R->VAO);   
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(curve_point)*R->Vertices_Count,R->Vertices_Data,GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)offsetof(curve_point,Position));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)offsetof(curve_point,HandleOut));
    glVertexAttribIPointer(2,1,GL_INT,sizeof(curve_point),(void*)offsetof(curve_point,Size));
    glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)(offsetof(curve_point,Color)));
    glVertexAttribPointer(4,2,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)(sizeof(curve_point)+offsetof(curve_point,HandleIn)));
    glVertexAttribPointer(5,2,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)(sizeof(curve_point)+offsetof(curve_point,Position)));
    glVertexAttribIPointer(6,1,GL_INT,sizeof(curve_point),(void*)(sizeof(curve_point)+offsetof(curve_point,Size)));
    glVertexAttribPointer(7,4,GL_FLOAT,GL_FALSE,sizeof(curve_point),(void*)(sizeof(curve_point)+offsetof(curve_point,Color)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnableVertexAttribArray(7);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    const char *vs_src=  "#version 320 es\n"
                                    "layout(location = 0) in vec2 aPos1;\n"
                                    "layout(location = 1) in vec2 aHandOut;\n"
                                    "layout(location = 2) in int aSize1;\n"
                                    "layout(location = 3) in vec4 aColor1;\n"
                                    "layout(location = 4) in vec2 aHandIn;\n"
                                    "layout(location = 5) in vec2 aPos2;\n"
                                    "layout(location = 6) in int aSize2;\n"
                                    "layout(location = 7) in vec4 aColor2;\n"
                                    "flat out vec2 vPos1;\n"
                                    "flat out vec2 vHandOut;\n"
                                    "flat out int vSize1;\n"
                                    "flat out vec4 vColor1;\n"
                                    "flat out vec2 vHandIn;\n"
                                    "flat out vec2 vPos2;\n"
                                    "flat out int vSize2;\n"
                                    "flat out vec4 vColor2;\n"
                                    "void main(){\n"
                                        "vPos1=aPos1;\n"
                                        "vHandOut=aHandOut;\n"
                                        "vHandIn=aHandIn;\n"
                                        "vPos2=aPos2;\n"
                                        "vSize1=aSize1;\n"
                                        "vSize2=aSize2;\n"
                                        "vColor1=aColor1;\n"
                                        "vColor2=aColor2;\n"
                                        "gl_Position=vec4(0.0);\n"
                                    "}";
    const char *fs_src= "#version 320 es\n"
                                    "precision mediump float;"
                                    "out vec4 color;\n"
                                    "in vec4 gColor;\n"
                                    "void main(){\n"
                                        "color=gColor;\n"
                                    "}";
    const char *gs_src=  "#version 320 es\n"
                                    "layout(points) in;\n"
                                    "layout(triangle_strip, max_vertices = 130) out;\n"
                                    "flat in vec2 vPos1[];\n"
                                    "flat in int vSize1[];\n"
                                    "flat in vec4 vColor1[];\n"
                                    "flat in vec2 vHandOut[];\n"
                                    "flat in vec2 vHandIn[];\n"
                                    "flat in vec2 vPos2[];\n"
                                    "flat in int vSize2[];\n"
                                    "flat in vec4 vColor2[];\n"
                                    "out vec4 gColor;\n"
                                    "uniform mat4 projection;\n"
                                    "uniform mat4 view;\n"
                                    "void main(){\n"
                                        "int i=0;\n"
                                        "vec2 Pos=vPos1[0];\n"
                                        "float t=0.0,mt;\n"
                                        "if (vHandOut[0]==vec2(0.0,0.0))\n"
                                            "return;\n"
                                        "while(i<=64){\n"
                                            "t=float(i)/64.0;\n"
                                            "mt=1.0-t;\n"
                                            "vec2 NewPos=mt*mt*mt*vPos1[0]+3.0*mt*mt*t*vHandOut[0]+3.0*mt*t*t*vHandIn[0]+t*t*t*vPos2[0];\n"
                                            "vec2 Tangent=NewPos-Pos;"
                                            "Pos=NewPos\n;"
                                            "Tangent =normalize(Tangent);\n"
                                            "vec2 Normal=vec2(Tangent.y,-Tangent.x);\n"
                                            "float Size=mix(float(vSize1[0]),float(vSize2[0]),t);\n"
                                            "vec4 Color=mix(vColor1[0],vColor2[0],t);\n"
                                            "vec2 Left= Pos+Normal*Size;\n"
                                            "vec2 Right= Pos-Normal*Size;\n"
                                            "gColor=Color;\n"
                                            "gl_Position=projection*view*vec4(Left,-1.0,1.0);\n"
                                            "EmitVertex();\n"
                                            "gColor=Color;\n"
                                            "gl_Position=projection*view*vec4(Right,-1.0,1.0);\n"
                                            "EmitVertex();\n"
                                            "i++;\n"
                                        "}\n"
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
        GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl es 3.2 curve renderer index array");
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