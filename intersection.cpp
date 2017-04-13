#include "intersection.h"

Intersection::Intersection(glm::vec4 position, glm::vec4 normal, float t, Geometry *geometry){
    this->position = position;
    this->normal = normal;
    this->t = t;
    this->geometry = geometry;
}

