#include "sphere.h"
#include <math.h>
#include <iostream>
#include <QColor>

const float PI = 3.14159265358979323846264338327950288419716939937510582;

Sphere::Sphere(glm::mat4 transformMatrix) {
    this->transform = transformMatrix;
    std::cout<<"Constructed a sphere"<< std::endl;
}

Intersection Sphere::getIntersection(Ray& input) {
//    std::cout<<"Check intersection sphere"<< std::endl;

    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);

    glm::vec3 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;
    
    glm::vec3 s = glm::vec3(orig[0] - center[0], orig[1] - center[1], orig[2] - center[2]);
    float a = (dir[0] * dir[0]) + (dir[1] * dir[1]) + (dir[2] * dir[2]);
    float b = 2 * ((dir[0] * s[0]) + (dir[1] * s[1]) + (dir[2] * s[2]));
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
    }

    glm::vec4 temp = orig + glm::vec4(t0 * dir, 0);
    glm::vec4 point = transform * temp;

    glm::vec3 normal = glm::vec3(temp);
    normal = glm::normalize(normal);

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
    if (this->mat.normalMap != "") {
        QImage *normalImg = this->mat.normalImg;
        int nx = normalImg->width() - 1;
        int ny = normalImg->height() - 1;

        float u1 = u * nx - floor(u * nx);
        float v1 = v * ny - floor(v * ny);

        // indices for the pixel the intersection is mapped to
        int i = (int)floor(u * (float)nx);
        int j = (int)floor(v * (float)ny);

        if (i + 1 >= nx) {
            i--;
        }

        if (j + 1 >= ny) {
            j--;
        }

        QColor first(normalImg->pixel(i, j));
        QColor second(normalImg->pixel(i + 1, j));
        QColor third(normalImg->pixel(i, j + 1));
        QColor fourth(normalImg->pixel(i + 1, j + 1));

        unsigned char red = (1 - u1) * (1 - v1) * first.red() + u1 * (1 - v1) * second.red()
                        + (1 - u1) * v1 * third.red() + u1 * v1 * fourth.red();

        unsigned char green = (1 - u1) * (1 - v1) * first.green() + u1 * (1 - v1) * second.green()
                        + (1 - u1) * v1 * third.green() + u1 * v1 * fourth.green();

        unsigned char blue = (1 - u1) * (1 - v1) * first.blue() + u1 * (1 - v1) * second.blue()
                        + (1 - u1) * v1 * third.blue() + u1 * v1 * fourth.blue();

        float x = ((red / 255.f) * 2.f) - 1.f;
        float y = ((green / 255.f) * 2.f) - 1.f;
        float z = ((blue / 255.f) * 2.f) - 1.f;

        glm::vec3 rgb = glm::vec3(x, y, z);

        // calculate orientation matrix
        glm::vec3 tangent = glm::cross(glm::vec3(0, 1, 0), normal);
        tangent = glm::normalize(tangent);

        glm::vec3 bitangent = glm::cross(normal, tangent);
        bitangent = glm::normalize(bitangent);

        glm::mat4 matrix = glm::mat4(glm::vec4(tangent[0], tangent[1], tangent[2], 0),
                                     glm::vec4(bitangent[0], bitangent[1], bitangent[2], 0),
                                     glm::vec4(normal[0], normal[1], normal[2], 0),
                                     glm::vec4(0, 0, 0, 1));

        glm::vec3 newNormal = glm::normalize(glm::vec3(matrix * glm::vec4(rgb, 0)));

        newNormal = glm::vec3(glm::transpose(inverted) * glm::vec4(newNormal, 0));
        newNormal = glm::normalize(newNormal);

        return Intersection(point, newNormal, uv, t0, this);
    } else {
        normal = glm::vec3(glm::transpose(inverted) * temp);
        normal = glm::normalize(normal);
        return Intersection(point, normal, uv, t0, this);
    }
}


