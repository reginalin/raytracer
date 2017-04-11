#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Geometry {

public:
    const glm::vec4 center = glm::vec4(0, 0, 0, 0);
    const float radius = 0.5;
    glm::mat4 transform;

    Sphere(glm::mat4 transformMatrix) { }

    Intersection getIntersection(const ray& input) /*const*/;

    // just need to pass in transform matrix
};

#endif
