#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ray.h"
#include "intersection.h"
#include "la.h"
#include "material.h"
#include <QString>

class Geometry {

public:
    // transform matrix- rotation, scale, transform

    // if you cast a ray, will you hit - intersect function
    // Intersection = the Intersection class
    Geometry();
    QString name;
    QString material;
    QString type;
    Material mat;
    Geometry(glm::mat4 transform);
    glm::mat4 transform;
    virtual Intersection getIntersection(Ray& input) /*const*/;


};
#endif // GEOMETRY_H

