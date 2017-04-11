#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class Camera
{
public:
    float left;
    float right;
    float top;
    float bottom;

    float near;
    float far;

    float eye_x;
    float eye_y;
    float eye_z;

    float center_x;
    float center_y;
    float center_z;

    float up_x;
    float up_y;
    float up_z;

    int width;
    int height;

    Camera();
    Camera(const char *filename, int width, int height);
    ray raycast(int x, int y);
};

#endif // CAMERA_H
