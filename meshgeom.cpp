#include "meshgeom.h"
#include "triangle.h"
#include "camera.h"
#include <math.h>

float currT;

struct face {
    glm::vec4 vertex1;
    glm::vec4 vertex2;
    glm::vec4 vertex3;
};


std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

std::vector<glm::vec4> normVecs;
std::vector<face> faceVecs;
std::vector<float> indices;
std::vector<glm::vec4> transNormVecs;

Mesh::Mesh(glm::mat4 transformMatrix, char* inputFile) {
    transform = transformMatrix;
    tinyobj::LoadObj(shapes, materials, inputFile);

    // positions: vector of vertex coordinates
    std::vector<float> positions;
    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        for (int j = 0; j < static_cast<int>(shapes[i].mesh.positions.size()); j++) {
            positions.push_back(shapes[i].mesh.positions[j]);
        }
    }

    // vector of vec4s, with w coordinate of 1
    std::vector<glm::vec4> posVecs;
    for (int i = 0; i < static_cast<int>(positions.size()); i+=3) {
        glm::vec4 here = glm::vec4(positions[i], positions[i+1], positions[i+2], 1);
        posVecs.push_back(here);
    }

    // normals; vector of normal coordinates
    std::vector<float> normals;

    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        for (int j = 0; j < static_cast<int>(shapes[i].mesh.normals.size()); j++) {
            normals.push_back(shapes[i].mesh.normals[j]);
        }
    }

    // vector of vec4s, with w coordinate of 0
    for (int i = 0; i < static_cast<int>(normals.size()); i+=3) {
        glm::vec4 here = glm::vec4(normals[i], normals[i+1], normals[i+2], 0);
        normVecs.push_back(here);
    }

    for (int i = 0; i < static_cast<int>(normVecs.size()); i++) {
        glm::vec4 update = Camera::rotation * normVecs[i];
        transNormVecs.push_back(update);
    }

    // indices: vector of vertex indices that specify vertices of triangles
    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        for (int j = 0; j < static_cast<int>(shapes[i].mesh.indices.size()); j++) {
            indices.push_back(shapes[i].mesh.indices[j]);
        }
    }

    // vector of faces
    for (int i = 0; i < static_cast<int>(indices.size()); i+=3) {
        face here;
        here.vertex1 = posVecs[indices[i]];
        here.vertex2 = posVecs[indices[i+1]];
        here.vertex3 = posVecs[indices[i+2]];
        faceVecs.push_back(here);
    }

    for (int i = 0; i < static_cast<int>(faceVecs.size()); i++) {
        glm::vec4 first = Camera::projectionMat * Camera::viewMat * faceVecs[i].vertex1;
        float w1 = first[3];
        faceVecs[i].vertex1 = first/w1;

        glm::vec4 second = Camera::projectionMat * Camera::viewMat * faceVecs[i].vertex2;
        float w2 = second[3];
        faceVecs[i].vertex2 = second/w2;

        glm::vec4 third = Camera::projectionMat * Camera::viewMat * faceVecs[i].vertex3;
        float w3 = third[3];
        faceVecs[i].vertex3 = third/w3;
    }

    for (int i = 0; i < static_cast<int>(faceVecs.size()); i++) {
        // convert from NDC to pixel coordinates
        // ^ check if these are the coordinates we're using
        glm::vec4 first = faceVecs[i].vertex1;
        float x1 = (first[0] + 1) * (Camera::width/2);
        float y1 = (1 - first[1]) * (Camera::height/2);
        float z1 = first[2];
        float w1 = first[3];
        faceVecs[i].vertex1 = glm::vec4(x1, y1, z1, w1);

        glm::vec4 second = faceVecs[i].vertex2;
        float x2 = (second[0] + 1) * (Camera::width/2);
        float y2 = (1 - second[1]) * (Camera::height/2);
        float z2 = second[2];
        float w2 = second[3];
        faceVecs[i].vertex2 = glm::vec4(x2, y2, z2, w2);

        glm::vec4 third = faceVecs[i].vertex3;
        float x3 = (third[0] + 1) * (Camera::width/2);
        float y3 = (1 - third[1]) * (Camera::height/2);
        float z3 = third[2];
        float w3 = third[3];
        faceVecs[i].vertex3 = glm::vec4(x3, y3, z3, w3);

    }
}

Intersection Mesh::getIntersection(Ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);


    glm::vec3 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;

    // initializing it as the first one
    Triangle closest = Triangle(faceVecs[1].vertex1, faceVecs[1].vertex2, faceVecs[1].vertex3, transform);
    float currT;

    for (int i = 0; i < static_cast<int>(faceVecs.size()); i++) {
        Triangle current = Triangle(faceVecs[i].vertex1, faceVecs[i].vertex2, faceVecs[i].vertex3, transform);
        triangles.push_back(current);
        float t = current.getIntersection(input).t;
        if (t < currT && t > 0) {
            currT = t;
            closest = current;
        }
    }

//    glm::vec4 temp = orig + t * dir;
//    glm::vec4 point = temp * transform; // left or right multiply?

//    glm::vec4 normal = temp * transpose(inverted);

    return closest.getIntersection(input);

}



