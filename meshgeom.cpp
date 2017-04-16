#include "meshgeom.h"
#include <math.h>


Mesh::Mesh(glm::mat4 transformMatrix) {
    transform = transformMatrix;
}


Intersection Mesh::getIntersection(Ray& input) {
    glm::mat4 inverted = glm::inverse(transform);
    Ray objRay = input.getTransformedCopy(inverted);


    glm::vec4 dir = objRay.direction;
    glm::vec4 orig = objRay.origin;



//    glm::vec4 temp = orig + t * dir;
//    glm::vec4 point = temp * transform; // left or right multiply?

//    glm::vec4 normal = temp * transpose(inverted);

    return Intersection(glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), -1, this);
}

struct face {
    vec4 vertex1;
    vec4 vertex2;
    vec4 vertex3;
};


float l, r, t, b, n, f, eye_x, eye_y, eye_z, center_x, center_y, center_z, up_x, up_y, up_z;

vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;


int width;
int height;
char* outputPpm;
char* colorOption;
bool isColorOption;

void readCam(char* camText);
mat4 orientMat();
void rasterizeTriangles();
void colorPixel(int pixel, int numTri);


img_t *img;
vector<vec4> normVecs;
vector<face> faceVecs;
vector<float> indices;
vector<vec4> transNormVecs;

int main(int argc, char *argv[]) {

    char* inputFile = argv[1];
    char* cameraText = argv[2];
    width = atoi(argv[3]);
    height = atoi(argv[4]);
    outputPpm = argv[5];

    if (argc == 7) {
        colorOption = argv[6];
        isColorOption = true;
    }

    cout<< tinyobj::LoadObj(shapes, materials, inputFile);

    readCam(cameraText);

    img = new_img(width, height);

    rasterizeTriangles();

    return 0;
}

void readCam(char *camText) {
    FILE *camFile = fopen(camText, "r");
    fscanf(camFile, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
           &l, &r, &t, &b, &n, &f, &eye_x, &eye_y, &eye_z, &center_x, &center_y, &center_z, &up_x, &up_y, &up_z);
    fclose(camFile);

}

mat4 orientMat() {
    vec4 up = vec4(up_x, up_y, up_z, 0).normalize();

    vec4 f = vec4(center_x - eye_x, center_y - eye_y, center_z - eye_z, 0).normalize();
    vec4 r = cross(f, up).normalize();
    vec4 u = cross(f, r).normalize();

    mat4 orientation = mat4(vec4(r[0], u[0], f[0], 0),
                            vec4(r[1], u[1], f[1], 0),
                            vec4(r[2], u[2], f[2], 0),
                            vec4(0, 0, 0, 1));
    return orientation;
}

mat4 viewMat() {
    mat4 translation = mat4(vec4(1, 0, 0, 0),
                            vec4(0, 1, 0, 0),
                            vec4(0, 0, 1, 0),
                            vec4(-eye_x, -eye_y, -eye_z, 1));
    return orientMat() * translation;
}


mat4 projMat() {
    mat4 frustum = mat4(vec4((2*n)/(r-l), 0, 0, 0),
                        vec4(0, (2*n)/(t-b), 0, 0),
                        vec4((r+l)/(r-l), (t+b)/(t-b), f/(f-n), 1),
                        vec4(0, 0, (-f*n)/(f-n), 0));

    return frustum;
}

void rasterizeTriangles() {

    float zBuffs[width * height];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            zBuffs[width * i + j] = 2;
        }
    }

    // positions: vector of vertex coordinates
    vector<float> positions;
    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        for (int j = 0; j < static_cast<int>(shapes[i].mesh.positions.size()); j++) {
            positions.push_back(shapes[i].mesh.positions[j]);
        }
    }

    // vector of vec4s, with w coordinate of 1
    vector<vec4> posVecs;
    for (int i = 0; i < static_cast<int>(positions.size()); i+=3) {
        vec4 here = vec4(positions[i], positions[i+1], positions[i+2], 1);
        posVecs.push_back(here);
    }

    // normals; vector of normal coordinates
    vector<float> normals;

    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        for (int j = 0; j < static_cast<int>(shapes[i].mesh.normals.size()); j++) {
            normals.push_back(shapes[i].mesh.normals[j]);
        }
    }

    // vector of vec4s, with w coordinate of 0
    for (int i = 0; i < static_cast<int>(normals.size()); i+=3) {
        vec4 here = vec4(normals[i], normals[i+1], normals[i+2], 0);
        normVecs.push_back(here);
    }

    for (int i = 0; i < static_cast<int>(normVecs.size()); i++) {
        vec4 update = orientMat() * normVecs[i];
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
        vec4 first = projMat() * viewMat() * faceVecs[i].vertex1;
        float w1 = first[3];
        faceVecs[i].vertex1 = first/w1;

        vec4 second = projMat() * viewMat() * faceVecs[i].vertex2;
        float w2 = second[3];
        faceVecs[i].vertex2 = second/w2;

        vec4 third = projMat() * viewMat() * faceVecs[i].vertex3;
        float w3 = third[3];
        faceVecs[i].vertex3 = third/w3;
    }

    for (int i = 0; i < static_cast<int>(faceVecs.size()); i++) {
        // convert from NDC to pixel coordinates
        vec4 first = faceVecs[i].vertex1;
        float x1 = (first[0] + 1) * (width/2);
        float y1 = (1 - first[1]) * (height/2);
        float z1 = first[2];
        float w1 = first[3];
        faceVecs[i].vertex1 = vec4(x1, y1, z1, w1);

        vec4 second = faceVecs[i].vertex2;
        float x2 = (second[0] + 1) * (width/2);
        float y2 = (1 - second[1]) * (height/2);
        float z2 = second[2];
        float w2 = second[3];
        faceVecs[i].vertex2 = vec4(x2, y2, z2, w2);

        vec4 third = faceVecs[i].vertex3;
        float x3 = (third[0] + 1) * (width/2);
        float y3 = (1 - third[1]) * (height/2);
        float z3 = third[2];
        float w3 = third[3];
        faceVecs[i].vertex3 = vec4(x3, y3, z3, w3);


        // triangles that are not visible
        if (z1 < 0 && z2 < 0 && z3 < 0) {
            continue;
        } else if (z1 > 1 && z2 > 1 && z2 > 1) {
            continue;
        }

        // compute 2D bounding box
        int minX = floor(std::min({x1, x2, x3}));
        int maxX = ceil(std::max({x1, x2, x3}));

        int minY = floor(std::min({y1, y2, y3}));
        int maxY = ceil(std::max({y1, y2, y3}));

        if (maxX < 0 || minX > width || maxY < 0 || minY > height) {
            continue;
        }
    }
}
