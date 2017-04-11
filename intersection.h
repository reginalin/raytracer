#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vec4.h"
#include "tiny_obj_loader.h"

class Intersection
{
public:
    vec4 position;
    vec4 normal;
    float t;
    tinyobj::shape_t *geometry;
    Intersection(vec4 position, vec4 normal, float t, tinyobj::shape_t *geometry);
};

#endif // INTERSECTION_H
