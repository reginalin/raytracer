#ifndef CUBE_H
#define CUBE_H

#include "geometry.h"
#include "intersection.h"
#include "ray.h"


class Cube : public Geometry {

public:
    glm::vec4 center = glm::vec4(0, 0, 0, 0);
    float radius = 0.5;
    glm::glm::mat4 transform;

    Cube(glm::glm::mat4 transformMatrix);

    virtual Intersection getIntersection(ray& input) /*const*/;

    // just need to pass in transform matrix
};



#endif // CUBE_H
