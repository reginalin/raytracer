#include "triangle.h"
#include <math.h>


Triangle::Triangle(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::mat4 transformMatrix) {
    P1 = p1;
    P2 = p2;
    P3 = p3;
    transform = transformMatrix;
}

<<<<<<< HEAD
glm::vec4 crossVec4(glm::vec4 _v1, glm::vec4 _v2){
    glm::vec3 vec1 = glm::vec3(_v1[0], _v1[1], _v1[2]);
    glm::vec3 vec2 = glm::vec3(_v2[0], _v2[1], _v2[2]);
    glm::vec3 res = glm::cross(vec1, vec2);
    return glm::vec4(res[0], res[1], res[2], 1);
}

=======
<<<<<<< HEAD
Intersection Triangle::getIntersection(Ray& input) {
    mat4 inverted = inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);
=======
>>>>>>> 11db3696eae68f0f97c9cf61e6d6ee6d9957e929
Intersection Triangle::getIntersection(ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);
>>>>>>> 5c8438b443bc29b5c1c696407a547bbcd97081c7

    glm::vec4 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;

    glm::vec4 E1 = P2 - P1;
    glm::vec4 E2 = P3 - P1;
    glm::vec4 T = orig - P1;

    glm::vec4 tuv = (1 / glm::dot(crossVec4(dir, E1), E2)) * glm::vec4(glm::dot(crossVec4(T, E1), E2), glm::dot(crossVec4(dir, E2), T), glm::dot(crossVec4(dir, E1), T), 1.0);

    float t = tuv[0];
    float u = tuv[1];
    float v = tuv[2];

    if (u < 0 || u > 1 || v < 0 || v > 1) {
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), -1, this);
    }

    glm::vec4 temp = orig + t * dir;
    glm::vec4 point = temp * transform; // left or right multiply?

    glm::vec4 normal = temp * glm::transpose(inverted);

    return Intersection(point, normal, t, this);
}
