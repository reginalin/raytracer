#include "sphere.h"
#include <math.h>


Sphere::Sphere(glm::mat4 transformMatrix) {
    transform = transformMatrix;
}

Intersection Sphere::getIntersection(Ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);


    glm::vec3 dir = glm::vec3(objRay.direction[0], objRay.direction[1], objRay.direction[2]); // vec3 to vec4
    glm::vec4 orig = objRay.origin;
    
    glm::vec3 s = glm::vec3(orig - center);
    float a = glm::dot(dir, dir);
    float b = glm::dot(dir, s) * 2;
    float c = glm::dot(s, s) + (radius * radius);

    float disc = (b * b) - (4 * a * c);
    if (disc < 0) {
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), -1, this);
    }

    float t0, t1;

    t0 = (-b - (sqrtf(disc))) / (2 * a);
    if (t0 < 0) {
        t1 = (-b + (sqrtf(disc))) / (2 * a);
        t0 = t1;
        // what if both are negative
    }

    glm::vec4 temp = orig + glm::vec4(t0 * dir, 0);
    glm::vec4 point = transform * temp;
    //vec3?

    glm::vec3 normal = glm::vec3(glm::transpose(inverted) * temp);

    return Intersection(point, normal, t0, this);
}

