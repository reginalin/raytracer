#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "tiny_obj_loader.h"
#include "la.h"

class Geometry;

class Intersection
{
public:
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 uv;
    float t;
    Geometry *geometry;
    Intersection(glm::vec4 position, glm::vec3 normal, glm::vec2 uv, float t, Geometry *geometry);
};

#endif // INTERSECTION_H
