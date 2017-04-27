#include "ray.h"

Ray::Ray(glm::vec4 origin, glm::vec3 direction)
{
    this->origin = origin;
    this->direction = direction;
    this->ignoreGeo = NULL;
}

Ray::Ray(glm::vec4 origin, glm::vec3 direction, Geometry *ignoreGeo)
{
    this->origin = origin;
    this->direction = direction;
    this->ignoreGeo = ignoreGeo;
}

const Ray Ray::getTransformedCopy(glm::mat4 transform) {
    return Ray(transform * this->origin, glm::vec3(transform * glm::vec4(this->direction, 0)));
}
