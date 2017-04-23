#include "sphere.h"
#include <math.h>
#include <iostream>

const float PI = 3.14159265358979323846264338327950288419716939937510582;

Sphere::Sphere(glm::mat4 transformMatrix) {
    transform = transformMatrix;
    std::cout<<"Constructed a sphere"<< std::endl;
}

Intersection Sphere::getIntersection(Ray& input) {
//    std::cout<<"Check intersection sphere"<< std::endl;

    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);


    glm::vec3 dir = glm::vec3(objRay.direction[0], objRay.direction[1], objRay.direction[2]); // vec3 to vec4
    glm::vec4 orig = objRay.origin;
    
    glm::vec3 s = glm::vec3(orig - center);
    float a = glm::dot(dir, dir);
    float b = glm::dot(dir, s) * 2;
    float c = glm::dot(s, s) + (radius * radius);

    float disc = (b * b) - (4 * a * c);
    if (disc < 0) {
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
    }

    float t0, t1;

    t0 = (-b - (sqrtf(disc))) / (2 * a);
    if (t0 < 0) {
        t1 = (-b + (sqrtf(disc))) / (2 * a);
        t0 = t1;
        // what if both are negative
    }

    glm::vec4 temp = orig + glm::vec4(t0 * dir, 0);
    glm::vec4 point = transform * temp;

    glm::vec3 normal = glm::vec3(glm::transpose(inverted) * temp);

    // calculate u, v
    glm::vec3 norm = normal;
    glm::normalize(norm);

    float psi = atan2f(norm[2], norm[0]);
    if (psi < 0) {
        psi += 2 * PI;
    }

    float theta = acos(norm[1]);

    float u = 1 - (psi / (2 * PI));
    float v = 1 - (theta / PI);

    glm::vec2 uv = glm::vec2(u, v);

    // normal mapping
    glm::vec3 tangent = glm::cross(normal, glm::vec3(0, 1, 0));
    glm::normalize(tangent);

    glm::vec3 bitangent = glm::cross(normal, tangent);
    glm::normalize(tangent); // should this be normalized?

    glm::mat4 matrix = glm::mat4(glm::vec4(tangent[0], tangent[1], tangent[2], 0),
                                 glm::vec4(bitangent[0], bitangent[1], bitangent[2], 0),
                                 glm::vec4(normal[0], normal[1], normal[2], 0),
                                 glm::vec4(0, 0, 0, 1));

    glm::vec3 newNormal = glm::vec3(glm::vec4(normal, 0) * matrix);

    return Intersection(point, newNormal, uv, t0, this);
}


