#ifndef RAY_H
#define RAY_H

#include "vec4.h"

class ray
{
public:
    vec4 origin;
    vec4 direction;
    ray(vec4 origin, vec4 direction);
    const ray getTransformedCopy(mat4 transform);
};

#endif // RAY_H
