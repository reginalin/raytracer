#ifndef SPHERE_H
#define SPHERE_H

#include "geometry.h"
#include "intersection.h"
#include "ray.h"

class Sphere : public Geometry {

public:
    glm::vec4 center = glm::vec4(0, 0, 0, 0);
    float radius = 0.5;
//    static glm::vec4 normal;

    Sphere(glm::mat4 transformMatrix);

    virtual Intersection getIntersection(Ray& input) /*const*/;
};

#endif
