#include "scene.h"

Scene::Scene() { }

Scene::Scene(const char *filename) {
    QString contents;
    QString fn;
    fn.sprintf("%s", filename);

    QDir dir = QDir::currentPath();
    QString path = QCoreApplication::applicationDirPath() + "/" + fn;

    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    contents = file.readAll();
    file.close();

    QJsonDocument json = QJsonDocument::fromJson(contents.toUtf8());
    QJsonObject object = json.object();
    QJsonObject scene = object.value("scene").toObject();
    QJsonObject camera = scene.value("camera").toObject();
    geometry = scene.value("geometry").toArray();
    material = scene.value("material").toArray();

    parseGeometry();
    parseCamera();
    parseMaterial();
}

void Scene::parseGeometry() {
    foreach (const QJsonValue & v, geometry) {
        QJsonArray scale, rotate, translate;
        QJsonObject geo_obj = v.toObject();
        QJsonValue material = geo_obj["material"];
        QJsonValue type = geo_obj["type"];
        QJsonValue name = geo_obj["name"];

        //these are optional
        if (geo_obj.contains("transform")) {
            QJsonObject transform = geo_obj["transform"].toObject();
            if (transform.contains("scale")) {
                scale = transform["scale"].toArray();
                float x = (float) scale.at(0).toDouble();
                float y = (float) scale.at(1).toDouble();
                float z = (float) scale.at(2).toDouble();
                mat4 scale_matrix = mat4::scale(x, y, z);
            }
            if (transform.contains("rotate")) {
                rotate = transform["rotate"].toArray();
                float x = (float) rotate.at(0).toDouble();
                float y = (float) rotate.at(1).toDouble();
                float z = (float) rotate.at(2).toDouble();
                mat4 rot_mat = rotation(x, y, z);
            }
            if (transform.contains("translate")) {
                translate = transform["translate"].toArray();
                float x = (float) translate.at(0).toDouble();
                float y = (float) translate.at(1).toDouble();
                float z = (float) translate.at(2).toDouble();
                mat4 trans_matrix = mat4::trans(x, y, z);
            }
        }

        //create geometry object
    }
}

//rotation martrix
mat4 Scene::rotation(float x, float y, float z) {
    vec4 x0 = vec4(1, 0, 0, 0);
    vec4 x1 = vec4(0, cos(x), sin(x), 0);
    vec4 x2 = vec4(0, -sin(x), cos(x), 0);
    vec4 col3 = vec4(0, 0, 0, 1);
    mat4 rx = mat4(x0, x1, x2, col3);

    vec4 y0 = vec4(cos(y), 0, -sin(y), 0);
    vec4 y1 = vec4(0, 1, 0, 0);
    vec4 y2 = vec4(sin(y), 0, cos(y), 0);
    mat4 ry = mat4(y0, y1, y2, col3);

    vec4 z0 = vec4(cos(z), sin(z), 0, 0);
    vec4 z1 = vec4(-sin(z), cos(z), 0, 0);
    vec4 z2 = vec4(0, 0, 1, 0);
    mat4 rz = mat4(z0, z1, z2, col3);

    std::cout << rx * ry * rz;
}

void Scene::parseCamera() {
    QJsonArray target = camera["target"].toArray();
    QJsonArray eye = camera["eye"].toArray();
    QJsonArray worldUp = camera["worldUp"].toArray();
    int fov = camera["fov"].toInt();
    int width = camera["width"].toInt();
    int height = camera["height"].toInt();
}

void Scene::parseMaterial() {
    foreach (const QJsonValue & v, material) {
        QString texture, normalMap;
        bool emissive;
        QJsonObject submaterials = v.toObject();
        QString type = submaterials["type"].toString();
        QString name = submaterials["name"].toString();
        QString baseColor = submaterials["baseColor"].toString();
        if (submaterials.contains("texture")) {
            texture = submaterials["texture"].toString();
            qDebug() << texture;
        }
        if (submaterials.contains("normalMap")) {
            normalMap = submaterials["normalMap"].toString();
        }
        if (submaterials.contains("emissive")) {
            emissive = submaterials["emissive"].toBool();
        }
    }
}
