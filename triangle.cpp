#include "triangle.h"
#include <math.h>


Triangle::Triangle(vec4 p1, vec4 p2, vec4 p3, mat4 transformMatrix) {
    P1 = p1;
    P2 = p2;
    P3 = p3;
    transform = transformMatrix;
}

Intersection Triangle::getIntersection(ray& input) {
    mat4 inverted = inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);

    vec4 dir = objRay.direction;
    vec4 orig = objRay.origin;

    vec4 E1 = P2 - P1;
    vec4 E2 = P3 - P1;
    vec4 T = orig - P1;

    vec4 tuv = (1 / cross(dir, E1)) * vec4(dot(cross(T, E1), E2), dot(cross(dir, E2), T), dot(cross(dir, E1), T), 1);

    float t = tuv[0];
    float u = tuv[1];
    float v = tuv[2];

    if (u < 0 || u > 1 || v < 0 || v > 1) {
        return NULL;
    }

    vec4 temp = objRay.origin + t * dir;
    vec4 point = temp * transform; // left or right multiply?

    vec4 normal = temp * transpose(inverted);

    return Intersection(point, normal, t, this);
}
