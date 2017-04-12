#include "intersection.h"

Intersection::Intersection(vec4 position, vec4 normal, float t, Geometry *geometry){
    this->position = position;
    this->normal = normal;
    this->t = t;
    this->geometry = geometry;
}

