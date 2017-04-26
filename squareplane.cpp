#include "squareplane.h"
#include <math.h>


SquarePlane::SquarePlane(glm::mat4 transformMatrix) {
    transform = transformMatrix;
}


Intersection SquarePlane::getIntersection(Ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);

    glm::vec4 planePoint = glm::vec4(0, 0, 0, 0);
    glm::vec3 planeNormal = glm::vec3(0, 0, 1);
    glm::vec3 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;

    if (orig[0] < -0.5 || orig[0] > 0.5 || orig[1] < -0.5 || orig[1] > 0.5) {
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
    }

    float t = glm::dot(planeNormal, glm::vec3(planePoint - orig)) / glm::dot(planeNormal, dir);
    glm::vec4 temp = objRay.origin + glm::vec4(t * dir, 0);
    glm::vec4 point = transform * temp;

    // texture mapping- change span from [-0.5, 0.5] to [0.0, 1.0]
    float u = temp[0] + 0.5;
    float v = temp[1] + 0.5;

    // normal mapping
    glm::vec4 normal = glm::vec4(0, 0, 1, 0);
    glm::vec3 normal1 = glm::normalize(glm::vec3(glm::transpose(inverted) * normal));

    return Intersection(point, normal1, glm::vec2(u, v), t, this);
}
