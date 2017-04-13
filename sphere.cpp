#include "sphere.h"
#include <math.h>


Sphere::Sphere(glm::mat4 transformMatrix) {
    transform = transformMatrix;
}

<<<<<<< HEAD
Intersection Sphere::getIntersection(Ray& input) {
    mat4 inverted = inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);
=======
Intersection Sphere::getIntersection(ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);
>>>>>>> 5c8438b443bc29b5c1c696407a547bbcd97081c7

    glm::vec4 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;
    
    glm::vec4 s = orig - center;
    float a = glm::dot(dir, dir);
    float b = glm::dot(dir, s) * 2;
    float c = glm::dot(s, s) + (radius * radius);

    float disc = (b * b) - (4 * a * c);
    if (disc < 0) {
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), -1, this);
    }

    float t0, t1;

    t0 = (-b - (sqrtf(disc))) / (2 * a);
    if (t0 < 0) {
        t1 = (-b + (sqrtf(disc))) / (2 * a);
        t0 = t1;
        // what if both are negative
    }

    glm::vec4 temp = orig + t0 * dir;
    glm::vec4 point = temp * transform; // left or right multiply?
    //vec3?

    glm::vec4 normal = temp * glm::transpose(inverted);

    return Intersection(point, normal, t0, this);
}

