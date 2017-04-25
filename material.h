#ifndef MATERIAL_H
#define MATERIAL_H

#include "string.h"
#include <QString>
#include <iostream>
#include "color.h"

class Material
{
public:
    Material();
    Material(QString type, QString normalMap, Color baseColor, bool emissive, bool reflective, QString texture);

    QString type;
    QString name;
    QString normalMap;
    Color baseColor;
    bool emissive;
    bool reflective;
    QString texture;

};

#endif // MATERIAL_H
