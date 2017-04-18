#ifndef MESHGEOM_H
#define MESHGEOM_H

#include "geometry.h"
#include "intersection.h"
#include "ray.h"
#include "camera.h"

class Mesh : public Geometry {

public:
//The Mesh class encapsulate and OBJ file.

    glm::mat4 transform;
    Camera cam;

    Mesh(glm::mat4 transformMatrix, char* inputFile, Camera cam);

    virtual Intersection getIntersection(Ray& input) /*const*/;

};

#endif // MESHGEOM_H
