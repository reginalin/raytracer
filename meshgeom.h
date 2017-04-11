#ifndef MESHGEOM_H
#define MESHGEOM_H

class Mesh : public Geometry {

public:
//The Mesh class encapsulate and OBJ file.

    glm::mat4 transform;

    Mesh(glm::mat4 transformMatrix) {
        transform = transformMatrix;
    }

    virtual Intersection getIntersection(const ray& input) /*const*/;

};

#endif // MESHGEOM_H
