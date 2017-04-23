#include "cube.h"
#include <math.h>
#include <iostream>
Cube::Cube(glm::mat4 transformMatrix) {
    transform = transformMatrix;
    std::cout<<"Constructed a cube"<< std::endl;
}

Intersection Cube::getIntersection(Ray& input) {
    std::cout<<"Check intersection cube"<< std::endl;
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
        std::cout<<"not on screen"<<std::endl;
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
    }

//    max of x, y, z and glm sine

    glm::vec4 temp = orig + tn * glm::vec4(dir, 0);

    std::cout<<temp[0]<<std::endl;


    glm::vec4 point = transform * temp;

    glm::vec3 normal = glm::vec3(glm::transpose(inverted) * temp);

    // texture mapping
    float u = temp[0] + 0.5;
    float v = temp[1] + 0.5;
    // not good- interpolate between corners

    // normal mapping
//    float max = std::max(temp[0], std::max(temp[1], temp[2]));
    glm::vec3 newNormal;
    // what if the values aren't exactly 0.5/-0.5???
//    if (point[0] == 0.5) {
//        newNormal = glm::vec3(0.5, 0, 1);
//    } else if (point[0] == -0.5) {
//        newNormal = glm::vec3(-0.5, 0, -1);
//    } else if (point[1] == 0.5) {
//        newNormal = glm::vec3();
//    } else if (point[1] == -0.5) {
//        newNormal = glm::vec3(0, 0, -1);
//    } else if (point[2] == 0.5) {
//        newNormal = glm::vec3(0, 0, -1);
//    } else if (point[2] == -0.5) {
//        newNormal = glm::vec3(0, 0, -1);
//    }

    return Intersection(point, normal, glm::vec2(u, v), tn, this);
}
