#include "scene.h"
using namespace glm;


QString basePath;
QString path;

Scene::Scene() { }

Scene::Scene(const char *filename) {
    std::cout << "in scene" << std::endl;
    this->geo_objs = std::vector<Geometry *>();
    QString contents;
    QString fn;
    fn.sprintf("%s", filename);

    QDir dir = QDir::currentPath();

    basePath =  QCoreApplication::applicationDirPath();

    // MAKE SURE BASEPATH IS CORRECT
//    path = basePath + "/../../raytracer/" + fn;
    path = basePath + "/../raytracer/" + fn;


    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    contents = file.readAll();
    file.close();

    QJsonDocument json = QJsonDocument::fromJson(contents.toUtf8());
    QJsonObject object = json.object();
    QJsonObject scene = object.value("scene").toObject();
    camera = scene.value("camera").toObject();
    geometry = scene.value("geometry").toArray();
    material = scene.value("material").toArray();

    parseCamera();
    parseMaterial();
    parseGeometry();
}

void Scene::parseGeometry() {
    foreach (const QJsonValue & v, geometry) {
        Geometry *object;
        QJsonArray scale, rotate, translate;
        QJsonObject geo_obj = v.toObject();
        QString material = geo_obj["material"].toString();
        QString type = geo_obj["type"].toString();
        QString name = geo_obj["name"].toString();
        glm::mat4 trans_matrix = glm::mat4();
        glm::mat4 scale_matrix = glm::mat4();
        glm::mat4 rotation_mat = glm::mat4();
        std::vector<glm::mat4> rot_mats = std::vector<glm::mat4>(); //in case there are many rotation matrices
        //these are optional
        if (geo_obj.contains("transform")) {
            QJsonObject transform = geo_obj["transform"].toObject();
            if (transform.contains("scale")) {
                scale = transform["scale"].toArray();
                float x = scale.at(0).toDouble();
                float y = scale.at(1).toDouble();
                float z = scale.at(2).toDouble();
                glm::vec3 scalars = glm::vec3(x, y, z);
                scale_matrix = glm::scale(scale_matrix, scalars);
            }
            if (transform.contains("rotate")) {
                rotate = transform["rotate"].toArray();
                float x = glm::radians(rotate.at(0).toDouble());
                float y = glm::radians(rotate.at(1).toDouble());
                float z = glm::radians(rotate.at(2).toDouble());
                if (x != 0) {
                    glm::mat4 rot = glm::mat4();
                    rot = glm::rotate(rot, x, glm::vec3(1, 0, 0));
                    rot_mats.push_back(rot);
                }
                if (y != 0) {
                    glm::mat4 rot = glm::mat4();
                    rot = glm::rotate(rot, y, glm::vec3(0, 1, 0));
                    rot_mats.push_back(rot);
                }
                if (z != 0) {
                    glm::mat4 rot = glm::mat4();
                    rot = glm::rotate(rot, z, glm::vec3(0, 0, 1));
                    rot_mats.push_back(rot);
                }

            }
            if (transform.contains("translate")) {
                translate = transform["translate"].toArray();
                float x = translate.at(0).toDouble();
                float y = translate.at(1).toDouble();
                float z = translate.at(2).toDouble();
                glm::vec3 scalars = glm::vec3(x, y, z);
                trans_matrix = glm::translate(trans_matrix, scalars);
            }
        }
        for (int i = 0; i < rot_mats.size(); i++) {
            glm::mat4 rot = rot_mats.at(i);
            rotation_mat *= rot;
        }
        glm::mat4 transform_mat = trans_matrix * rotation_mat * scale_matrix;
//        std::cout << "scale " << glm::to_string(scale_matrix) << std::endl;
//        std::cout << "translate" << glm::to_string(trans_matrix) << std::endl;
//        std::cout << "transform " << glm::to_string(transform_mat) << std::endl;
//        std::cout << "Trans matrix made" << std::endl;

        if (QString::compare(type, "sphere") == 0) {
            object = new Sphere(transform_mat);
            object->name = name;
            object->material = material;
            object->mat = material_types.at(material);
            object->type = type;
            if (QString::compare(material, "emissive_material") == 0) {
                this->light = object;
            }
            this->geo_objs.push_back(object);
        } else if (QString::compare(type, "cube") == 0) {
            object = new Cube(transform_mat);
            object->name = name;
            object->material = material;
            object->mat = material_types.at(material);
            object->type = type;
            if (QString::compare(material, "emissive_material") == 0) {
                this->light = object;
            }
            this->geo_objs.push_back(object);
        } else if (QString::compare(type, "square") == 0) {
            object = new SquarePlane(transform_mat);
            object->name = name;
            object->material = material;
            object->mat = material_types.at(material);
            object->type = type;
            if (QString::compare(material, "emissive_material") == 0) {
                this->light = object;
            }
            this->geo_objs.push_back(object);
        } else if (QString::compare(type, "obj") == 0) {
            QString filename = geo_obj["filename"].toString();
            Mesh *object = new Mesh(transform_mat, filename, this->cam);
            object->name = name;
            object->material = material;
            object->mat = material_types.at(material);
            object->type = type;
            this->geo_objs.push_back(object);
            if (QString::compare(material, "emissive_material") == 0) {
                this->light = object;
            }
        }
    }
}

void Scene::parseCamera() {
    QJsonArray target = camera["target"].toArray();
    glm::vec4 target_vec = glm::vec4(target[0].toDouble(), target[1].toDouble(), target[2].toDouble(), 1);
    QJsonArray eye = camera["eye"].toArray();
    glm::vec4 eye_vec = glm::vec4(eye[0].toDouble(), eye[1].toDouble(), eye[2].toDouble(), 1);
    QJsonArray worldUp = camera["worldUp"].toArray();
    glm::vec3 up_vec = glm::vec3(worldUp[0].toDouble(), worldUp[1].toDouble(), worldUp[2].toDouble());
    float fov = camera["fov"].toDouble();
    int width = camera["width"].toInt();
    int height = camera["height"].toInt();
    cam = Camera(target_vec, eye_vec, up_vec, fov, width, height);
    std::cout << "Loaded camera";
}

void Scene::parseMaterial() {
    foreach (const QJsonValue & v, material) {
        Material mat = Material();
        QString texture, normalMap;
        bool emissive, reflective;
        float reflectivity;
        QJsonObject submaterials = v.toObject();
        QString type = submaterials["type"].toString();
        QString name = submaterials["name"].toString();
        if (submaterials.contains("baseColor")) {
            QJsonArray baseColor = submaterials["baseColor"].toArray();
            float r = (unsigned char)std::floor(baseColor.at(0).toDouble() * 255);
            float g = (unsigned char)std::floor(baseColor.at(1).toDouble() * 255);
            float b = (unsigned char)std::floor(baseColor.at(2).toDouble() * 255);
            mat.baseColor = glm::vec3(r, g, b);
        }
        else {
            mat.baseColor = glm::vec3(255, 255, 255);
        }
        mat.type = type;
        mat.name = name;
        if (submaterials.contains("texture")) {
            texture = submaterials["texture"].toString();
            mat.texture = texture;
            mat.textureImg = new QImage(texture);
        }
        else {
            mat.texture = "";
            mat.textureImg = NULL;
        }
        if (submaterials.contains("normalMap")) {
            normalMap = submaterials["normalMap"].toString();
            mat.normalMap = normalMap;
            mat.normalImg = new QImage(texture);
        }
        else {
            mat.normalMap = "";
            mat.normalImg = NULL;
        }
        if (submaterials.contains("emissive")) {
            emissive = submaterials["emissive"].toBool();
            mat.emissive = emissive;
        }
        else {
            mat.emissive = false;
        }
        if (submaterials.contains("reflectivity")) {
            reflectivity = submaterials["reflectivity"].toDouble();
            reflective = true;
            mat.reflective = reflective;
            mat.reflectivity = reflectivity;
        }
        else {
            mat.reflective = false;
            mat.reflectivity = 0;
        }

        material_types.insert(std::pair<QString, Material>(mat.name, mat));
    }
    std::cout << "Loaded material" << std::endl;
}
