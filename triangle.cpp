#include "triangle.h"
#include <math.h>


Triangle::Triangle(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::mat4 transformMatrix) {
    P1 = p1;
    P2 = p2;
    P3 = p3;
    transform = transformMatrix;
}

Intersection Triangle::getIntersection(ray& input) {
    glm::mat4 inverted = inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);

    glm::vec4 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;

    glm::vec4 E1 = P2 - P1;
    glm::vec4 E2 = P3 - P1;
    glm::vec4 T = orig - P1;

    glm::vec4 tuv = (1 / cross(dir, E1)) * glm::vec4(dot(cross(T, E1), E2), dot(cross(dir, E2), T), dot(cross(dir, E1), T), 1);

    float t = tuv[0];
    float u = tuv[1];
    float v = tuv[2];

    if (u < 0 || u > 1 || v < 0 || v > 1) {
        return NULL;
    }

    glm::vec4 temp = orig + t * dir;
    glm::vec4 point = temp * transform; // left or right multiply?

    glm::vec4 normal = temp * transpose(inverted);

    return Intersection(point, normal, t, this);
}
