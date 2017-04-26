#include "color.h"

Color::Color() {
    r = 255;
    g = 255;
    b = 255;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

//Deprecated color class. Replaced with vec3s. Regina, you may want to move lambert shading to main.cpp
void Color::lambert(Intersection intersection) {
    glm::vec3 norm = intersection.normal;
    //light direction is vector from light source to center...
    glm::vec3 light;
}


