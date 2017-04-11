#include "ray.h"

ray::ray(vec4 origin, vec4 direction)
{
    this->origin = origin;
    this->direction = direction;
}

const ray ray::getTransformedCopy(mat4 transform) {
    return ray(this->origin * transform, this->direction * transform);
}
