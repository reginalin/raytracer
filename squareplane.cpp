#include "squareplane.h"
#include <math.h>
#include <QColor>

SquarePlane::SquarePlane(glm::mat4 transformMatrix) {
    transform = transformMatrix;
}


Intersection SquarePlane::getIntersection(Ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);

    glm::vec4 planePoint = glm::vec4(0, 0, 0, 0);
    glm::vec3 planeNormal = glm::vec3(0, 0, 1);
    glm::vec3 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;

    float t = glm::dot(planeNormal, glm::vec3(planePoint - orig)) / glm::dot(planeNormal, dir);
    glm::vec4 temp = orig + glm::vec4(t * dir, 0);

    if (temp[0] < -0.5 || temp[0] > 0.5 || temp[1] < -0.5 || temp[1] > 0.5) {
        return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
    }

    glm::vec4 point = transform * temp;

    // texture mapping- change span from [-0.5, 0.5] to [0.0, 1.0]
    float u = temp[0] + 0.5;
    float v = temp[1] + 0.5;
    glm::vec2 uv = glm::vec2(u, v);


    // normal mapping
    glm::vec4 normal = glm::vec4(0, 0, 1, 0);
    normal = glm::normalize(normal);

    glm::vec3 normal1 = glm::normalize(glm::vec3(glm::transpose(inverted) * normal));


    if (this->mat.normalMap != "") {

        QImage *normalImg = this->mat.normalImg;
        int nx = normalImg->width() - 1;
        int ny = normalImg->height() - 1;

        float u1 = u * nx - floor(u * nx);
        float v1 = v * ny - floor(v * ny);

        // indices for the pixel the intersection is mapped to
        int i = (int)floor(u * (float)nx);
        int j = (int)floor(v * (float)ny);

        if (i + 1 >= nx) {
            i--;
        }

        if (j + 1 >= ny) {
            j--;
        }

        QColor first(normalImg->pixel(i, j));
        QColor second(normalImg->pixel(i + 1, j));
        QColor third(normalImg->pixel(i, j + 1));
        QColor fourth(normalImg->pixel(i + 1, j + 1));

        unsigned char red = (1 - u1) * (1 - v1) * first.red() + u1 * (1 - v1) * second.red()
                        + (1 - u1) * v1 * third.red() + u1 * v1 * fourth.red();

        unsigned char green = (1 - u1) * (1 - v1) * first.green() + u1 * (1 - v1) * second.green()
                        + (1 - u1) * v1 * third.green() + u1 * v1 * fourth.green();

        unsigned char blue = (1 - u1) * (1 - v1) * first.blue() + u1 * (1 - v1) * second.blue()
                        + (1 - u1) * v1 * third.blue() + u1 * v1 * fourth.blue();

        float x = ((red / 255.f) * 2.f) - 1.f;
        float y = ((green / 255.f) * 2.f) - 1.f;
        float z = ((blue / 255.f) * 2.f) - 1.f;

        glm::vec3 rgb = glm::vec3(x, y, z);

        // calculate orientation matrix
        glm::vec3 tangent = glm::cross(glm::vec3(0, 1, 0), normal1);
        tangent = glm::normalize(tangent);

        glm::vec3 bitangent = glm::cross(normal1, tangent);
        bitangent = glm::normalize(bitangent);

        glm::mat4 matrix = glm::mat4(glm::vec4(tangent[0], tangent[1], tangent[2], 0),
                                     glm::vec4(bitangent[0], bitangent[1], bitangent[2], 0),
                                     glm::vec4(normal1[0], normal1[1], normal1[2], 0),
                                     glm::vec4(0, 0, 0, 1));

        glm::vec3 newNormal = glm::normalize(glm::vec3(matrix * glm::vec4(rgb, 0)));

        return Intersection(point, newNormal, uv, t, this);
    } else {
        return Intersection(point, normal1, uv, t, this);
    }

}
