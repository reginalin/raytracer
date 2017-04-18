#ifndef MATERIAL_H
#define MATERIAL_H

#include "string.h"
#include <iostream>

class Material
{
public:
    Material();
    Material(std::string type, std::string normalMap, std::string baseColor, bool emissive, bool reflective, std::string texture);

    std::string type;
    std::string name;
    std::string normalMap;
    std::string baseColor;
    bool emissive;
    bool reflective;
    std::string texture;

};

#endif // MATERIAL_H
