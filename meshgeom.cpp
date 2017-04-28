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

struct uvCoors {
    glm::vec2 vertex1;
    glm::vec2 vertex2;
    glm::vec2 vertex3;
};


std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

std::vector<glm::vec4> normVecs;
std::vector<face> faceVecs;
std::vector<float> indices;
//std::vector<glm::vec4> transNormVecs;
std::vector<glm::vec2> uv;
std::vector<uvCoors> uvVecs;

Mesh::Mesh(glm::mat4 transformMatrix, QString inputFile, Camera cam) {
    transform = transformMatrix;
    const char * fn = inputFile.toStdString().c_str();
    tinyobj::LoadObj(shapes, materials, fn);
    this->cam = cam;


    // positions: vector of vertex coordinates
    std::vector<float> positions = std::vector<float>();

    // positions: vector of vertex coordinates
    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        for (int j = 0; j < static_cast<int>(shapes[i].mesh.positions.size()); j++) {
            positions.push_back(shapes[i].mesh.positions[j]);
        }
    }

    // vector of vec4s, with w coordinate of 1
    std::vector<glm::vec4> posVecs = std::vector<glm::vec4>();
    for (int i = 0; i < static_cast<int>(positions.size()); i+=3) {
        glm::vec4 here = glm::vec4(positions[i], positions[i+1], positions[i+2], 1);
        posVecs.push_back(here);
    }

    // normals; vector of normal coordinates
    std::vector<float> normals = std::vector<float>();
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

//    for (int i = 0; i < static_cast<int>(normVecs.size()); i++) {
//        glm::vec4 update = cam.viewMat * normVecs[i];
//        transNormVecs.push_back(update);
//    }

    // texcoords: vector of u and vs
    std::vector<float> texcoords = std::vector<float>();
    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        for (int j = 0; j < static_cast<int>(shapes[i].mesh.texcoords.size()); j++) {
            texcoords.push_back(shapes[i].mesh.texcoords[j]);
        }
    }

    // vector of vec2s for u and v
    for (int i = 0; i < static_cast<int>(texcoords.size()); i+=2) {
        glm::vec2 here = glm::vec2(texcoords[i], texcoords[i+1]);
        uv.push_back(here);
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

    // vector of uvCoors
    for (int i = 0; i < static_cast<int>(indices.size()); i+=3) {
        uvCoors here;
        here.vertex1 = uv[indices[i]];
        here.vertex2 = uv[indices[i+1]];
        here.vertex3 = uv[indices[i+2]];
        uvVecs.push_back(here);
    }

//    std::vector<float> &texcoords = shapes[0].mesh.texcoords;
    //                    float u0 = texcoords[indices[j] * 2];
        //                float v0 = texcoords[indices[j] * 2 + 1];
        //                float u1 = texcoords[indices[j + 1] * 2];
        //                float v1 = texcoords[indices[j + 1] * 2 + 1];
        //                float u2 = texcoords[indices[j + 2] * 2];
        //                float v2 = texcoords[indices[j + 2] * 2 + 1];

//    for (int i = 0; i < static_cast<int>(faceVecs.size()); i++) {
//        glm::vec4 first = cam.projectionMat * cam.viewMat * faceVecs[i].vertex1;
//        float w1 = first[3];
//        faceVecs[i].vertex1 = first/w1;

//        glm::vec4 second = cam.projectionMat * cam.viewMat * faceVecs[i].vertex2;
//        float w2 = second[3];
//        faceVecs[i].vertex2 = second/w2;

//        glm::vec4 third = cam.projectionMat * cam.viewMat * faceVecs[i].vertex3;
//        float w3 = third[3];
//        faceVecs[i].vertex3 = third/w3;
//    }

//    for (int i = 0; i < static_cast<int>(faceVecs.size()); i++) {
//        // convert from NDC to pixel coordinates
//        glm::vec4 first = faceVecs[i].vertex1;
//        float x1 = (first[0] + 1) * (cam.width/2);
//        float y1 = (1 - first[1]) * (cam.height/2);
//        float z1 = first[2];
//        float w1 = first[3];
//        faceVecs[i].vertex1 = glm::vec4(x1, y1, z1, w1);

//        glm::vec4 second = faceVecs[i].vertex2;
//        float x2 = (second[0] + 1) * (cam.width/2);
//        float y2 = (1 - second[1]) * (cam.height/2);
//        float z2 = second[2];
//        float w2 = second[3];
//        faceVecs[i].vertex2 = glm::vec4(x2, y2, z2, w2);

//        glm::vec4 third = faceVecs[i].vertex3;
//        float x3 = (third[0] + 1) * (cam.width/2);
//        float y3 = (1 - third[1]) * (cam.height/2);
//        float z3 = third[2];
//        float w3 = third[3];
//        faceVecs[i].vertex3 = glm::vec4(x3, y3, z3, w3);
//    }
}

//Mesh::Mesh(glm::mat4 transformMatrix, QString inputFile, Camera cam) {
//    this->transform = transformMatrix;
//    std::vector<tinyobj::shape_t> shapes;
//    std::vector<tinyobj::material_t> materials;
//    tinyobj::LoadObj(shapes, materials, inputOBJ, NULL);

//    //Store positions as vector of "face"s
//    std::vector<face> triangles = vector<face>();
//    for (int i = 0; i < (int)shapes.size(); i++) {
//        std::vector<unsigned int> &indices = shapes[i].mesh.indices;
//        std::vector<float> &positions = shapes[i].mesh.positions;
//        std::vector<float> &normals = shapes[i].mesh.normals;
//        std::vector<int> &material_ids = shapes[i].mesh.material_ids;
//        std::vector<float> &texcoords = shapes[i].mesh.texcoords;
//        for (int j = 0; j < (int)indices.size(); j += 3) {
//            vec4 pos0 = vec4(positions[indices[j] * 3], positions[indices[j] * 3 + 1], positions[indices[j] * 3 + 2], 1);
//            vec4 pos1 = vec4(positions[indices[j + 1] * 3], positions[indices[j + 1] * 3 + 1], positions[indices[j + 1] * 3 + 2], 1);
//            vec4 pos2 = vec4(positions[indices[j + 2] * 3], positions[indices[j + 2] * 3 + 1], positions[indices[j + 2] * 3 + 2], 1);
//            vec4 normals0 = vec4(normals[indices[j] * 3], normals[indices[j] * 3 + 1], normals[indices[j] * 3 + 2], 0);
//            vec4 normals1 = vec4(normals[indices[j + 1] * 3], normals[indices[j + 1] * 3 + 1], normals[indices[j + 1] * 3 + 2], 0);
//            vec4 normals2 = vec4(normals[indices[j + 2] * 3], normals[indices[j + 2] * 3 + 1], normals[indices[j + 2] * 3 + 2], 0);
//            vec4 color = vec4(materials[material_ids[j/3]].diffuse[0], materials[material_ids[j/3]].diffuse[1], materials[material_ids[j/3]].diffuse[2], 0);
//            tinyobj::material_t mat = materials[material_ids[j/3]];
//            if (texcoords.empty()) {
//                triangles.push_back(face(vertex(pos0, normals0), vertex(pos1, normals1), vertex(pos2, normals2), color, mat));
//            }
//            else {
//                float u0 = texcoords[indices[j] * 2];
//                float v0 = texcoords[indices[j] * 2 + 1];
//                float u1 = texcoords[indices[j + 1] * 2];
//                float v1 = texcoords[indices[j + 1] * 2 + 1];
//                float u2 = texcoords[indices[j + 2] * 2];
//                float v2 = texcoords[indices[j + 2] * 2 + 1];
//                triangles.push_back(face(vertex(pos0, normals0, u0, v0), vertex(pos1, normals1, u1, v1), vertex(pos2, normals2, u2, v2), color, mat));
//            }
//        }
//    }
//}

Intersection Mesh::getIntersection(Ray& input) {

    std::vector<glm::vec2> vertexUV = std::vector<glm::vec2>();
    vertexUV.push_back(uvVecs[0].vertex1);
    vertexUV.push_back(uvVecs[0].vertex2);
    vertexUV.push_back(uvVecs[0].vertex3);


    Triangle closest = Triangle(faceVecs[0].vertex1, faceVecs[0].vertex2, faceVecs[0].vertex3, vertexUV, transform, this);
    float currT = 100;

    for (int i = 0; i < static_cast<int>(faceVecs.size()); i++) {

        std::vector<glm::vec2> vertexUV1 = std::vector<glm::vec2>();
        vertexUV1.push_back(uvVecs[i].vertex1);
        vertexUV1.push_back(uvVecs[i].vertex2);
        vertexUV1.push_back(uvVecs[i].vertex3);

        Triangle current = Triangle(faceVecs[i].vertex1, faceVecs[i].vertex2, faceVecs[i].vertex3, vertexUV1, transform, this);
//        triangles.push_back(current);
        float t = current.getIntersection(input).t;
        if (t < currT && t > 0) {
            currT = t;
            closest = current;
        }
    }

    return closest.getIntersection(input);
}



