#include <QCoreApplication>

#include <QImage>
#include <QString>
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
        int x = i / img->w;
        int y = i % img->w;
        Color color = traceAPix(x, y, img, scene, cam);
        img->data[i].r = color.r;
        img->data[i].g = color.g;
        img->data[i].b = color.b;
    }
}

void loadJpg() {
    QImage *textureJpg = new QImage;
    QString file = "../../../../tex_nor_maps/154.jpg"; // or other file depending on json
    textureJpg->load(file);

    //    int nx = width of texture jpg
    //    int ny = height;

    // for each intersection
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
    Scene scene = Scene("all_shapes.json");
    Camera *cam = &scene.cam;
    img_t *img = new_img(256, 256);
    traceEachPix(img, &scene, cam);
    write_ppm(img, "output.ppm");
    destroy_img(&img);
    return a.exec();

}
