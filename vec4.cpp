#include "vec4.h"
#include "assert.h"
#include "math.h"

//Constructors
vec4::vec4() {
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
}

vec4::vec4(float x, float y, float z, float w) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
}

vec4::vec4(const vec4 &v2) {
    data[0] = v2[0];
    data[1] = v2[1];
    data[2] = v2[2];
    data[3] = v2[3];
}

//Getters/Setters
//How does setting work if one is const and the other is a reference?
float vec4::operator[](unsigned int index) const {
    return data[index];
}

float &vec4::operator[](unsigned int index) {
    float &toReturn = data[index];
    return toReturn;
}

float vec4::operator()(unsigned int index) const {
    assert(index <= 3);
    return data[index];
}

float &vec4::operator()(unsigned int index) {
    assert(index <= 3);
    float &toReturn = data[index];
    return toReturn;
}

//Set
vec4 &vec4::operator=(const vec4 &v2) {
    data[0] = v2[0];
    data[1] = v2[1];
    data[2] = v2[2];
    data[3] = v2[3];
    vec4 &toReturn = *this;
    return toReturn;
}

//Compare
bool vec4::operator==(const vec4 &v2) const {
    if (data[0] != v2[0]) return false;
    if (data[1] != v2[1]) return false;
    if (data[2] != v2[2]) return false;
    if (data[3] != v2[3]) return false;
    return true;
}

bool vec4::operator!=(const vec4 &v2) const {
    if (
            data[0] == v2[0]
            && data[1] == v2[1]
            && data[2] == v2[2]
            && data[3] == v2[3]) {
        return false;
    }
    else {
        return true;
    }
}

//Arithmatic
//Modify in place
vec4& vec4::operator+=(const vec4 &v2) {
    data[0] = data[0] + v2[0];
    data[1] = data[1] + v2[1];
    data[2] = data[2] + v2[2];
    data[3] = data[3] + v2[3];
    return *this;
}

vec4& vec4::operator-=(const vec4 &v2) {
    data[0] = data[0] - v2[0];
    data[1] = data[1] - v2[1];
    data[2] = data[2] - v2[2];
    data[3] = data[3] - v2[3];
    return *this;
}

vec4& vec4::operator*=(float c) {
    data[0] = data[0] * c;
    data[1] = data[1] * c;
    data[2] = data[2] * c;
    data[3] = data[3] * c;
    return *this;
}

vec4& vec4::operator/=(float c) {
    data[0] = data[0] / c;
    data[1] = data[1] / c;
    data[2] = data[2] / c;
    data[3] = data[3] / c;
    return *this;
}

//Return new modified
vec4 vec4::operator+(const vec4 &v2) const {
    vec4 toReturn = vec4();     //How do
    toReturn[0] = data[0] + v2[0];
    toReturn[1] = data[1] + v2[1];
    toReturn[2] = data[2] + v2[2];
    toReturn[3] = data[3] + v2[3];
    return toReturn;
}

vec4 vec4::operator-(const vec4 &v2) const {
    vec4 toReturn = vec4();
    toReturn[0] = data[0] - v2[0];
    toReturn[1] = data[1] - v2[1];
    toReturn[2] = data[2] - v2[2];
    toReturn[3] = data[3] - v2[3];
    return toReturn;
}

vec4 vec4::operator*(float c) const {
    vec4 toReturn = vec4();
    toReturn[0] = data[0] * c;
    toReturn[1] = data[1] * c;
    toReturn[2] = data[2] * c;
    toReturn[3] = data[3] * c;
    return toReturn;
}

vec4 vec4::operator/(float c) const {
    vec4 toReturn = vec4();
    toReturn[0] = data[0] / c;
    toReturn[1] = data[1] / c;
    toReturn[2] = data[2] / c;
    toReturn[3] = data[3] / c;
    return toReturn;
}

/// Returns the geometric length of the input vector
float vec4::length() const {
    return sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2] + data[3]*data[3]);
}

/// return a new vec4 that is a normalized (unit-length) version of this one
vec4 vec4::normalize() const {
    if (this->length() == 0) {
        return vec4();
    }
    return (*this)/this->length();
}

/// noralize this vector in place
void vec4::norm() {
    if (this->length() == 0) {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
    }
    (*this) /= this->length();
}

/// Dot Product
float dot(const vec4 &v1, const vec4 &v2) {
    return v1[0] * v2[0] + v1[1] *  v2[1] + v1[2] *  v2[2] + v1[3] *  v2[3];
}

/// Cross Product
vec4 cross(const vec4 &v1, const vec4 &v2) {
    float x = v1[1] * v2[2] - v2[1] * v1[2];
    float y = v1[2] * v2[0] - v2[2] * v1[0];
    float z = v1[0] * v2[1] - v2[0] * v1[1];
    float w = 0;    //4D cross product does not exist, use 0 instead.
    return vec4(x, y, z, w);
}

/// Scalar Multiplication (c * v)
vec4 operator*(float c, const vec4 &v) {
    return v * c;   //Scalar multiplication is commutative
}

/// Prints the vector to a stream in a nice format for integration with cout
std::ostream &operator<<(std::ostream &o, const vec4 &v) {
    return o << " (" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ") ";
}


