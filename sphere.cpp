#include "sphere.h"
#include <math.h>


Sphere::Sphere(mat4 transformMatrix) {
    transform = transformMatrix;
}

Intersection Sphere::getIntersection(ray& input) {
    mat4 inverted = inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);

    vec4 dir = objRay.direction;
    vec4 s = objRay.origin - center;
    float a = dot(dir, dir);
    float b = dot(dir, s) * 2;
    float c = dot(s, s) + (center * center);

    float disc = (b * b) - (4 * a * c);
    if (disc < 0) {
        return NULL;
    }

    float t0, t1;

    t0 = (-b - (sqrtf(disc))) / (2 * a);
    if (t0 < 0) {
        t1 = (-b + (sqrtf(disc))) / (2 * a);
        t0 = t1;
        // what if both are negative
    }

    vec4 temp = origin + t0 * dir;
    vec4 point = temp * transform; // left or right multiply?
    //vec3?

    vec4 normal = temp * inverted.transpose();

    return Intersection(point, normal, t0, this);
}

