#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <vec4.h>
#include <vector>
#include <tiny_obj_loader.h>

class vertex
{
public:
    vec4 coords;
    vec4 normals;
    float u, v;
    vertex(vec4 coords, vec4 normals);
    vertex(vec4 coords, vec4 normals, float u, float v);
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
    vec4 color;
    tinyobj::material_t mat;
    face(vertex point1, vertex point2, vertex point3, vec4 color, tinyobj::material_t mat);
    void computeBounds();
};

#endif // COMPONENTS_H
