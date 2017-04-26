#include "scene.h"
using namespace glm;

//changes: probably need to change constructors of geometry subclasses
// b/c we need to store more information
//right now we create geometry objects but all they contain is just the transform

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
    //std::cout<<basePath.toStdString()<<std::endl;

    path = basePath + "/../../raytracer/" + fn;
//    QString path = basePath + "/../raytracer/" + fn;


   //std::cout<<path.toStdString()<<std::endl;
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
        QString material = geo_obj["material"].toString();
        QString type = geo_obj["type"].toString();
        QString name = geo_obj["name"].toString();
        glm::mat4 trans_matrix = glm::mat4();
        glm::mat4 scale_matrix = glm::mat4();
        glm::mat4 rotation_mat = glm::mat4();
        std::vector<glm::mat4> rot_mats; //in case there are many rotation matrices

        //these are optional
        if (geo_obj.contains("transform")) {
            QJsonObject transform = geo_obj["transform"].toObject();
            if (transform.contains("scale")) {
                scale = transform["scale"].toArray();
                float x = scale.at(0).toDouble();
                float y = scale.at(1).toDouble();
                float z = scale.at(2).toDouble();
                glm::vec3 scalars = glm::vec3(x, y, z);
//                std::cout << "scale " << glm::to_string(scale_matrix) << std::endl;
                scale_matrix = glm::scale(scale_matrix, scalars);
//                std::cout << "scale " << glm::to_string(scale_matrix) << std::endl;
//                std::cout << "scale is here" << std::endl;
            }
            if (transform.contains("rotate")) {
//                std::cout << "rotate";
                rotate = transform["rotate"].toArray();
                float x = rotate.at(0).toDouble();
                float y = rotate.at(1).toDouble();
                float z = rotate.at(2).toDouble();
                if (x != 0) {
                    glm::mat4 rot;
                    rot = glm::rotate(rot, x, glm::vec3(1, 0, 0));
                    rot_mats.push_back(rot);
                }
                if (y != 0) {
                    std::cout << " y ";
                    glm::mat4 rot;
                    rot = glm::rotate(rot, y, glm::vec3(0, 1, 0));
                    rot_mats.push_back(rot);
                }
                if (z != 0) {
                    glm::mat4 rot;
                    rot = glm::rotate(rot, z, glm::vec3(0, 0, 1));
                    rot_mats.push_back(rot);
                }
//                std::cout << "rotate is here" << std::endl;

            }
            if (transform.contains("translate")) {
                translate = transform["translate"].toArray();
                float x = translate.at(0).toDouble();
                float y = translate.at(1).toDouble();
                float z = translate.at(2).toDouble();
                glm::vec3 scalars = glm::vec3(x, y, z);
                trans_matrix = glm::translate(trans_matrix, scalars);
                std::cout << "translate" << glm::to_string(trans_matrix) << std::endl;
            }
        }
        while (!rot_mats.empty()) {
            glm::mat4 rot = rot_mats.front();
            rotation_mat *= rot;
            rot_mats.erase(rot_mats.begin());
        }
        glm::mat4 transform_mat = trans_matrix * rotation_mat * scale_matrix;
        std::cout << "scale " << glm::to_string(scale_matrix) << std::endl;
        std::cout << "translate" << glm::to_string(trans_matrix) << std::endl;
        std::cout << "transform " << glm::to_string(transform_mat) << std::endl;
        std::cout << "Trans matrix made" << std::endl;

        //create geometry object

        if (QString::compare(type, "sphere") == 0) {
            object = new Sphere(transform_mat);
            object->name = name;
            object->material = material;
            object->mat = material_types.at(material);
            object->type = type;
            this->geo_objs.push_back(object);
        } else if (QString::compare(type, "cube") == 0) {
            object = new Cube(transform_mat);
            object->name = name;
            object->material = material;
            object->mat = material_types.at(material);
            object->type = type;
            this->geo_objs.push_back(object);
            std::cout << "cube added" << std::endl;
        } else if (QString::compare(type, "square") == 0) {
            object = new SquarePlane(transform_mat);
            object->name = name;
            object->material = material;
            object->mat = material_types.at(material);
            object->type = type;
            this->geo_objs.push_back(object);
            std::cout << "square added" << std::endl;
        } else if (QString::compare(type, "obj") == 0) {
            //Mesh *object = new Mesh(transform);
            //geo_objs.push_back(object);
        }
        std::cout << "object made" << std::endl;
    }
    std::cout << "objects made" << std::endl;
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
            unsigned char r = (unsigned char)std::floor(baseColor.at(0).toDouble() * 255);
            unsigned char g = (unsigned char)std::floor(baseColor.at(1).toDouble() * 255);
            unsigned char b = (unsigned char)std::floor(baseColor.at(2).toDouble() * 255);
            mat.baseColor = Color(r, g, b);
        }
        else {
            mat.baseColor = Color(255, 255, 255);
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
        }
        else {
            mat.reflective = false;
        }

        material_types.insert(std::pair<QString, Material>(mat.name, mat));
    }
    std::cout << "Loaded material" << std::endl;
}
