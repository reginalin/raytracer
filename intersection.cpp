#include "intersection.h"

Intersection::Intersection(glm::vec4 position, glm::vec3 normal, glm::vec2 uv, float t, Geometry *geometry){
    this->position = position;
    this->normal = normal;
    this->uv = uv;
    this->t = t;
    this->geometry = geometry;
}

