#include "cube.h"
#include <math.h>
#include <iostream>
#include <QColor>

Cube::Cube(glm::mat4 transformMatrix) {
    transform = transformMatrix;
    std::cout<<"Constructed a cube"<< std::endl;
}

Intersection Cube::getIntersection(Ray& input) {
//    std::cout<<"Check intersection cube"<< std::endl;
    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);

    glm::vec3 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;
    float xmin = -radius;
    float xmax = radius;
    float ymin = -radius;
    float ymax = radius;
    float zmin = -radius;
    float zmax = radius;

    float tn = -INFINITY; // t near
    float tf = INFINITY; // t far
    float t0x, t1x, t0y, t1y, t0z, t1z;

    // x slab
    if (dir[0] == 0) {
        if (orig[0] < xmin || orig[0] > xmax) {
            std::cout<<"X SLAB"<<std::endl;

            return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
        }
    }

    t0x = (xmin - orig[0]) / dir[0];
    t1x = (xmax - orig[0]) / dir[0];

    if (t0x > t1x) {
        // swap
        float temp = t0x;
        t0x = t1x;
        t1x = temp;
    }

    if (t0x > tn) {
        tn = t0x;
    }

    if (t1x < tf) {
        tf = t1x;
    }

    // y slab
    if (dir[1] == 0) {
        if (orig[1] < ymin || orig[1] > ymax) {
            std::cout<<"Y SLAB"<<std::endl;
            return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
        }
    }

    t0y = (ymin - orig[1]) / dir[1];
    t1y = (ymax - orig[1]) / dir[1];

    if (t0y > t1y) {
        // swap
        float temp = t0y;
        t0y = t1y;
        t1y = temp;
    }

    if (t0y > tn) {
        tn = t0y;
    }

    if (t1y < tf) {
        tf = t1y;
    }

    // z slab
    if (dir[2] == 0) {
        if (orig[2] < zmin || orig[2] > zmax) {
            std::cout<<"Z SLAB"<<std::endl;

            return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
        }
    }

    t0z = (zmin - orig[2]) / dir[2];
    t1z = (zmax - orig[2]) / dir[2];

    if (t0z > t1z) {
        // swap
        float temp = t0z;
        t0z = t1z;
        t1z = temp;
    }

    if (t0z > tn) {
        tn = t0z;
    }

    if (t1z < tf) {
        tf = t1z;
    }


    if (tn > tf) {
//        std::cout<<"not on screen"<<std::endl;
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
    }


    glm::vec4 temp = orig + tn * glm::vec4(dir, 0);
    glm::vec4 point = transform * temp;
//    glm::vec3 normal = glm::normalize(glm::vec3(point - transform[3]));

    glm::vec3 normal;

    // calculate u and v and normals
    float x = temp[0];
    float y = temp[1];
    float z = temp[2];

    int coor = 0;
    float u, v;

    float max = std::max(std::abs(x), std::max(std::abs(y), std::abs(z)));
    if (max == std::abs(x)) {
        coor = 0;
    } else if (max == std::abs(y)) {
        coor = 1;
    } else if (max == std::abs(z)) {
        coor = 2;
    }

    if (coor == 0) {
        u = z + 0.5;
        v = y + 0.5;

        if (x < 0) {
            normal = glm::vec3(-1, 0, 0);
        } else if (x > 0) {
            normal = glm::vec3(1, 0, 0);
        }
    } else if (coor == 1) {
        u = x + 0.5;
        v = z + 0.5;

        if (y < 0) {
            normal = glm::vec3(0, -1, 0);
        } else if (y > 0) {
            normal = glm::vec3(0, 1, 0);
        }
    } else if (coor == 2) {
        u = x + 0.5;
        v = y + 0.5;

        if (z < 0) {
            normal = glm::vec3(0, 0, -1);
        } else if (z > 0) {
            normal = glm::vec3(0, 0, 1);
        }
    }

    glm::vec2 uv = glm::vec2(u, v);

    glm::vec3 normal1 = glm::normalize(glm::vec3(glm::transpose(inverted) * glm::vec4(normal, 0)));

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

        glm::vec3 rgb = glm::vec3(red, green, blue);

        // calculate orientation matrix
        glm::vec3 tangent = glm::cross(normal1, glm::vec3(0, 1, 0));
        tangent = glm::normalize(tangent);

        glm::vec3 bitangent = glm::cross(normal1, tangent);
        tangent = glm::normalize(tangent); // should this be normalized?

        glm::mat4 matrix = glm::mat4(glm::vec4(tangent[0], tangent[1], tangent[2], 0),
                                     glm::vec4(bitangent[0], bitangent[1], bitangent[2], 0),
                                     glm::vec4(normal1[0], normal1[1], normal1[2], 0),
                                     glm::vec4(0, 0, 0, 1));

        glm::vec3 newNormal = glm::normalize(glm::vec3(glm::vec4(rgb, 0) * matrix));

        return Intersection(point, newNormal, uv, tn, this);
    } else {
        return Intersection(point, normal1, uv, tn, this);
    }


}
