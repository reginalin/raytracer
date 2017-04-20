#include <QCoreApplication>
#include "scene.h"

#include "ppm_example.h"
#include "ray.h"
#include "geometry.h"
#include "color.h"

Color traceAPix(int x, int y, img_t *img, Scene *scene, Camera *cam) {
    Ray ray = cam->raycast(x, y);
    QList<Intersection> intersections = QList<Intersection>();
    std::vector<Geometry> *geometryArray = &scene->geo_objs;
    for (int i = 0; i < (int) geometryArray->size(); i++) {
        Geometry *geometry = &geometryArray->at(i);
        Intersection intersection = geometry->getIntersection(ray);
        if (intersection.t != -1) intersections.append(intersection);
    }
    if (!intersections.empty()) {
        return Color(255,255,255);
    }
    else {
        return Color(0,0,0);
    }
}

void traceEachPix(img_t *img, Scene *scene, Camera *cam) {
    for (int i = 0; i < img->h * img->w; i++) {
        if (i % 20 == 0) {
            std::cout << i;
        }
        int x = i / img->w;
        int y = i % img->w;
        Color color = traceAPix(x, y, img, scene, cam);
        img->data[i].r = color.r;
        img->data[i].g = color.g;
        img->data[i].b = color.b;
    }
}

void loadJpg() {
//    QImage textureJpg
//            tex_nor_maps/154.jpg
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "parsing scene";
    Scene scene = Scene("all_shapes.json");
    Camera *cam = &scene.cam;
    img_t *img = new_img(256, 256);
    traceEachPix(img, &scene, cam);
    write_ppm(img, "output.ppm");
    destroy_img(&img);
    return a.exec();

}
