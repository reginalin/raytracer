#include "ray.h"

Ray::Ray(vec4 origin, vec4 direction)
{
    this->origin = origin;
    this->direction = direction;
}

const Ray Ray::getTransformedCopy(mat4 transform) {
    return Ray(this->origin * transform, this->direction * transform);
}
