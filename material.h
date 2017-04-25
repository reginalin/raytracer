#ifndef MATERIAL_H
#define MATERIAL_H

#include "string.h"
#include <iostream>
#include "color.h"

class Material
{
public:
    Material();
    Material(std::string type, std::string normalMap, Color baseColor, bool emissive, bool reflective, std::string texture);

    std::string type;
    std::string name;
    std::string normalMap;
    Color baseColor;
    bool emissive;
    bool reflective;
    std::string texture;

};

#endif // MATERIAL_H
