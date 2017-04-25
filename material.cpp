#include "material.h"

Material::Material()
{

}

Material::Material(std::string type, std::string normalMap, Color baseColor, bool emissive, bool reflective, std::string texture) {
    this->type = type;
    this->normalMap = normalMap;
    this->baseColor = baseColor;
    this->emissive = emissive;
    this->texture = texture;
    this->reflective = reflective;
}
