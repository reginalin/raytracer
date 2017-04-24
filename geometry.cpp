#include "geometry.h"
#include <math.h>
#include <iostream>

Geometry::Geometry() {

}

Geometry::Geometry(glm::mat4 transform) {
    this->transform = transform;
}

Intersection Geometry::getIntersection(Ray& input) {
    return Intersection(glm::vec4(0, 0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0), -1, this);
}

