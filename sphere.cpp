#include "sphere.h"
#include <math.h>


Sphere::Sphere(glm::mat4 transformMatrix) {
    transform = transformMatrix;
}

Intersection Sphere::getIntersection(const ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);

    glm::vec4 dir = objRay.direction;
    glm::vec4 s = objRay.origin - center;
    float a = glm::dot(dir, dir);
    float b = glm::dot(dir, s) * 2;
    float c = glm::dot(s, s) + (center * center);

    float disc = (b * b) - (4 * a * c);
//    if (disc < 0) {} // no intersection
    float t0, t1;

    t0 = (-b - (sqrtf(disc))) / (2 * a);
    if (t0 < 0) {
        t1 = (-b + (sqrtf(disc))) / (2 * a);
        t0 = t1;
        // what if both are negative
    }

    vec4 temp = rayObj.origin + t0 * dir;
    vec4 pointTemp = temp * transform; // left or right multiply?
    vec3 point = vec3(pointTemp);

    vec4 normTemp = temp * glm::transpose(inverted);
    vec3 normal = vec3(normTemp);

    return Intersection(point, normal, t0, this);
}

