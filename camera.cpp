#include "camera.h"
#include <stdio.h>
#include <iostream>

Camera::Camera() {
    left = right = bottom = top = near = far = eye_x = eye_y = eye_z = center_x = center_y = center_z = up_x = up_y = up_z = 0;
}

Camera::Camera(const char *filename) {
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
}

ray Camera::raycast(int x, int y) {
    vec4 cameraCoords = vec4(center_x, center_y, center_z, 1);
    //Help position x and y as world coordinates
}

