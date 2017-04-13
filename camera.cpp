#include "camera.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

Camera::Camera() {
    left = right = bottom = top = near = far = eye_x = eye_y = eye_z = center_x = center_y = center_z = up_x = up_y = up_z = width = height = 0;
}

Camera::Camera(float left, float right, float bottom, float top, float near, float far, float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z, int width, int height) {
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->near = near;
    this->far = far;
    this->eye_x = eye_x;
    this->eye_y = eye_y;
    this->eye_z = eye_z;
    this->center_x = center_x;
    this->center_y = center_y;
    this->center_z = center_z;
    this->up_x = up_x;
    this->up_y = up_y;
    this->up_z = up_z;

    std::cout << "Stored" << left << ", " << right << ", " << bottom << ", " << top << ", " << near << ", " << far
         << ", " << eye_x << ", " << eye_y << ", " << eye_z
         << ", " << center_x << ", " << center_y << ", " << center_z
         << ", " << up_x << ", " << up_y << ", " << up_z << "\n";
    this->width = width;
    this->height = height;
}

glm::vec4 crossVec4(glm::vec4 _v1, glm::vec4 _v2){
    glm::vec3 vec1 = glm::vec3(_v1[0], _v1[1], _v1[2]);
    glm::vec3 vec2 = glm::vec3(_v2[0], _v2[1], _v2[2]);
    glm::vec3 res = glm::cross(vec1, vec2);
    return glm::vec4(res[0], res[1], res[2], 1);
}

Ray Camera::raycast(int x, int y) {
    //I think I did this wrong

    const float PI = atan(1)*4;
    glm::vec4 cameraCoords = glm::vec4(center_x, center_y, center_z, 1);
    float pixNDCX = (x + 0.5)/width;
    float pixNDCY = (y + 0.5)/height;
    float pixScreenX = 2 * pixNDCX - 1;
    float pixScreenY = 1 - 2 * pixNDCY;
    float aspectRatio = width/(float)height;
    float pixCameraX = (2 * pixScreenX - 1) * aspectRatio * tan(PI/4);
    float pixCameraY = (1 - 2 * pixScreenY) * tan(PI/4);
    glm::vec4 pixCameraCoords = glm::vec4(pixCameraX, pixCameraY, -1, 1);

    glm::mat4 translation = glm::mat4(
                glm::vec4(1, 0, 0, 0),
                glm::vec4(0, 1, 0, 0),
                glm::vec4(0, 0, 1, 0),
                glm::vec4(-eye_x, -eye_y, -eye_z, 1)
                );
    glm::vec4 center = glm::vec4(center_x, center_y, center_z, 0);
    glm::vec4 eye = glm::vec4(eye_x, eye_y, eye_z, 0);
    glm::vec4 zAxis = (center - eye);
    glm::vec4 yAxis = glm::vec4(up_x, up_y, up_z, 0);
    glm::normalize(zAxis);
    glm::normalize(yAxis);
    glm::vec4 xAxis = crossVec4(zAxis, yAxis);
    glm::normalize(xAxis);
    glm::mat4 rotation = glm::mat4(
                glm::vec4(xAxis[0], yAxis[0], zAxis[0], 0),
                glm::vec4(xAxis[1], yAxis[1], zAxis[1], 0),
                glm::vec4(xAxis[2], yAxis[2], zAxis[2], 0),
                glm::vec4(0, 0, 0, 1)
                );
    glm::mat4 transformation = rotation * translation;
    glm::vec4 pixWorldCoords = pixCameraCoords * transformation;

    glm::vec4 rayDir = glm::normalize(pixWorldCoords - pixCameraCoords);
    return Ray(cameraCoords, rayDir);
}


