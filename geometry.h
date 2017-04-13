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
<<<<<<< HEAD
    virtual Intersection getIntersection(ray& input) /*= 0*/ /*const*/;
    Geometry();
=======
    virtual Intersection getIntersection(Ray& input) /*= 0*/ /*const*/;
>>>>>>> 11db3696eae68f0f97c9cf61e6d6ee6d9957e929
};



#endif // GEOMETRY_H

