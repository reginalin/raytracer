#ifndef SQUAREPLANE_H
#define SQUAREPLANE_H

#include "geometry.h"
#include "vec4.h"
#include "mat4.h"
#include "intersection.h"
#include "ray.h"


class SquarePlane : public Geometry {

public:
//    The SquarePlane class should span [-0.5, 0.5] in the X and Y directions,
//            and should have Z coordinates of 0, meaning it sits in the Z plane centered at the origin.
    mat4 transform;

    SquarePlane(mat4 transformMatrix);

    virtual Intersection getIntersection(ray& input) /*const*/;

};

#endif // SQUAREPLANE_H
