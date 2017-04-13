#ifndef SPHERE_H
#define SPHERE_H

#include "geometry.h"
#include "vec4.h"
#include "mat4.h"
#include "intersection.h"
#include "ray.h"

class Sphere : public Geometry {

public:
    vec4 center = vec4(0, 0, 0, 0);
    float radius = 0.5;
    mat4 transform;

    Sphere(mat4 transformMatrix);

    Intersection getIntersection(Ray& input) /*const*/;
};

#endif
