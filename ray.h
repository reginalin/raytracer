#ifndef RAY_H
#define RAY_H

#include "ray.h"
#include "la.h"

class ray
{
public:
    glm::vec4 origin;
    glm::vec4 direction;
    ray(glm::vec4 origin, glm::vec4 direction);
    const ray getTransformedCopy(glm::mat4 transform);
};

#endif // RAY_H
