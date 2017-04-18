#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ray.h"
#include "intersection.h"
#include "la.h"

class Geometry {

public:
    // transform matrix- rotation, scale, transform

    // if you cast a ray, will you hit - intersect function
    // Intersection = the Intersection class

    Geometry();
    virtual Intersection getIntersection(Ray& input) /*= 0*/ /*const*/;

};



#endif // GEOMETRY_H

