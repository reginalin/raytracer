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
#include <mat4.h>
#define _USE_MATH_DEFINES
#include <math.h>

class Scene
{
private:
    QJsonObject camera;
    QJsonArray geometry;
    QJsonArray material;
public:
    Scene();
    Scene(const char *filename);
    void parseGeometry();
    void parseCamera();
    void parseMaterial();

    mat4 rotation(float x, float y, float z);
};

#endif // SCENE_H
