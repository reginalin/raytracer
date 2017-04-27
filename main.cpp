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

float PI = 3.141592653589793238462643383279502884;

glm::vec3 texture(Intersection intersection) { // input the shape (or the intersection? and the texture file, output color? or draw directly from here
    QImage *textureJpg = intersection.geometry->mat.textureImg;
//    std::cout<<"NULL"<<textureJpg->isNull()<<std::endl;
    int nx = textureJpg->width() - 1;
    int ny = textureJpg->height() - 1;
    float u = intersection.uv[0];
    float v = intersection.uv[1];
    if (u < 0 || v < 0 || u > 1 || v > 1) {
        int i = 0;
        return glm::vec3(0,0,0);
    }
        // for each intersection

        float u1 = u * nx - floor(u * nx);
        float v1 = v * ny - floor(v * ny);

//        std::cout << "(" << nx << ", " << ny << ")" << std::endl;

        // indices for the pixel the intersection is mapped to
        int i = (int)floor(u * (float)nx);
        int j = (int)floor(v * (float)ny);

        QColor first(textureJpg->pixel(i, j));
        QColor second(textureJpg->pixel(i + 1, j));
        QColor third(textureJpg->pixel(i, j + 1));
        QColor fourth(textureJpg->pixel(i + 1, j + 1));

        unsigned char red = (1 - u1) * (1 - v1) * first.red() + u1 * (1 - v1) * second.red()
                        + (1 - u1) * v1 * third.red() + u1 * v1 * fourth.red();

        unsigned char green = (1 - u1) * (1 - v1) * first.green() + u1 * (1 - v1) * second.green()
                        + (1 - u1) * v1 * third.green() + u1 * v1 * fourth.green();

        unsigned char blue = (1 - u1) * (1 - v1) * first.blue() + u1 * (1 - v1) * second.blue()
                        + (1 - u1) * v1 * third.blue() + u1 * v1 * fourth.blue();

        return glm::vec3(red, green, blue);
//    } else {
//        return Color(0, 0, 0);

// JUST NOTES BELOW
//    color at (u, v) = (1 - u1) * (1 - v1) * c_ij + u1 * (1 - v1) * c_(i+1)j
//    + (1 - u1) * v1 * c_i(j+1) + u1 * v1 * c(i+1)(j+1)


//    for ( int row = 1; row < img.height() + 1; ++row )
//    for ( int col = 1; col < img.width() + 1; ++col )
//    QColor clrCurrent(textureJpg->pixel(200,200)); // row, color

//    std::cout /*<< "Pixel at [" << row << "," << col << "] contains color ("*/
//             << clrCurrent.red() << ", "
//             << clrCurrent.green() << ", "
//             << clrCurrent.blue() << ", "
//             << clrCurrent.alpha()
//             << std::endl;

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
            Ray newRay = Ray(closestIntersect.position, newDir, closestIntersect.geometry);
            glm::vec3 reflectColor = traceAPix(newRay, scene, cam, recursions + 1);
            color = color * (1 - hitGeo->mat.reflectivity) + reflectColor * hitGeo->mat.reflectivity;
        }
        //lambert
        //if emissive

//        color = (glm::vec3(closestIntersect.normal * 255.0f) + 255.0f)/2.0f;
//        color = (glm::vec3(closestIntersect.position/10.0f * 255.0f) + 255.0f)/2.0f;
    }
    return color;
}

//assume that every scene has a single light source
//which is a sphere
void lambert(Intersection intersection, Scene *scene, Camera *cam, glm::vec3 color) {
//    Sphere *light;
//    std::vector<Geometry *> lambert_objs = std::vector<Geometry *>();
//    std::vector<Geometry *> objs = scene->geo_objs;
//    for (int i = 0; i < objs.size(); i++) {
//        if (QString::compare(objs[i]->material, "emissive_material") == 0) {
//            light = objs[i];
//        }
//        if ( QString::compare(objs[i]->type, "lambert") == 0 ) {
//            lambert_objs.push_back(objs[i]);
//            Intersection intersection = objs[i]->getIntersection(ray);
//            glm::vec4 light = (scene->light)->center;
//            glm::vec3 norm = intersection.normal;
//            glm::vec4 l = light - intersection.position;
//            //float angle = cos(glm::dot(norm, l));
//        }
//    }
}

void traceEachPix(img_t *img, Scene *scene, Camera *cam) {
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
    Scene scene = Scene("transparent_containing_objects.json");
    std::cout << "size " << scene.geo_objs.size() << std::endl;
    Camera *cam = &scene.cam;
    img_t *img = new_img(cam->width, cam->height);
    traceEachPix(img, &scene, cam);
    write_ppm(img, "output.ppm");
    destroy_img(&img);
    //return a.exec();
}
