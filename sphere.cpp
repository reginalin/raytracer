#include "sphere.h"
#include <math.h>
#include <iostream>

const float PI = 3.14159265358979323846264338327950288419716939937510582;

Sphere::Sphere(glm::mat4 transformMatrix) {
    this->transform = transformMatrix;
    std::cout<<"Constructed a sphere"<< std::endl;
}

Intersection Sphere::getIntersection(Ray& input) {
//    std::cout<<"Check intersection sphere"<< std::endl;

    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);

//    glm::vec3 dir = glm::vec3(objRay.direction[0], objRay.direction[1], objRay.direction[2]); // vec3 to vec4
    glm::vec3 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;
    
    glm::vec3 s = glm::vec3(orig[0] - center[0], orig[1] - center[1], orig[2] - center[2]);
//    float a = glm::dot(dir, dir);
    float a = (dir[0] * dir[0]) + (dir[1] * dir[1]) + (dir[2] * dir[2]);
//    float b = glm::dot(dir, s) * 2;
    float b = 2 * ((dir[0] * s[0]) + (dir[1] * s[1]) + (dir[2] * s[2]));
//    float c = glm::dot(s, s) + (radius * radius);
    float c = (s[0] * s[0]) + (s[1] * s[1]) + (s[2] * s[2]) - (radius * radius);

    float disc = (b * b) - (4 * a * c);
    if (disc < 0) {
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
    }

    float t0, t1 = -10;

    t0 = (-b - (sqrtf(disc))) / (2 * a);
    if (t0 < 0) {
        t1 = (-b + (sqrtf(disc))) / (2 * a);
        t0 = t1;
        // what if both are negative
    }

    glm::vec4 temp = input.origin + glm::vec4(t0 * input.direction, 0);
    glm::vec4 point = temp;

    glm::vec3 normal = glm::normalize(glm::vec3(point - transform[3]));

    // calculate u, v
    float psi = atan2f(normal[2], normal[0]);
    if (psi < 0) {
        psi += 2 * PI;
    }

    float theta = acos(normal[1]);

    float u = 1 - (psi / (2 * PI));
    float v = 1 - (theta / PI);

    glm::vec2 uv = glm::vec2(u, v);

    // normal mapping
//    glm::vec3 tangent = glm::cross(normal, glm::vec3(0, 1, 0));
//    tangent = glm::normalize(tangent);

//    glm::vec3 bitangent = glm::cross(normal, tangent);
//    tangent = glm::normalize(tangent); // should this be normalized?

//    glm::mat4 matrix = glm::mat4(glm::vec4(tangent[0], tangent[1], tangent[2], 0),
//                                 glm::vec4(bitangent[0], bitangent[1], bitangent[2], 0),
//                                 glm::vec4(normal[0], normal[1], normal[2], 0),
//                                 glm::vec4(0, 0, 0, 1));

//    glm::vec3 newNormal = glm::normalize(glm::vec3(glm::vec4(normal, 0) * matrix));

//    std::cout << glm::to_string(newNormal) << std::endl;

    return Intersection(point, normal, uv, t0, this);
}


