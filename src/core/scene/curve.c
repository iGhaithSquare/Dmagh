#include "curve.h"
#include "gaven.h"
#include <string.h>
curve_array* create_curve_array(){
    curve_array* Array=(curve_array*)malloc(sizeof(curve_array));
    GAVEN_ASSERT(Array,"Couldnt allocate memory for curve array");
    Array->Point_Cap=128;
    Array->Points=(curve_point*)malloc(sizeof(curve_point)*Array->Point_Cap);
    GAVEN_ASSERT(Array->Points,"Couldnt allocate memory for curve array");
    Array->Point_Count=0;
    Array->Curve_Count=0;
    Array->Creating_Curve_Flag=0;
    return Array;
}
curve begin_curve(curve_array* Array,curve_point Start){
    curve Curve;
    if(Array->Creating_Curve_Flag){
        GAVEN_WARN("A Curve is already being created");
        Curve=Array->Current_Curve;
    }
    else{
        Curve.point_offset=Array->Point_Count;
        Curve.point_count=0;

        Array->Current_Curve=Curve;
        Array->Creating_Curve_Flag=1;
    }
    curve_point P=Start;
    glm_vec2_copy((vec2){0.0f,0.0f},P.HandleIn);
    add_point(Array,P);
    return Curve;
}
void add_point(curve_array* Array,curve_point Point){
    if(Array->Point_Count==Array->Point_Cap){
        Array->Point_Cap*=2;
        Array->Points=realloc(Array->Points,sizeof(curve_point)*Array->Point_Cap);
        GAVEN_ASSERT(Array->Points,"Couldnt allocate memory for curve array");
    }
    Array->Points[Array->Point_Count++]=Point;
}
void end_curve(curve_array* Array,curve* Curve,curve_point End){
    if(!Array->Creating_Curve_Flag){
        GAVEN_WARN("No Curve is being created");
        return;
    }
    curve_point P=End;
    glm_vec2_copy((vec2){0.0f,0.0f},P.HandleOut);
    add_point(Array,P);

    Curve->point_count=Array->Point_Count-Curve->point_offset;
    Array->Current_Curve=*Curve;
    Array->Creating_Curve_Flag=0;
    Array->Curve_Count++;
}
void destroy_curve_array(curve_array* Array){
    free(Array->Points);
    free(Array);
}
void remove_curve_from_curve_array(curve_array* Array, curve Curve){
    memmove(Array->Points+Curve.point_offset,Array->Points+Curve.point_offset+Curve.point_count,(Array->Point_Count-Curve.point_count-Curve.point_offset)*sizeof(curve_point));
    Array->Point_Count-=Curve.point_count;
    Array->Curve_Count--;
}