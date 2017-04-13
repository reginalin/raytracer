#include "ray.h"

Ray::Ray(glm::vec4 origin, glm::vec4 direction)
{
    this->origin = origin;
    this->direction = direction;
}

const Ray Ray::getTransformedCopy(glm::mat4 transform) {
    return Ray(this->origin * transform, this->direction * transform);
}
