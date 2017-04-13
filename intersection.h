#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "tiny_obj_loader.h"

class Geometry;

class Intersection
{
public:
    glm::vec4 position;
    glm::vec4 normal;
    float t;
    Geometry *geometry;
    Intersection(glm::vec4 position, glm::vec4 normal, float t, Geometry *geometry);
};

#endif // INTERSECTION_H
