#include "material.h"

Material::Material()
{

}

Material::Material(String type, String normalMap, String baseColor, bool emissive, bool reflective, String texture) {
    this->type = type;
    this->normalMap = normalMap;
    this->baseColor = baseColor;
    this->emissive = emissive;
    this->texture = texture;
    this->reflective = reflective;
}
