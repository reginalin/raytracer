#include "camera.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

Camera::Camera() {
    target = eye = worldUp = glm::vec4(0, 0, 0, 1);
    fov = width = height = 0;
}

Camera::Camera(glm::vec4 target, glm::vec4 eye, glm::vec3 worldUp, float fov, int width, int height) {
    this->target = target;
    this->eye = eye;
    this->worldUp = worldUp;
    this->fov = fov;
    this->width = width;
    this->height = height;
}

glm::vec4 crossVec4(glm::vec4 v1, glm::vec4 v2) {
    glm::vec3 vec1 = glm::vec3(v1[0], v1[1], v1[2]);
    glm::vec3 vec2 = glm::vec3(v2[0], v2[1], v2[2]);
    glm::vec3 res = glm::cross(vec1, vec2);
    return glm::vec4(res[0], res[1], res[2], 1);
}

Ray Camera::raycast(int x, int y) {
    //I think I did this wrong

    const int far = 1000;
    const int near = 1;
    const float PI = atan(1)*4;
    float pixX = (x + 0.5)/width;
    float pixY = (y + 0.5)/height;
    float pixNDCX = 2 * pixX - 1;
    float pixNDCY = 1 - 2 * pixY;
    glm::vec4 pixNDCCoords = glm::vec4(pixNDCX, pixNDCY, 1, 1);
    pixNDCCoords *= far;
    float aspectRatio = width/height;
    glm::mat4 projectionMat = glm::mat4(
                glm::vec4((1 / aspectRatio * std::tan(fov/2)), 0, 0, 0),
                glm::vec4(0, (1 / std::tan(fov/2)), 0, 0),
                glm::vec4(0, 0, (far / (far - near)), 1),
                glm::vec4(0, 0, ((-far * near) / (far - near), 0));

    glm::mat4 translation = glm::mat4(
                glm::vec4(1, 0, 0, 0),
                glm::vec4(0, 1, 0, 0),
                glm::vec4(0, 0, 1, 0),
                glm::vec4(-eye.x, -eye.y, -eye.z, 1)
                );
    glm::vec3 zAxis = glm::vec3((target - eye));
    glm::normalize(zAxis);
    glm::vec3 yAxis = worldUp;
    glm::normalize(yAxis);
    glm::vec3 xAxis = glm::cross(zAxis, yAxis);
    glm::normalize(xAxis);
    yAxis = glm::cross(xAxis, zAxis);
    glm::mat4 rotation = glm::mat4(
                glm::vec4(xAxis[0], yAxis[0], zAxis[0], 0),
                glm::vec4(xAxis[1], yAxis[1], zAxis[1], 0),
                glm::vec4(xAxis[2], yAxis[2], zAxis[2], 0),
                glm::vec4(0, 0, 0, 1)
                );
    glm::mat4 viewMat = rotation * translation;
    glm::mat4 transformMat = projectionMat * viewMat;
    glm::vec4 pixWorldCoords = glm::inverse(transformMat) * pixNDCCoords;
    glm::vec3 rayDir = glm::vec3(glm::normalize(pixWorldCoords - eye));
    return Ray(eye, rayDir);
}


