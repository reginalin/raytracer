#ifndef MESHGEOM_H
#define MESHGEOM_H

#include "geometry.h"
#include "intersection.h"
#include "ray.h"

class Mesh : public Geometry {

public:
//The Mesh class encapsulate and OBJ file.

    glm::mat4 transform;

    Mesh(glm::mat4 transformMatrix);

    virtual Intersection getIntersection(ray& input) /*const*/;

};

#endif // MESHGEOM_H
