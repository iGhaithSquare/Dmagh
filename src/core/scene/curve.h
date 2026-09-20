#ifndef CURVE_H
#define CURVE_H
#include "cglm/cglm.h"
typedef struct {
    vec2 HandleIn;
    vec2 Position;
    int Size;
    vec4 Color;
    vec2 HandleOut;
} curve_point;
typedef struct {
    unsigned int point_offset;
    unsigned int point_count;
} curve;
typedef struct{
    curve_point* Points;
    uint32_t Point_Count;
    uint32_t Point_Cap;
    uint32_t Curve_Count;
    uint8_t Creating_Curve_Flag;
    curve Current_Curve;
} curve_array;
curve_array* create_curve_array();
curve begin_curve(curve_array* Array,curve_point Start);
void add_point(curve_array* Array,curve_point Point);
void end_curve(curve_array* Array,curve* Curve,curve_point End);
void destroy_curve_array(curve_array* Array);
void remove_curve_from_curve_array(curve_array* Array, curve Curve);
#endif