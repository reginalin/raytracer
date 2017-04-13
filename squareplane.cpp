#include "squareplane.h"
#include <math.h>


SquarePlane::SquarePlane(glm::mat4 transformMatrix) {
    transform = transformMatrix;
}

Intersection SquarePlane::getIntersection(ray& input) {
    glm::mat4 inverted = inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);

    glm::vec4 planePoint = glm::vec4(0, 0, 0, 0);
    glm::vec4 planeNormal = glm::vec4(0, 0, 1, 1);
    glm::vec4 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;

    if (orig[0] < -0.5 || orig[0] > 0.5 || orig[1] < -0.5 || orig[1] > 0.5) {
        return NULL;
    }

    float t = dot(planeNormal, (planePoint - orig)) / dot(planeNormal, dir);

    glm::vec4 temp = objRay.origin + t * dir;
    glm::vec4 point = temp * transform; // left or right multiply?
//    vec3 point = vec3(pointTemp);

    glm::vec4 normal = temp * transpose(inverted);
//    vec3 normal = vec3(normTemp);

    return Intersection(point, normal, t, this);
}
