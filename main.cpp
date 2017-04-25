#include <QCoreApplication>

#include <QImage>
#include <QString>
#include "scene.h"

#include "ppm_example.h"
#include "ray.h"
#include "geometry.h"
#include "color.h"
#include <iostream>

Color traceAPix(int x, int y, img_t *img, Scene *scene, Camera *cam) {
    Ray ray = cam->raycast(x, y);
    QList<Intersection> intersections = QList<Intersection>();
    std::vector<Geometry *> *geometryArray = &scene->geo_objs;
    glm::mat4 transform = glm::mat4(glm::vec4(1, 0, 0, 0),
                                    glm::vec4(0, 1, 0, 0),
                                    glm::vec4(0, 0, 1, 0),
                                    glm::vec4(0, 0.5, -30, 1));
//    Sphere sphere = Sphere(transform);
    for (int i = 0; i < (int) geometryArray->size(); i++) {
        Geometry *geometry = geometryArray->at(i);
        Intersection intersection = geometry->getIntersection(ray);
        if (intersection.t != -1) intersections.append(intersection);
    }

    if (!intersections.empty()) {
//        std::cout<<"Render an intersection"<< std::endl;

        return Color(255,255,255);
    }
    else {
        return Color(0,0,0);
    }
}

void traceEachPix(img_t *img, Scene *scene, Camera *cam) {
    for (int i = 0; i < img->h * img->w; i++) {
//        if (i % 20 == 0) {
//            std::cout << i;
//        }
        int x = i / ((float)(img->w));
        int y = i % img->h;
        Color color = traceAPix(x, y, img, scene, cam);
        img->data[i].r = color.r;
        img->data[i].g = color.g;
        img->data[i].b = color.b;
    }
}

void loadJpg() {
    QImage *textureJpg = new QImage;
    QString file = "text_nor_maps/154.JPG"; // or other file depending on json

    textureJpg->load(file);
    std::cout<<"NULL"<<textureJpg->isNull()<<std::endl;
    int nx = textureJpg->width();
    int ny = textureJpg->height();
    std::cout<<"width"<<nx<<std::endl;
    std::cout<<"height"<<ny<<std::endl;

    // for each intersection
//    float u = intersection.uv[0];
//    float v = intersection.uv[1];
//    float u1 = u * nx - floor(u * nx);
//    float v1 = v * ny - floor(v * ny);

    //    indices for the pixel the intersection is mapped to
    //    int i = (int)floor(u * nx);
    //    int j = (int)floor(v * ny);

    //    color at (u, v) = (1 - u1) * (1 - v1) * c_ij + u1 * (1 - v1) * c_(i+1)j
    //    + (1 - u1) * v1 * c_i(j+1) + u1 * v1 * c(i+1)(j+1)
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "parsing scene";
    Scene scene = Scene("sphere.json");
    std::cout << scene.geo_objs.size() << endl;
    Camera *cam = &scene.cam;
    img_t *img = new_img(cam->width, cam->height);
    traceEachPix(img, &scene, cam);
    loadJpg();
    write_ppm(img, "output.ppm");
    destroy_img(&img);
    //return a.exec();

}
