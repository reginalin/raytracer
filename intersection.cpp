#include "intersection.h"

Intersection::Intersection(vec4 position, vec4 normal, float t, tinyobj::shape_t *geometry){
    this->position = position;
    this->normal = normal;
    this->t = t;
    this->geometry = geometry;
}

