#ifndef CUBE_H
#define CUBE_H

class Cube : public Geometry {

public:
    const glm::vec4 center = glm::vec4(0, 0, 0, 0);
    const float radius = 0.5;
    glm::mat4 transform;

    Cube(glm::mat4 transformMatrix) { }

    virtual Intersection getIntersection(const ray& input) /*const*/;

    // just need to pass in transform matrix
};



#endif // CUBE_H
