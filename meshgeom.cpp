#include "meshgeom.h"
#include <math.h>


Mesh::Mesh(mat4 transformMatrix) {
    transform = transformMatrix;
}

Intersection Mesh::getIntersection(ray& input) {
    mat4 inverted = inverse(transform);
    ray objRay = input.getTransformedCopy(inverted);

    vec4 dir = objRay.direction;
    vec4 orig = objRay.origin;



    vec4 temp = rayObj.origin + t * dir;
    vec4 point = temp * transform; // left or right multiply?

    vec4 normal = temp * transpose(inverted);

    return Intersection(point, normal, t, this);
}
