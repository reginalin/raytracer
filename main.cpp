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
        QJsonArray *geometryArray = &scene.geometry;
        for (int i = 0; i < geometryArray->size(); i++) {
            Geometry *geometry = geometryArray->at(i).;
            Intersection intersection = geometry->getIntersection(ray);
            if (intersection != NULL) intersections.append(intersection);
        }
        if (!intersections.empty()) img->data[i] = pixel_struct(255, 255, 255);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Scene scene = Scene("all_shapes.json");
    Camera *cam = new Camera();  //TODO: Camera currently loads in all parameters at 0. Make camera read in json.
    img_t *img = new_img(256, 256);
    traceEachPix(img, scene, cam);
    //return a.exec();

}
