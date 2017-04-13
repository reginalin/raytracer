#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <vector>
#include <tiny_obj_loader.h>
#include "la.h"

class vertex
{
public:
    glm::vec4 coords;
    glm::vec4 normals;
    float u, v;
    vertex(glm::vec4 coords, glm::vec4 normals);
    vertex(glm::vec4 coords, glm::vec4 normals, float u, float v);
    vertex(const vertex &toCopy);
    vertex();
    float operator[](unsigned int index) const;
    float &operator[](unsigned int index);
};

class face
{
public:
    vertex index0;
    vertex index1;
    vertex index2;
    bool shouldRender;
    float xmin, xmax, ymin, ymax;
    glm::vec4 color;
    tinyobj::material_t mat;
    face(vertex point1, vertex point2, vertex point3, glm::vec4 color, tinyobj::material_t mat);
    void computeBounds();
};

#endif // COMPONENTS_H
