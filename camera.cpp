#include "camera.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

Camera::Camera() {
    left = right = bottom = top = near = far = eye_x = eye_y = eye_z = center_x = center_y = center_z = up_x = up_y = up_z = width = height = 0;
}

Camera::Camera(const char *filename, int width, int height) {
    FILE *f = fopen(filename, "rb");
    if(!f) {
        std::cout << filename << std::endl;
    }
    fscanf(f, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
           &left, &right,
           &bottom, &top, &near, &far,
           &eye_x, &eye_y, &eye_z,
           &center_x, &center_y, &center_z,
           &up_x, &up_y, &up_z
           );   //Help scanning file
    fclose(f);
    std::cout << "Stored" << left << ", " << right << ", " << bottom << ", " << top << ", " << near << ", " << far
         << ", " << eye_x << ", " << eye_y << ", " << eye_z
         << ", " << center_x << ", " << center_y << ", " << center_z
         << ", " << up_x << ", " << up_y << ", " << up_z << "\n";
    this->width = width;
    this->height = height;
}

ray Camera::raycast(int x, int y) {
    const float PI = atan(1)*4;
    vec4 cameraCoords = vec4(center_x, center_y, center_z, 1);
    float pixNDCX = (x + 0.5)/width;
    float pixNDCY = (y + 0.5)/height;
    float pixScreenX = 2 * pixNDCX - 1;
    float pixScreenY = 1 - 2 * pixNDCY;
    float aspectRatio = width/(float)height;
    float pixCameraX = (2 * pixScreenX - 1) * aspectRatio * tan(PI/4);
    float pixCameraY = (1 - 2 * pixScreenY) * tan(PI/4);
    vec4 pixCameraCoords = vec4(pixCameraX, pixCameraY, -1, 1);

    mat4 translation = mat4(
                vec4(1, 0, 0, 0),
                vec4(0, 1, 0, 0),
                vec4(0, 0, 1, 0),
                vec4(-eye_x, -eye_y, -eye_z, 1)
                );
    vec4 center = vec4(center_x, center_y, center_z, 0);
    vec4 eye = vec4(eye_x, eye_y, eye_z, 0);
    vec4 zAxis = (center - eye);
    vec4 yAxis = vec4(up_x, up_y, up_z, 0);
    zAxis.norm();
    yAxis.norm();
    vec4 xAxis = cross(zAxis, yAxis);
    xAxis.norm();
    mat4 rotation = mat4(
                vec4(xAxis[0], yAxis[0], zAxis[0], 0),
                vec4(xAxis[1], yAxis[1], zAxis[1], 0),
                vec4(xAxis[2], yAxis[2], zAxis[2], 0),
                vec4(0, 0, 0, 1)
                );
    mat4 transformation = rotation * translation;
    vec4 pixWorldCoords = pixCameraCoords * transformation;

    vec4 rayDir = (pixWorldCoords - pixCameraCoords).normalize();
    return ray(cameraCoords, rayDir);
}

