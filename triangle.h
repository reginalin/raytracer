#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "geometry.h"
#include "intersection.h"
#include "ray.h"

class Triangle : public Geometry {

public:
//The Triangle does not have a unit shape;
//it can have vertices at any 3D coordinates.
//It does not need to store normals per vertex;
//it may simply use its plane normal.

    glm::vec4 P1, P2, P3;
    glm::mat4 transform;

    Triangle(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::mat4 transformMatrix);

    virtual Intersection getIntersection(ray& input) /*const*/;

};
#endif // TRIANGLE_H
