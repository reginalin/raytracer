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

class Scene
{
public:
    Scene();
    Scene(const char *filename);
    void parseGeometry();
    void parseCamera();
    void parseMaterial();
    QJsonObject camera;
    Camera cam;
    QJsonArray geometry;
    QJsonArray material;

    glm::mat4 rotation(float x, float y, float z);
};

#endif // SCENE_H
