#ifndef RAY_H
#define RAY_H

#include "ray.h"
#include "la.h"

class Ray
{
public:
    glm::vec4 origin;
    glm::vec3 direction;
    Ray(glm::vec4 origin, glm::vec3 direction);
    const Ray getTransformedCopy(glm::mat4 transform);
};

#endif // RAY_H
