#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vec4.h"
#include "tiny_obj_loader.h"
#include "geometry.h"

class Intersection
{
public:
    vec4 position;
    vec4 normal;
    float t;
    Geometry *geometry;
    Intersection(vec4 position, vec4 normal, float t, Geometry *geometry);
};

#endif // INTERSECTION_H
