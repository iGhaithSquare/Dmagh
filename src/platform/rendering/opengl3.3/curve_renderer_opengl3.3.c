#ifdef DMAGH_RENDERER_OPENGL3_3
#include "curve_renderer_opengl3.3.h"
#include <stdlib.h>
#include <gaven.h>

curve_renderer* create_curve_renderer(renderer_api* renderer,curve_array* Curve_Array){
    curve_renderer* R=(curve_renderer*)malloc(sizeof(curve_renderer));
    R->Renderer_api=renderer;
    R->Curve_Array=Curve_Array;
    R->Curve_Array_Version=Curve_Array->Version;
    R->Vertices_Cap=Curve_Array->Curve_Count?Curve_Array->Curve_Count:16;
    R->Vertices_Data=malloc(sizeof(curve)*R->Vertices_Cap);
    GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl 3.3 renderer vertex array");
    R->Vertices_Count=0;
    glGenVertexArrays(1,&R->VAO);    
    glGenBuffers(1,&R->VBO);
    glGenBuffers(1,&R->Point_Buffer);
    glGenTextures(1,&R->Point_Texture);

    glBindVertexArray(R->VAO);   
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(curve)*R->Vertices_Count,R->Vertices_Data,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_TEXTURE_BUFFER,R->Point_Buffer);
    glBufferData(GL_TEXTURE_BUFFER,sizeof(curve_point)*Curve_Array->Point_Count,Curve_Array->Points,GL_DYNAMIC_DRAW);
    glBindTexture(GL_TEXTURE_BUFFER,R->Point_Texture);
    glTexBuffer(GL_TEXTURE_BUFFER,GL_RG32F,R->Point_Buffer);


    glVertexAttribIPointer(0,2,GL_UNSIGNED_INT,sizeof(curve),NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindBuffer(GL_TEXTURE_BUFFER,0);
    glBindTexture(GL_TEXTURE_BUFFER,0);

    const char *vs_src=  "#version 330 core\n"
                                    "layout(location = 0) in uvec2 aCurve;\n"
                                    "flat out uvec2 vCurve;"
                                    "void main(){\n"
                                        "vCurve=aCurve;\n"
                                        "gl_Position=vec4(0.0);"
                                    "}";
    const char *fs_src= "#version 330 core\n"
                                    "out vec4 color;\n"
                                    "void main(){\n"
                                        "color=vec4(1.0,0.0,0.0,1.0);\n"
                                    "}";
    const char *gs_src=  "#version 330 core\n"
                                    "layout(points) in;\n"
                                    "layout(line_strip, max_vertices = 256) out;\n"
                                    "flat in uvec2 vCurve[];\n"
                                    "uniform mat4 projection;\n"
                                    "uniform mat4 view;\n"
                                    "uniform samplerBuffer Points;\n"
                                    "void main(){\n"
                                        "uint point_offset = vCurve[0].x;\n"
                                        "uint point_count = vCurve[0].y;\n"
                                        "for(uint i=0u;i<point_count;i++){\n"
                                            "vec2 position=texelFetch(Points,int((point_offset+i)*3u)).rg;\n"
                                            "gl_Position=projection*view*vec4(position,0.0,1.0);"
                                            "EmitVertex();"
                                        "}\n"
                                        "EndPrimitive();\n"
                                    "}";
    R->Shader = create_shader(vs_src,fs_src,gs_src);
    use_shader(R->Shader);
    glUniform1i(glGetUniformLocation(R->Shader->Program,"Points"),0);
    R->Uniform_Projection_Location=glGetUniformLocation(R->Shader->Program,"projection");
    R->Uniform_View_Location=glGetUniformLocation(R->Shader->Program,"view");

    return R;
}
void destroy_curve_renderer(curve_renderer* renderer){
    free(renderer->Vertices_Data);
    destroy_shader(renderer->Shader);
    glDeleteBuffers(1,&renderer->VBO);
    glDeleteVertexArrays(1,&renderer->VAO);
    glDeleteBuffers(1,&renderer->Point_Buffer);
    glDeleteTextures(1,&renderer->Point_Texture);
    free(renderer);
}
void draw_curve(void* renderer,curve Curve){
    renderer_api* main=(renderer_api*)renderer;
    curve_renderer *R=main->Curve_Renderer;
    if(R->Vertices_Count==R->Vertices_Cap){
        R->Vertices_Cap*=2;
        R->Vertices_Data=realloc(R->Vertices_Data,sizeof(curve)*R->Vertices_Cap);
        GAVEN_ASSERT(R->Vertices_Data,"Couldnt allocate memory for opengl 3.3 curve renderer index array");
    }
    R->Vertices_Data[R->Vertices_Count++]=Curve;
    
    curve_array* Array=R->Curve_Array;
    if(R->Curve_Array_Version!=Array->Version){
        glBindBuffer(GL_TEXTURE_BUFFER,R->Point_Buffer);
        glBufferData(GL_TEXTURE_BUFFER,sizeof(curve_point)*Array->Point_Count,Array->Points,GL_DYNAMIC_DRAW);
        glBindBuffer(GL_TEXTURE_BUFFER,0);
        R->Curve_Array_Version=Array->Version;
    }
}
void begin_frame_curve_renderer(curve_renderer* R){
    R->Vertices_Count=0;
}
void end_frame_curve_renderer(curve_renderer* R){
    use_shader(R->Shader);
    glUniformMatrix4fv(R->Uniform_Projection_Location,1,GL_FALSE,(float*)R->Renderer_api->Uniform_Projection);
    glUniformMatrix4fv(R->Uniform_View_Location,1,GL_FALSE,(float*)R->Renderer_api->Uniform_View);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER,R->Point_Texture);
    glBindVertexArray(R->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,R->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(curve)*R->Vertices_Count,R->Vertices_Data,GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS,0,R->Vertices_Count);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}
#endif