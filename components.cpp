#include "components.h"

vertex::vertex(vec4 coords, vec4 normals) {
    this->coords = coords;
    this->normals = normals;
    u = 0;
    v = 0;
}

vertex::vertex(vec4 coords, vec4 normals, float u, float v) {
    this->coords = coords;
    this->normals = normals;
    this->u = u;
    this->v = v;
}

vertex::vertex(const vertex &toCopy) {
    this->coords = toCopy.coords;
    this->normals = toCopy.normals;
    u = toCopy.u;
    v = toCopy.v;
}

vertex::vertex() {
    this->coords = vec4(0, 0, 0, 1);
    this->normals = vec4(0, 0, 0, 1);
    u = 0;
    v = 0;
}

float vertex::operator[](unsigned int index) const {
    return coords[index];
}

float &vertex::operator[](unsigned int index) {
    float &toReturn = coords[index];
    return toReturn;
}

face::face(vertex point1, vertex point2, vertex point3, vec4 color, tinyobj::material_t mat) {
    index0 = point1;
    index1 = point2;
    index2 = point3;
    this->color = color;
    this->mat = mat;
    shouldRender = true;
}

void face::computeBounds() {
    xmin = std::min(index0[0], std::min(index1[0], index2[0]));
    xmax = std::max(index0[0], std::max(index1[0], index2[0]));
    ymin = std::min(index0[1], std::min(index1[1], index2[1]));
    ymax = std::max(index0[1], std::max(index1[1], index2[1]));
}
