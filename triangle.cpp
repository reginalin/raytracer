#include "triangle.h"
#include <math.h>
#include <iostream>
#include <QColor>

Triangle::Triangle(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, std::vector<glm::vec2> uv, glm::mat4 transformMatrix, Mesh *obj) {
    P1 = p1;
    P2 = p2;
    P3 = p3;
    UV = uv;
    this->obj = obj;
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

    glm::vec4 temp = input.origin + glm::vec4(t * input.direction, 0);
    glm::vec4 point = temp;

    glm::vec3 normal = glm::vec3(temp);
    normal = glm::normalize(normal);

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

    float u2 = UV[0][0] * w1 + UV[1][0] * w2 + UV[2][0] * w3;
    float v2 = UV[0][1] * y1 + UV[1][1] * w2 + UV[2][1] * w3;

    glm::vec2 uv = glm::vec2(u2, v2);

    if (obj->mat.normalMap != "") {

        QImage *normalImg = obj->mat.normalImg;
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
        glm::vec3 tangent = ((UV[2][1] - UV[0][1]) * (glm::vec3(P2 - P1)) - (UV[1][1] - UV[0][1]) * (glm::vec3(P3 - P1)))
                            / ((UV[2][1] - UV[0][1]) * (UV[1][0] - UV[0][0]) - (UV[1][1] - UV[0][1]) * (UV[2][0] - UV[0][0]));
        tangent = glm::normalize(tangent);

        glm::vec3 bitangent = (glm::vec3(P3 - P1) - (UV[2][0] - UV[0][0]) * tangent) / (UV[2][1] - UV[0][1]);
        bitangent = glm::normalize(bitangent); // should this be normalized?

        // should anything be normalized??
        glm::mat4 matrix = glm::mat4(glm::vec4(tangent[0], tangent[1], tangent[2], 0),
                                     glm::vec4(bitangent[0], bitangent[1], bitangent[2], 0),
                                     glm::vec4(normal[0], normal[1], normal[2], 0),
                                     glm::vec4(0, 0, 0, 1));

        glm::vec3 newNormal = glm::normalize(glm::vec3(matrix * glm::vec4(rgb, 0)));

        newNormal = glm::vec3(glm::transpose(inverted) * glm::vec4(newNormal, 0));
        newNormal = glm::normalize(newNormal);

        return Intersection(point, newNormal, uv, t, obj);
    } else {
        normal = glm::vec3(glm::transpose(inverted) * temp);
        normal = glm::normalize(normal);
        return Intersection(point, normal, uv, t, obj);
    }
}

