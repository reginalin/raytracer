#include "ray.h"

ray::ray(glm::vec4 origin, glm::vec4 direction)
{
    this->origin = origin;
    this->direction = direction;
}

const ray ray::getTransformedCopy(glm::mat4 transform) {
    return ray(this->origin * transform, this->direction * transform);
}
