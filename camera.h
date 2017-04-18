#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class Camera
{
public:
    glm::vec4 target;
    glm::vec4 eye;
    glm::vec3 worldUp;
    glm::mat4 viewMat;
    glm::mat4 projectionMat;
    float fov;
    int width;
    int height;

    Camera();
    Camera(glm::vec4 target, glm::vec4 eye, glm::vec3 worldUp, float fov, int width, int height);
    Ray raycast(int x, int y);
};

#endif // CAMERA_H
