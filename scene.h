#ifndef SCENE_H
#define SCENE_H
#include <QFile>
#include <QIODevice>
#include <iostream>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include "la.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "camera.h"
#include <geometry.h>
#include <sphere.h>
#include <cube.h>
#include <squareplane.h>
#include <meshgeom.h>
#include <material.h>
#include <map>
#include <stdio.h>

class Scene
{
public:
    QJsonObject camera;
    QJsonArray geometry;
    QJsonArray material;
    Camera cam;

    std::vector<Geometry *> geo_objs;
    std::map<QString, Material> material_types;

    Scene();
    Scene(const char *filename);
    void parseGeometry();
    void parseCamera();
    void parseMaterial();

    glm::mat4 rotation(float x, float y, float z);
};

#endif // SCENE_H
