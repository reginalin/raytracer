#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "tiny_obj_loader.h"
<<<<<<< HEAD
#include "la.h"
=======
>>>>>>> 11db3696eae68f0f97c9cf61e6d6ee6d9957e929

class Geometry;

class Intersection
{
public:
    glm::vec4 position;
    glm::vec4 normal;
    float t;
    Geometry *geometry;
    Intersection(glm::vec4 position, glm::vec4 normal, float t, Geometry *geometry);
};

#endif // INTERSECTION_H
