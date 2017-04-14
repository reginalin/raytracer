#include "scene.h"
using namespace glm;

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
        glm::mat4 trans_matrix;
        glm::mat4 scale_matrix;
        glm::mat4 rot_matrix;

        //these are optional
        if (geo_obj.contains("transform")) {
            QJsonObject transform = geo_obj["transform"].toObject();
            if (transform.contains("scale")) {
                scale = transform["scale"].toArray();
                float x = (float) scale.at(0).toDouble();
                float y = (float) scale.at(1).toDouble();
                float z = (float) scale.at(2).toDouble();
                glm::vec3 scalars = glm::vec3(x, y, z);
                glm::scale(scale_matrix, scalars);
            }
            if (transform.contains("rotate")) {
                rotate = transform["rotate"].toArray();
                float x = (float) rotate.at(0).toDouble();
                float y = (float) rotate.at(1).toDouble();
                float z = (float) rotate.at(2).toDouble();

                //glm::mat4 rot_mat = rotation(x, y, z);
            }
            if (transform.contains("translate")) {
                translate = transform["translate"].toArray();
                float x = (float) translate.at(0).toDouble();
                float y = (float) translate.at(1).toDouble();
                float z = (float) translate.at(2).toDouble();
                glm::vec3 scalars = glm::vec3(x, y, z);
                glm::translate(trans_matrix, scalars);
            }
        }

        //create geometry object
    }
}

//rotation martrix
glm::mat4 Scene::rotation(float x, float y, float z) {
    glm::vec4 x0 = glm::vec4(1, 0, 0, 0);
    glm::vec4 x1 = glm::vec4(0, cos(x), sin(x), 0);
    glm::vec4 x2 = glm::vec4(0, -sin(x), cos(x), 0);
    glm::vec4 col3 = glm::vec4(0, 0, 0, 1);
    glm::mat4 rx = glm::mat4(x0, x1, x2, col3);

    glm::vec4 y0 = glm::vec4(cos(y), 0, -sin(y), 0);
    glm::vec4 y1 = glm::vec4(0, 1, 0, 0);
    glm::vec4 y2 = glm::vec4(sin(y), 0, cos(y), 0);
    glm::mat4 ry = glm::mat4(y0, y1, y2, col3);

    glm::vec4 z0 = glm::vec4(cos(z), sin(z), 0, 0);
    glm::vec4 z1 = glm::vec4(-sin(z), cos(z), 0, 0);
    glm::vec4 z2 = glm::vec4(0, 0, 1, 0);
    glm::mat4 rz = glm::mat4(z0, z1, z2, col3);

//    std::cout << rx * ry * rz;
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
