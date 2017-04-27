#include <QCoreApplication>

#include <QImage>
#include <QImageReader>
#include <QString>
#include <QColor>
#include "scene.h"

#include "ppm_example.h"
#include "ray.h"
#include "geometry.h"
#include "color.h"
#include <iostream>

const float PI = 3.14159265358979323846264338327950288419716939937510582;

glm::vec3 texture(Intersection intersection) { // input the shape (or the intersection? and the texture file, output color? or draw directly from here
    QImage *textureJpg = intersection.geometry->mat.textureImg;
//    std::cout<<"NULL"<<textureJpg->isNull()<<std::endl;
    int nx = textureJpg->width() - 1;
    int ny = textureJpg->height() - 1;
    float u = intersection.uv[0];
    float v = intersection.uv[1];
    if (u < 0 || v < 0 || u > 1 || v > 1) {
        return glm::vec3(0,0,0);
    }
    // for each intersection

    float u1 = u * nx - floor(u * nx);
    float v1 = v * ny - floor(v * ny);

    // indices for the pixel the intersection is mapped to
    int i = (int)floor(u * (float)nx);
    int j = (int)floor(v * (float)ny);
    int i_ = i + 1;
    int j_ = j + 1;

    if (i_ >= nx) {
        i_--;
    }

    if (j_ >= ny) {
        j_--;
    }

    QColor first(textureJpg->pixel(i, j));
    QColor second(textureJpg->pixel(i_, j));
    QColor third(textureJpg->pixel(i, j_));
    QColor fourth(textureJpg->pixel(i_, j_));

    unsigned char red = (1 - u1) * (1 - v1) * first.red() + u1 * (1 - v1) * second.red()
                    + (1 - u1) * v1 * third.red() + u1 * v1 * fourth.red();

    unsigned char green = (1 - u1) * (1 - v1) * first.green() + u1 * (1 - v1) * second.green()
                    + (1 - u1) * v1 * third.green() + u1 * v1 * fourth.green();

    unsigned char blue = (1 - u1) * (1 - v1) * first.blue() + u1 * (1 - v1) * second.blue()
                    + (1 - u1) * v1 * third.blue() + u1 * v1 * fourth.blue();

    return glm::vec3(red, green, blue);
}

//assume that every scene has a single light source
//which is a sphere
//returns color
glm::vec3 lambert(Intersection intersection, Scene *scene, glm::vec3 color) {
    Geometry *light_source = scene->light;
    glm::vec4 light = light_source->transform[3];
    glm::vec3 dir = glm::normalize(glm::vec3(light - intersection.position));
    glm::vec3 norm = intersection.normal;

    float cosine = glm::dot(norm, dir);
    // float albedo;
    // albedo is color
    if (cosine == 1) {
        return glm::vec3(0, 0, 0);
    } else {
        color[0] = color[0] / M_PI * cosine;
        color[1] = color[1] / M_PI * cosine;
        color[2] = color[2] / M_PI * cosine;
    }

    return color;
}

float aoGather(Intersection intersection, int samples, float distance) {
//    glm::vec4 point = intersection.position;
//    float golden_angle = PI * (3 - std::sqrt(5));
//    for (int i = 0; i < samples; i++) {
//        float theta = golden_angle * i;
//        float z = (1 - 1 / samples) * (1 - (2 * i) / (samples - 1));
//        radius = numpy.sqrt(1 - z * z);

//        points = numpy.zeros((n, 3))
//        points[:,0] = radius * numpy.cos(theta)
//        points[:,1] = radius * numpy.sin(theta)
//        points[:,2] = z
//    }
}

glm::vec3 traceAPix(Ray ray, Scene *scene, Camera *cam, int recursions) {
//    return (ray.direction * 255.0f + 255.0f)/2.f;
    QList<Intersection> intersections = QList<Intersection>();
    std::vector<Geometry *> *geometryArray = &scene->geo_objs;
    for (int i = 0; i < (int) geometryArray->size(); i++) {
        Geometry *geometry = geometryArray->at(i);
        Intersection intersection = geometry->getIntersection(ray);
        if (intersection.t > 0 && intersection.geometry != ray.ignoreGeo) intersections.append(intersection);
    }

    glm::vec3 color = glm::vec3(0,0,0);
    if (!intersections.empty()) {
        Intersection closestIntersect = intersections[0];
        for (int i = 0; i < intersections.size(); i++) {
            if (intersections[i].t < closestIntersect.t) closestIntersect = intersections[i];
        }

        //Shading
        Geometry *hitGeo = closestIntersect.geometry;
        color = hitGeo->mat.baseColor;
        if (hitGeo->mat.texture != "" && hitGeo->mat.textureImg != NULL) {
            color = texture(closestIntersect);
        }
//        if (hitGeo->type == "sphere") color = glm::vec3(255, 0, 0);
//        if (hitGeo->type == "cube") color = glm::vec3(0, 0, 255);
//        if (hitGeo->type == "square") color = glm::vec3(0, 255, 0);
        if (hitGeo->mat.reflective && recursions < 8) {
            glm::vec3 newDir = glm::reflect(ray.direction, closestIntersect.normal);
//            return (newDir * 255.0f + 255.0f)/2.0f;
            Ray newRay = Ray(closestIntersect.position, newDir, hitGeo);
            glm::vec3 reflectColor = traceAPix(newRay, scene, cam, recursions + 1);
            color = color * (1 - hitGeo->mat.reflectivity) + reflectColor * hitGeo->mat.reflectivity;
        }
        //lambert
        if (/*QString::compare((hitGeo->mat).type, "lambert") == 0*/ 1) {
            Geometry *light_source = scene->light;
            glm::vec4 lightPos = light_source->transform[3];
            glm::vec3 dir = glm::normalize(glm::vec3(lightPos - closestIntersect.position));
            Ray lightRay = Ray(closestIntersect.position, dir, hitGeo);

            Intersection checkLit = light_source->getIntersection(lightRay);
            QList<Intersection> inter = QList<Intersection>();

            for (int k = 0; k < (int) geometryArray->size(); k++) {
                Geometry *geo = geometryArray->at(k);
                Intersection intersect = geo->getIntersection(lightRay);
                if (intersect.t > 0 && intersect.geometry != lightRay.ignoreGeo) inter.append(intersect);
            }
            if (!inter.empty()) {
                Intersection closest = inter[0];
                for (int k = 0; k < inter.size(); k++) {
                    if (inter[k].t < closest.t) closest = inter[k];
                }
                //Geometry *close = closestIntersect.geometry;
                if (closest.t < checkLit.t) {
                    color *= 0.1f;
                } else {
                    color = lambert(closestIntersect, scene, color);
                }
            } else {
                color = lambert(closestIntersect, scene, color);
            }
        }

//        color = (glm::vec3(closestIntersect.normal * 255.0f) + 255.0f)/2.0f;
//        color = (glm::vec3(closestIntersect.position/10.0f * 255.0f) + 255.0f)/2.0f;
    }
    return color;
}

void traceEachPix(img_t *img, Scene *scene, Camera *cam) {
    //get light source
//    Geometry *light;
//    std::vector<Geometry *> objs = scene->geo_objs;
//    for (int i = 0; i < objs.size(); i++) {
//        if (QString::compare(objs[i]->material, "emissive_material") == 0) {
//            light = objs[i];
//        }
//    glm::vec4 light = (scene->light)->center;

    for (int i = 0; i < img->h * img->w; i++) {
//        if (i % 20 == 0) {
//            std::cout << i;
//        }
        int x = i % img->w;
        int y = i / ((float)(img->w));
        Ray ray = cam->raycast(x, y);
        glm::vec3 color = traceAPix(ray, scene, cam, 0);
        //color = lambert(ray, scene, cam, color);
        img->data[i].r = color.r;
        img->data[i].g = color.g;
        img->data[i].b = color.b;
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "parsing scene";
    Scene scene = Scene("all_shapes.json");
    std::cout << "size " << scene.geo_objs.size() << std::endl;
    Camera *cam = &scene.cam;
    img_t *img = new_img(cam->width, cam->height);
    traceEachPix(img, &scene, cam);
    write_ppm(img, "output.ppm");
    destroy_img(&img);
    //return a.exec();
}
