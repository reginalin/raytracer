#include "meshgeom.h"
#include <math.h>


Mesh::Mesh(glm::mat4 transformMatrix) {
    transform = transformMatrix;
}

Intersection Mesh::getIntersection(ray& input) {
    glm::mat4 inverted = inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);

    glm::vec4 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;



    glm::vec4 temp = orig + t * dir;
    glm::vec4 point = temp * transform; // left or right multiply?

    glm::vec4 normal = temp * transpose(inverted);

    return Intersection(point, normal, t, this);
}
