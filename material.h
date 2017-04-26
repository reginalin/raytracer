#ifndef MATERIAL_H
#define MATERIAL_H

#include "string.h"
#include <QString>
#include <iostream>
#include "color.h"
#include <QImage>

class Material
{
public:
    Material();
    Material(QString type, QString normalMap, Color baseColor, bool emissive, bool reflective, float reflectivity, QString texture);

    QString type;
    QString name;
    QString normalMap;
    QString texture;
    QImage *textureImg;
    QImage *normalImg;

    Color baseColor;
    bool emissive;
    bool reflective;
    float reflectivity;

};

#endif // MATERIAL_H
