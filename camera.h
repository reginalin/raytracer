#ifndef CAMERA_H
#define CAMERA_H


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

    Camera();
    Camera(const char *filename);
};

#endif // CAMERA_H
