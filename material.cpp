#include "material.h"

Material::Material()
{

}

Material::Material(QString type, QString normalMap, Color baseColor, bool emissive, bool reflective, float reflectivity, QString texture) {
    this->type = type;
    this->normalMap = normalMap;
    this->baseColor = baseColor;
    this->emissive = emissive;
    this->texture = texture;
    this->reflective = reflective;
    this->reflectivity = reflectivity;
}
