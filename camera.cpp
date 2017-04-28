#include "camera.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

const int far = 1000;
const int near = 1;

Camera::Camera() {
    target = eye = glm::vec4(0, 0, 0, 1);
    worldUp = glm::vec3(0, 0, 0);
    fov = width = height = 0;
}

Camera::Camera(glm::vec4 target, glm::vec4 eye, glm::vec3 worldUp, float fov, int width, int height) {
    this->target = target;
    this->eye = eye;
    this->worldUp = worldUp;
    this->fov = glm::radians(fov);
    this->width = width;
    this->height = height;
    this->aspectRatio = width/height;
    projectionMat = glm::mat4(
                glm::vec4((1 / aspectRatio * std::tan(fov/2)), 0, 0, 0),
                glm::vec4(0, (1 / std::tan(fov/2)), 0, 0),
                glm::vec4(0, 0, (far / (far - near)), 1),
                glm::vec4(0, 0, (-far * near) / (far - near), 0)
                );

    glm::mat4 translation = glm::mat4(
                glm::vec4(1, 0, 0, 0),
                glm::vec4(0, 1, 0, 0),
                glm::vec4(0, 0, 1, 0),
                glm::vec4(-eye.x, -eye.y, -eye.z, 1)
                );
    zAxis = glm::vec3((target - eye));
    zAxis = glm::normalize(zAxis);
    yAxis = worldUp;
    yAxis = glm::normalize(yAxis);
    xAxis = glm::cross(zAxis, yAxis);
    xAxis = glm::normalize(xAxis);
    yAxis = glm::cross(xAxis, zAxis);
    yAxis = glm::normalize(yAxis);
    glm::mat4 rotation = glm::mat4(
                glm::vec4(xAxis[0], yAxis[0], zAxis[0], 0),
                glm::vec4(xAxis[1], yAxis[1], zAxis[1], 0),
                glm::vec4(xAxis[2], yAxis[2], zAxis[2], 0),
                glm::vec4(0, 0, 0, 1)
                );
    viewMat = rotation * translation;
}

Ray Camera::raycast(int x, int y) {
//    float pixX = (x + 0.5)/((float)width);
//    float pixY = (y + 0.5)/((float)height);
//    float pixNDCX = 2 * pixX - 1;
//    float pixNDCY = 1 - 2 * pixY;
//    glm::vec4 pixNDCCoords = glm::vec4(pixNDCX, pixNDCY, 1, 1);
//    pixNDCCoords *= far;
//    glm::mat4 transformMat = projectionMat * viewMat;
//    glm::vec4 pixWorldCoords = glm::inverse(transformMat) * pixNDCCoords;

    // alternative way of calculating (p is the same as pixWorldCoords)
    float sx = (2 * (x / (float)width)) - 1;
    float sy = 1 - (2 * (y / (float)height));

    glm::vec4 ref = eye + glm::vec4(zAxis, 0);
    float len = glm::length(glm::vec3(ref - eye));
    float a = fov / (float)2;
    glm::vec3 V = yAxis * len * tanf(a);
    glm::vec3 H = xAxis * len * aspectRatio * tanf(a);
    glm::vec4 p = ref + glm::vec4(sx * H, 0) + glm::vec4(sy * V, 0);

    glm::vec3 rayDir = glm::normalize(glm::vec3(p - eye));
    return Ray(eye, rayDir);
}


