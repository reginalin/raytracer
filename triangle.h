#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "geometry.h"
#include "vec4.h"
#include "mat4.h"
#include "intersection.h"
#include "ray.h"

class Triangle : public Geometry {

public:
//The Triangle does not have a unit shape;
//it can have vertices at any 3D coordinates.
//It does not need to store normals per vertex;
//it may simply use its plane normal.

    vec4 P1, P2, P3;
    mat4 transform;

    Triangle(vec4 p1, vec4 p2, vec4 p3, mat4 transformMatrix);

    virtual Intersection getIntersection(Ray& input) /*const*/;

};
#endif // TRIANGLE_H
