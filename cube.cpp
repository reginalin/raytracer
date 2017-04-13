#include "cube.h"
#include <math.h>


Cube::Cube(mat4 transformMatrix) {
    transform = transformMatrix;
}

Intersection Cube::getIntersection(const Ray& input) {
    mat4 inverted = inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);

    vec4 dir = objRay.direction;
    vec4 orig = objRay.origin;
    float xmin, xmax, ymin, ymax, zmin, zmax = radius;

    float tn = -INFINITY; // t near
    float tf = INFINITY; // t far
    float t0x, t1x, t0y, t1y, t0z, t1z;

    // x slab
    if (dir[0] == 0) {
        if (orig[0] < xmin || orig[0] > xmax) {
            return NULL;
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
            return NULL;
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
            return NULL;
        }
    }

    t0z = (zmin - orig[2]) / dir[2];
    t1z = (zmax - orig[2]) / dir[2];

    if (t0z > t1z) {
        // swap
        float temp = t0xz;
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
        return NULL;
    }

    vec4 temp = orig + tn * dir;
    vec4 point = temp * transform; // left or right multiply?
//    vec3 point = vec3(pointTemp);

    vec4 normal = temp * transpose(inverted);
//    vec3 normal = vec3(normTemp);

    return Intersection(point, normal, tn, this);
}
