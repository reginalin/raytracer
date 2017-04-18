#ifndef MESHGEOM_H
#define MESHGEOM_H

#include "geometry.h"
#include "intersection.h"
#include "ray.h"
#include "stdio.h"
#include "triangle.h"

class Mesh : public Geometry {

public:
//The Mesh class encapsulate and OBJ file.

    glm::mat4 transform;
    std::vector<Triangle> triangles;

    Mesh(glm::mat4 transformMatrix, char* inputFile);

    virtual Intersection getIntersection(Ray& input) /*const*/;

};

#endif // MESHGEOM_H
