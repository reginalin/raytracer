#ifndef RAY_H
#define RAY_H

#include "vec4.h"
#include "ray.h"
#include "mat4.h"

class Ray
{
public:
    vec4 origin;
    vec4 direction;
    Ray(vec4 origin, vec4 direction);
    const Ray getTransformedCopy(mat4 transform);
};

#endif // RAY_H
