#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle : public Geometry {

public:
//The Triangle does not have a unit shape;
//it can have vertices at any 3D coordinates.
//It does not need to store normals per vertex;
//it may simply use its plane normal.

    glm::mat4 transform;

    Triangle(glm::mat4 transformMatrix) {
        transform = transformMatrix;
    }

    virtual Intersection getIntersection(const ray& input) /*const*/;

    // just need to pass in transform matrix
};
#endif // TRIANGLE_H
