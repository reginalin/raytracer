#include "triangle.h"
#include <math.h>


Triangle::Triangle(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::mat4 transformMatrix) {
    P1 = p1;
    P2 = p2;
    P3 = p3;
    transform = transformMatrix;
}

glm::vec4 crossVec4(glm::vec4 v1, glm::vec4 v2){
    glm::vec3 vec1 = glm::vec3(v1[0], v1[1], v1[2]);
    glm::vec3 vec2 = glm::vec3(v2[0], v2[1], v2[2]);
    glm::vec3 res = glm::cross(vec1, vec2);
    return glm::vec4(res[0], res[1], res[2], 1);
}


Intersection Triangle::getIntersection(Ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);

    glm::vec3 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;

    glm::vec3 E1 = glm::vec3(P2 - P1);
    glm::vec3 E2 = glm::vec3(P3 - P1);
    glm::vec3 T = glm::vec3(orig - P1);

    glm::vec4 tuv = (1 / glm::dot(glm::cross(dir, E2), E1)) * glm::vec4(glm::dot(glm::cross(T, E1), E2), glm::dot(glm::cross(dir, E2), T), glm::dot(glm::cross(T, E1), dir), 1.0);

    float t = tuv[0];
    float u = tuv[1];
    float v = tuv[2];

    if (u < 0 || u > 1 || v < 0 || v > 1) {
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), -1, this);
    }

    glm::vec4 temp = orig + glm::vec4(t * dir, 0);
    glm::vec4 point = transform * temp;

    glm::vec3 normal = glm::vec3(glm::transpose(inverted) * temp);

    return Intersection(point, normal, t, this);
}
