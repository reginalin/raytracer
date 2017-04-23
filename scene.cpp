#include "scene.h"
using namespace glm;

//changes: probably need to change constructors of geometry subclasses
// b/c we need to store more information
//right now we create geometry objects but all they contain is just the transform

Scene::Scene() { }

Scene::Scene(const char *filename) {
    std::cout << "in scene" << std::endl;
    this->geo_objs = std::vector<Geometry *>();
    QString contents;
    QString fn;
    fn.sprintf("%s", filename);

    QDir dir = QDir::currentPath();

    QString basePath =  QCoreApplication::applicationDirPath();
    //std::cout<<basePath.toStdString()<<std::endl;

//    QString path = basePath + "/../../raytracer/" + fn;
    QString path = basePath + "/../raytracer/" + fn;


   std::cout<<path.toStdString()<<std::endl;
//    std::cout<<"wanted: /raytracer/"<<fn.toStdString();

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
        const char *material = geo_obj["material"].toString().toStdString().c_str();
        const char *type = geo_obj["type"].toString().toStdString().c_str();
        const char *name = geo_obj["name"].toString().toStdString().c_str();
        glm::mat4 trans_matrix;
        glm::mat4 scale_matrix;
        std::vector<glm::mat4> rot_mats; //in case there are many rotation matrices

        //these are optiona
        if (geo_obj.contains("transform")) {
            QJsonObject transform = geo_obj["transform"].toObject();
            if (transform.contains("scale")) {
                scale = transform["scale"].toArray();
                float x = scale.at(0).toDouble();
                float y = scale.at(1).toDouble();
                float z = scale.at(2).toDouble();
                glm::vec3 scalars = glm::vec3(x, y, z);
                glm::scale(scale_matrix, scalars);
                std::cout << "scale is here" << std::endl;
            }
            if (transform.contains("rotate")) {
                std::cout << "rotate";
                rotate = transform["rotate"].toArray();
                float x = rotate.at(0).toDouble();
                float y = rotate.at(1).toDouble();
                float z = rotate.at(2).toDouble();
                if (x != 0) {
                    glm::mat4 rot;
                    glm::rotate(rot, x, glm::vec3(1, 0, 0));
                    rot_mats.push_back(rot);
                }
                if (y != 0) {
                    std::cout << " y ";
                    glm::mat4 rot;
                    glm::rotate(rot, y, glm::vec3(0, 1, 0));
                    rot_mats.push_back(rot);
                }
                if (z != 0) {
                    glm::mat4 rot;
                    glm::rotate(rot, z, glm::vec3(0, 0, 1));
                    rot_mats.push_back(rot);
                }
                std::cout << "rotate is here" << std::endl;

            }
            if (transform.contains("translate")) {
                translate = transform["translate"].toArray();
                float x = translate.at(0).toDouble();
                float y = translate.at(1).toDouble();
                float z = translate.at(2).toDouble();
                glm::vec3 scalars = glm::vec3(x, y, z);
                glm::translate(trans_matrix, scalars);
                std::cout << "translate is here" << std::endl;
            }
        }
        glm::mat4 transform_mat = glm::mat4();
        if (rot_mats.size() > 0) {
            glm::mat4 rot = rot_mats.front();
            transform_mat = scale_matrix * rot;
            while (!rot_mats.empty()) {
                rot = rot_mats.front();
                transform_mat = transform_mat * rot;
                rot_mats.erase(rot_mats.begin());
            }
        } else {
            transform_mat = scale_matrix;
        }
        transform_mat = transform_mat * trans_matrix;
        std::cout << "Trans matrix made" << std::endl;

        //create geometry object

        if (strcmp(type, "sphere") == 0) {
            object = new Sphere(transform_mat);
            this->geo_objs.push_back(object);
            std::cout << "sphere added" << std::endl;
        } else if (strcmp(type, "cube") == 0) {
            object = new Cube(transform_mat);
            this->geo_objs.push_back(object);
            std::cout << "cube added" << std::endl;
        } else if (strcmp(type, "square") == 0) {
            object = new SquarePlane(transform_mat);
            this->geo_objs.push_back(object);
            std::cout << "square added" << std::endl;
        } else if (strcmp(type, "obj") == 0) {
            //Mesh *object = new Mesh(transform);
            //geo_objs.push_back(object);
        }
        std::cout << "object made" << std::endl;
    }
    std::cout << "objects made" << std::endl;
}

void Scene::parseCamera() {
    QJsonArray target = camera["target"].toArray();
    glm::vec4 target_vec = glm::vec4(target[0].toDouble(), target[1].toDouble(), target[2].toDouble(), 0);
    QJsonArray eye = camera["eye"].toArray();
    glm::vec4 eye_vec = glm::vec4(eye[0].toDouble(), eye[1].toDouble(), eye[2].toDouble(), 0);
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
        QJsonObject submaterials = v.toObject();
        QString type = submaterials["type"].toString();
        QString name = submaterials["name"].toString();
        QString baseColor = submaterials["baseColor"].toString();
        mat.type = type.toStdString();
        mat.name = name.toStdString();
        mat.baseColor = baseColor.toStdString();
        if (submaterials.contains("texture")) {
            texture = submaterials["texture"].toString();
            mat.texture = texture.toStdString();
        }
        if (submaterials.contains("normalMap")) {
            normalMap = submaterials["normalMap"].toString();
            mat.normalMap = normalMap.toStdString();
        }
        if (submaterials.contains("emissive")) {
            emissive = submaterials["emissive"].toBool();
            mat.emissive = emissive;
        }
        if (submaterials.contains("reflective")) {
            reflective = submaterials["reflective"].toBool();
            mat.reflective = reflective;
        }

        material_types.insert(std::pair<std::string, Material>(mat.name, mat));
    }
    std::cout << "Loaded material" << std::endl;
}
