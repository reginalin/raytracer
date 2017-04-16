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

    parseCamera();
    parseMaterial();
    parseGeometry();
}

void Scene::parseGeometry() {
    foreach (const QJsonValue & v, geometry) {
        Geometry object;
        QJsonArray scale, rotate, translate;
        QJsonObject geo_obj = v.toObject();
        const char *material = geo_obj["material"].toString();
        const char *type = geo_obj["type"].toString();
        const char *name = geo_obj["name"].toString();
//        QJsonValue material = geo_obj["material"];
//        QJsonValue type = geo_obj["type"];
//        QJsonValue name = geo_obj["name"];
        glm::mat4 trans_matrix;
        glm::mat4 scale_matrix;
        std::vector<glm::mat4> rot_mats; //in case there are many rotation matrices

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
                if (x != 0) {
                    glm::mat4 rot;
                    glm::rotate(rot, x, 1.0, 0.0, 0.0);
                    rot_mats.push_back(rot);
                }
                if (y != 0) {
                    glm::mat4 rot;
                    glm::rotate(rot, y, 0.0, 1.0, 0.0);
                    rot_mats.push_back(rot);
                }
                if (z != 0) {
                    glm::mat4 rot;
                    glm::rotate(rot, z, 0.0, 0.0, 1.0);
                    rot_mats.push_back(rot);
                }

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
        glm::mat4 transform;
        glm::mat4 rot = rot_mats.pop_back();
        transform = scale_matrix * rot;
        while (!rot_mats.empty()) {
            rot = rot_mats.pop_back();
            transform = transform * rot;
        }
        transform = transform * trans_matrix;

        //create geometry object
        if (type.compare("sphere") == 0) {
            obj = new Sphere(transform);
        } else if (type.compare("cube") == 0) {
            obj = new Cube(transform);
        } else if (type.compare("square") == 0) {
            obj = new SquarePlane(transform);
        } else if (type.compare("obj") == 0) {
            obj = new Mesh(transform);
        }

    geo_objs.push_back(object);
    }
}

//rotation matrix
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
    glm::vec4 target_vec = glm::vec4(target[0], target[1], target[2], 0);
    QJsonArray eye = camera["eye"].toArray();
    glm::vec4 eye_vec = glm::vec4(eye[0], eye[1], eye[2], 0);
    QJsonArray worldUp = camera["worldUp"].toArray();
    glm::vec4 up_vec = glm::vec4(worldUp[0], worldUp[1], worldUp[2], 0);
    float fov = (float) camera["fov"].toDouble();
    int width = camera["width"].toInt();
    int height = camera["height"].toInt();
    cam = new Camera(target_vec, eye_vec, up_vec, fov, width, height);
}

void Scene::parseMaterial() {
    foreach (const QJsonValue & v, material) {
        Material mat = Material();
        QString texture, normalMap;
        bool emissive;
        QJsonObject submaterials = v.toObject();
        QString type = submaterials["type"].toString();
        QString name = submaterials["name"].toString();
        QString baseColor = submaterials["baseColor"].toString();
        mat.type = type;
        mat.name = name;
        mat.baseColor = baseColor;
        if (submaterials.contains("texture")) {
            texture = submaterials["texture"].toString();
            mat.texture = texture;
        }
        if (submaterials.contains("normalMap")) {
            normalMap = submaterials["normalMap"].toString();
            mat.normalMap = normalMap;
        }
        if (submaterials.contains("emissive")) {
            emissive = submaterials["emissive"].toBool();
            mat.emissive = emissive;
        }
        if (submaterials.contains("reflective")) {
            reflective = submaterials["reflective"].toBool();
            mat.reflective = reflective;
        }
        material_types.insert( std::pair<std::string, Material>(name, mat));
    }
}
