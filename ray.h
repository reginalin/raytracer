#ifndef RAY_H
#define RAY_H

#include "ray.h"
#include "la.h"
#include "geometry.h"

class Geometry;

class Ray
{
public:
    glm::vec4 origin;
    glm::vec3 direction;
    Geometry *ignoreGeo;
    Ray(glm::vec4 origin, glm::vec3 direction);
    Ray(glm::vec4 origin, glm::vec3 direction, Geometry *ignoreGeo);
    const Ray getTransformedCopy(glm::mat4 transform);
};

#endif // RAY_H
