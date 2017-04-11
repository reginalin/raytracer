#ifndef SQUAREPLANE_H
#define SQUAREPLANE_H

class SquarePlane : public Geometry {

public:
//    The SquarePlane class should span [-0.5, 0.5] in the X and Y directions,
//            and should have Z coordinates of 0, meaning it sits in the Z plane centered at the origin.
    glm::mat4 transform;

    SquarePlane(glm::mat4 transformMatrix) {
        transform = transformMatrix;
    }

    virtual Intersection getIntersection(const ray& input) /*const*/;

    // just need to pass in transform matrix
};

#endif // SQUAREPLANE_H
