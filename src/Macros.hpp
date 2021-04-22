#pragma once


#define VERTS 1000
#define TRIANGLES 4000
#define POLYGONS 5

#define VEC3_ARG(x) const float x ## _0, const float x ## _1, const float x ## _2
#define VEC3_ARG_CTONS(x) Vec3( x ## _0, x ## _1, x ## _2)
#define VEC3_SPREAD(x) x[0],x[1],x[2]