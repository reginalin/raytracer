#ifndef CUBE_H
#define CUBE_H

#include "geometry.h"
#include "vec4.h"
#include "mat4.h"
#include "intersection.h"
#include "ray.h"

class Cube : public Geometry {

public:
    vec4 center = vec4(0, 0, 0, 0);
    float radius = 0.5;
    mat4 transform;

    Cube(mat4 transformMatrix);

    virtual Intersection getIntersection(ray& input) /*const*/;

    // just need to pass in transform matrix
};



#endif // CUBE_H
