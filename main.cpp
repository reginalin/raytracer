#include <QCoreApplication>
#include "scene.h"

#include "ppm_example.h"
#include "ray.h"
#include "geometry.h"

void traceEachPix(img_t *img, Scene scene, Camera *cam) {
    for (int i = 0; i < img->h * img->w; i++) {
        int x = i / img->w;
        int y = i % img->w;
        Ray ray = cam->raycast(x, y);
        QList<Intersection> intersections = QList<Intersection>();
        std::vector<Geometry> *geometryArray = &scene.geo_objs;
        for (int i = 0; i < (int) geometryArray->size(); i++) {
            Geometry *geometry = &geometryArray->at(i);
            Intersection intersection = geometry->getIntersection(ray);
            if (intersection.t != -1) intersections.append(intersection);
        }
        if (!intersections.empty()) {
            img->data[i].r = 255;
            img->data[i].g = 255;
            img->data[i].b = 255;
        }
        else {
            img->data[i].r = 0;
            img->data[i].g = 0;
            img->data[i].b = 0;
        }
    }
}

void loadJpg() {
//    QImage textureJpg
//            tex_nor_maps/154.jpg
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Scene scene = Scene("all_shapes.json");
    Camera *cam = new Camera();  //TODO: Camera currently loads in all parameters at 0. Make camera read in json.
    img_t *img = new_img(256, 256);
    traceEachPix(img, scene, cam);
    write_ppm(img, "output.ppm");
    destroy_img(&img);
    return a.exec();

}
