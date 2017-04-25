#include "color.h"

Color::Color(unsigned char r, unsigned char g, unsigned char b)
{
    this->r = r;
    this->g = g;
    this->b = b;

}

void Color::lambert(Intersection intersection) {
    glm::vec3 norm = intersection.normal;
    //light direction is vector from light source to ...
    glm::vec3 light;
}


