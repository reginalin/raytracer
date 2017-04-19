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
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
    }

    glm::vec4 temp = orig + glm::vec4(t * dir, 0);
    glm::vec4 point = transform * temp;

    glm::vec3 normal = glm::vec3(glm::transpose(inverted) * temp);

    // calculate u, v
    float x1 = Triangle::P1[0];
    float y1 = Triangle::P1[1];
    float x2 = Triangle::P1[0];
    float y2 = Triangle::P1[1];
    float x3 = Triangle::P1[0];
    float y3 = Triangle::P1[1];

    float s = 0.5 * glm::cross(glm::vec3(x1, y1, 0) - glm::vec3(x2, y2, 0), glm::vec3(x3, y3, 0) - glm::vec3(x2, y2, 0)).length();
    float s1 = 0.5 * glm::cross(glm::vec3(point[0], point[1], 0) - glm::vec3(x2, y2, 0), glm::vec3(point[0], point[1], 0) - glm::vec3(x3, y3, 0)).length();
    float s2 = 0.5 * glm::cross(glm::vec3(point[0], point[1], 0) - glm::vec3(x3, y3, 0), glm::vec3(point[0], point[1], 0) - glm::vec3(x1, y1, 0)).length();
    float s3 = 0.5 * glm::cross(glm::vec3(point[0], point[1], 0) - glm::vec3(x1, y1, 0), glm::vec3(point[0], point[1], 0) - glm::vec3(x2, y2, 0)).length();

    float w1 = s1 / s;
    float w2 = s2 / s;
    float w3 = s3 / s;

    float u1 = x1 * w1 + x2 * w2 + x3 * w3;
    float v1 = y1 * y1 + y2 * w2 + y3 * w3;

    // normal mapping
    glm::vec3 tangent = ((y3 - y1) * (glm::vec3(P2 - P1)) - (y2 - y1) * (glm::vec3(P3 - P1))) / ((y3 - y1) * (x2 - x1) - (y2 - y1) * (x3 - x1));

    glm::vec3 bitangent = (glm::vec3(P3 - P1) - (x3 - x1) * tangent) / (y3 - y1);

    // should anything be normalized??
    glm::mat4 matrix = glm::mat4(glm::vec4(tangent[0], tangent[1], tangent[2], 0),
                                 glm::vec4(bitangent[0], bitangent[1], bitangent[2], 0),
                                 glm::vec4(normal[0], normal[1], normal[2], 0),
                                 glm::vec4(0, 0, 0, 1));

    glm::vec3 newNormal = glm::vec3(glm::vec4(normal, 0) * matrix);

    return Intersection(point, newNormal, glm::vec2(u1, v1), t, this);
}

