#include "squareplane.h"
#include <math.h>


SquarePlane::SquarePlane(mat4 transformMatrix) {
    transform = transformMatrix;
}

Intersection SquarePlane::getIntersection(ray& input) {
    mat4 inverted = inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);

    vec4 planePoint = vec4(0, 0, 0, 0);
    vec4 planeNormal = vec4(0, 0, 1, 1);
    vec4 dir = objRay.direction;
    vec4 orig = objRay.origin;

    if (orig[0] < -0.5 || orig[0] > 0.5 || orig[1] < -0.5 || orig[1] > 0.5) {
        return NULL;
    }

    float t = dot(planeNormal, (planePoint - orig)) / dot(planeNormal, dir);

<<<<<<< HEAD
    vec4 temp = origin + t * dir;
=======
    vec4 temp = objRay.origin + t * dir;
>>>>>>> dcb5767cf9defc94b1ed309f56fbb7a4fb7620ac
    vec4 point = temp * transform; // left or right multiply?
//    vec3 point = vec3(pointTemp);

    vec4 normal = temp * transpose(inverted);
//    vec3 normal = vec3(normTemp);

    return Intersection(point, normal, t, this);
}
