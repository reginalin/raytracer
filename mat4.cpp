#include "mat4.h"
#include "math.h"
#include "assert.h"

///Constructors
mat4::mat4() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            data[x][y] = 0;     //2D loop over every matrix position
        }
    }
}

/// Initializes the diagonal values of the matrix to diag. All other values are 0.
mat4::mat4(float diag) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (x == y) {
                data[x][y] = diag;      //same as mat4(), but checks if position is along the top-left to bottom-right diagonal
            }
            else {
                data[x][y] = 0;
            }
        }
    }
}

/// Initializes matrix with each vector representing a column in the matrix
mat4::mat4(const vec4 &col0, const vec4 &col1, const vec4 &col2, const vec4& col3) {
    data[0] = col0;
    data[1] = col1;
    data[2] = col2;
    data[3] = col3;
}

mat4::mat4(const mat4 &m2) { // copy constructor; copies values of m2 into this
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            data[x][y] = m2[x][y];
        }
    }
}

///----------------------------------------------------------------------
/// Getters
///----------------------------------------------------------------------

/// Returns the values of the column at the index
/// Does NOT check the array bound because doing so is slow
const vec4 &mat4::operator[](unsigned int index) const {
    return data[index];
}

/// Returns a reference to the column at the index
/// Does NOT check the array bound because doing so is slow
vec4 &mat4::operator[](unsigned int index) {
    vec4 &toReturn = data[index];
    return toReturn;
}

/// Returns the values of the column at the index
/// DOES check the array bound because doing so is slow
const vec4 &mat4::operator()(unsigned int index) const {
    assert(index <= 3);
    return data[index];
}

/// Returns a reference to the column at the index
/// DOES check the array bound because doing so is slow
vec4 &mat4::operator()(unsigned int index) {
    assert(index <= 3);
    vec4 &toReturn = data[index];
    return toReturn;
}

///----------------------------------------------------------------------
/// Static Initializers
///----------------------------------------------------------------------

/// Creates a 3-D rotation matrix.
/// Takes an angle in degrees and an axis represented by its xyz components, and outputs a 4x4 rotation matrix
mat4 mat4::rot(float angle, float x, float y, float z) {
    //As instructed, only functions using x, y, or z axis, one at a time. Used matrices from lecture slides.
    if (x != 0 && y == 0 && z == 0) {
        vec4 col0 = vec4(1, 0, 0, 0);
        vec4 col1 = vec4(0, cos(angle), sin(angle), 0);
        vec4 col2 = vec4(0, -sin(angle), cos(angle), 0);
        vec4 col3 = vec4(0, 0, 0, 1);
        return mat4(col0, col1, col2, col3);
    }
    else if (x == 0 && y != 0 && z == 0) {
        vec4 col0 = vec4(cos(angle), 0, -sin(angle), 0);
        vec4 col1 = vec4(0, 1, 0, 0);
        vec4 col2 = vec4(sin(angle), 0, cos(angle), 0);
        vec4 col3 = vec4(0, 0, 0, 1);
        return mat4(col0, col1, col2, col3);
    }
    else if (x == 0 && y == 0 && z != 0) {
        vec4 col0 = vec4(cos(angle), sin(angle), 0, 0);
        vec4 col1 = vec4(-sin(angle), cos(angle), 0, 0);
        vec4 col2 = vec4(0, 0, 1, 0);
        vec4 col3 = vec4(0, 0, 0, 1);
        return mat4(col0, col1, col2, col3);
    }
    //Return identity matrix otherwise
    vec4 col0 = vec4(1, 0, 0, 0);
    vec4 col1 = vec4(0, 1, 0, 0);
    vec4 col2 = vec4(0, 0, 1, 0);
    vec4 col3 = vec4(0, 0, 0, 1);
    return mat4(col0, col1, col2, col3);
}

/// Takes an xyz displacement and outputs a 4x4 translation matrix
mat4 mat4::trans(float x, float y, float z) {   //Used matrices from lecture slides.
    vec4 col0 = vec4(1, 0, 0, 0);
    vec4 col1 = vec4(0, 1, 0, 0);
    vec4 col2 = vec4(0, 0, 1, 0);
    vec4 col3 = vec4(x, y, z, 1);
    return mat4(col0, col1, col2, col3);
}

/// Takes an xyz scale and outputs a 4x4 scale matrix
mat4 mat4::scale(float x, float y, float z) {   //Used matrices from lecture slides.
    vec4 col0 = vec4(x, 0, 0, 0);
    vec4 col1 = vec4(0, y, 0, 0);
    vec4 col2 = vec4(0, 0, z, 0);
    vec4 col3 = vec4(0, 0, 0, 1);
    return mat4(col0, col1, col2, col3);
}


///----------------------------------------------------------------------
/// Operator Functions
///----------------------------------------------------------------------

/// Assign m2 to this and return this
mat4 &mat4::operator=(const mat4 &m2) {
    data[0] = m2[0];
    data[1] = m2[1];
    data[2] = m2[2];
    data[3] = m2[3];
    return (*this);
}

/// Test for equality
bool mat4::operator==(const mat4 &m2) const {
    if (data[0] != m2[0]) return false;
    if (data[1] != m2[1]) return false;
    if (data[2] != m2[2]) return false;
    if (data[3] != m2[3]) return false;
    return true;
}

/// Test for inequality
bool mat4::operator!=(const mat4 &m2) const {
    if (
            data[0] == m2[0]
            || data[1] == m2[1]
            || data[2] == m2[2]
            || data[3] == m2[3]) {
        return false;
    }
    return true;
}

/// Element-wise arithmetic
/// e.g. += adds the elements of m2 to this and returns this (like regular +=)
///      +  returns a new matrix whose elements are the sums of this and v2
mat4& mat4::operator+=(const mat4 &m2) {
    vec4 col0 = vec4(data[0][0] + m2[0][0], data[0][1] + m2[0][1], data[0][2] + m2[0][2], data[0][3] + m2[0][3]);
    vec4 col1 = vec4(data[1][0] + m2[1][0], data[1][1] + m2[1][1], data[1][2] + m2[1][2], data[1][3] + m2[1][3]);
    vec4 col2 = vec4(data[2][0] + m2[2][0], data[2][1] + m2[2][1], data[2][2] + m2[2][2], data[2][3] + m2[2][3]);
    vec4 col3 = vec4(data[3][0] + m2[3][0], data[3][1] + m2[3][1], data[3][2] + m2[3][2], data[3][3] + m2[3][3]);
    data[0] = col0;
    data[1] = col1;
    data[2] = col2;
    data[3] = col3;
    return *this;
}
mat4& mat4::operator-=(const mat4 &m2) {
    vec4 col0 = vec4(data[0][0] - m2[0][0], data[0][1] - m2[0][1], data[0][2] - m2[0][2], data[0][3] - m2[0][3]);
    vec4 col1 = vec4(data[1][0] - m2[1][0], data[1][1] - m2[1][1], data[1][2] - m2[1][2], data[1][3] - m2[1][3]);
    vec4 col2 = vec4(data[2][0] - m2[2][0], data[2][1] - m2[2][1], data[2][2] - m2[2][2], data[2][3] - m2[2][3]);
    vec4 col3 = vec4(data[3][0] - m2[3][0], data[3][1] - m2[3][1], data[3][2] - m2[3][2], data[3][3] - m2[3][3]);
    data[0] = col0;
    data[1] = col1;
    data[2] = col2;
    data[3] = col3;
    return *this;
}
mat4& mat4::operator*=(float c) {                 // multiplication by a scalar
    vec4 col0 = vec4(data[0][0] * c, data[0][1] * c, data[0][2] * c, data[0][3] * c);
    vec4 col1 = vec4(data[1][0] * c, data[1][1] * c, data[1][2] * c, data[1][3] * c);
    vec4 col2 = vec4(data[2][0] * c, data[2][1] * c, data[2][2] * c, data[2][3] * c);
    vec4 col3 = vec4(data[3][0] * c, data[3][1] * c, data[3][2] * c, data[3][3] * c);
    data[0] = col0;
    data[1] = col1;
    data[2] = col2;
    data[3] = col3;
    return *this;
}
mat4& mat4::operator/=(float c) {                 // division by a scalar
    vec4 col0 = vec4(data[0][0] / c, data[0][1] / c, data[0][2] / c, data[0][3] / c);
    vec4 col1 = vec4(data[1][0] / c, data[1][1] / c, data[1][2] / c, data[1][3] / c);
    vec4 col2 = vec4(data[2][0] / c, data[2][1] / c, data[2][2] / c, data[2][3] / c);
    vec4 col3 = vec4(data[3][0] / c, data[3][1] / c, data[3][2] / c, data[3][3] / c);
    data[0] = col0;
    data[1] = col1;
    data[2] = col2;
    data[3] = col3;
    return *this;
}
mat4  mat4::operator+(const mat4 &m2) const {
    vec4 col0 = vec4(data[0][0] + m2[0][0], data[0][1] + m2[0][1], data[0][2] + m2[0][2], data[0][3] + m2[0][3]);
    vec4 col1 = vec4(data[1][0] + m2[1][0], data[1][1] + m2[1][1], data[1][2] + m2[1][2], data[1][3] + m2[1][3]);
    vec4 col2 = vec4(data[2][0] + m2[2][0], data[2][1] + m2[2][1], data[2][2] + m2[2][2], data[2][3] + m2[2][3]);
    vec4 col3 = vec4(data[3][0] + m2[3][0], data[3][1] + m2[3][1], data[3][2] + m2[3][2], data[3][3] + m2[3][3]);
    return mat4(col0, col1, col2, col3);
}
mat4  mat4::operator-(const mat4 &m2) const {
    vec4 col0 = vec4(data[0][0] - m2[0][0], data[0][1] - m2[0][1], data[0][2] - m2[0][2], data[0][3] - m2[0][3]);
    vec4 col1 = vec4(data[1][0] - m2[1][0], data[1][1] - m2[1][1], data[1][2] - m2[1][2], data[1][3] - m2[1][3]);
    vec4 col2 = vec4(data[2][0] - m2[2][0], data[2][1] - m2[2][1], data[2][2] - m2[2][2], data[2][3] - m2[2][3]);
    vec4 col3 = vec4(data[3][0] - m2[3][0], data[3][1] - m2[3][1], data[3][2] - m2[3][2], data[3][3] - m2[3][3]);
    return mat4(col0, col1, col2, col3);
}
mat4  mat4::operator*(float c) const {             // multiplication by a scalar
    vec4 col0 = vec4(data[0][0] * c, data[0][1] * c, data[0][2] * c, data[0][3] * c);
    vec4 col1 = vec4(data[1][0] * c, data[1][1] * c, data[1][2] * c, data[1][3] * c);
    vec4 col2 = vec4(data[2][0] * c, data[2][1] * c, data[2][2] * c, data[2][3] * c);
    vec4 col3 = vec4(data[3][0] * c, data[3][1] * c, data[3][2] * c, data[3][3] * c);
    return mat4(col0, col1, col2, col3);
}
mat4  mat4::operator/(float c) const {             // division by a scalar
    vec4 col0 = vec4(data[0][0] / c, data[0][1] / c, data[0][2] / c, data[0][3] / c);
    vec4 col1 = vec4(data[1][0] / c, data[1][1] / c, data[1][2] / c, data[1][3] / c);
    vec4 col2 = vec4(data[2][0] / c, data[2][1] / c, data[2][2] / c, data[2][3] / c);
    vec4 col3 = vec4(data[3][0] / c, data[3][1] / c, data[3][2] / c, data[3][3] / c);
    return mat4(col0, col1, col2, col3);
}

/// Matrix multiplication (m1 * m2)
mat4 mat4::operator*(const mat4 &m2) const {
    vec4 col0 = vec4(data[0][0] * m2[0][0] + data[1][0] * m2[0][1] + data[2][0] * m2[0][2] + data[3][0] * m2[0][3],
            data[0][1] * m2[0][0] + data[1][1] * m2[0][1] + data[2][1] * m2[0][2] + data[3][1] * m2[0][3],
            data[0][2] * m2[0][0] + data[1][2] * m2[0][1] + data[2][2] * m2[0][2] + data[3][2] * m2[0][3],
            data[0][3] * m2[0][0] + data[1][3] * m2[0][1] + data[2][3] * m2[0][2] + data[3][3] * m2[0][3]);
    vec4 col1 = vec4(data[0][0] * m2[1][0] + data[1][0] * m2[1][1] + data[2][0] * m2[1][2] + data[3][0] * m2[1][3],
            data[0][1] * m2[1][0] + data[1][1] * m2[1][1] + data[2][1] * m2[1][2] + data[3][1] * m2[1][3],
            data[0][2] * m2[1][0] + data[1][2] * m2[1][1] + data[2][2] * m2[1][2] + data[3][2] * m2[1][3],
            data[0][3] * m2[1][0] + data[1][3] * m2[1][1] + data[2][3] * m2[1][2] + data[3][3] * m2[1][3]);
    vec4 col2 = vec4(data[0][0] * m2[2][0] + data[1][0] * m2[2][1] + data[2][0] * m2[2][2] + data[3][0] * m2[2][3],
            data[0][1] * m2[2][0] + data[1][1] * m2[2][1] + data[2][1] * m2[2][2] + data[3][1] * m2[2][3],
            data[0][2] * m2[2][0] + data[1][2] * m2[2][1] + data[2][2] * m2[2][2] + data[3][2] * m2[2][3],
            data[0][3] * m2[2][0] + data[1][3] * m2[2][1] + data[2][3] * m2[2][2] + data[3][3] * m2[2][3]);
    vec4 col3 = vec4(data[0][0] * m2[3][0] + data[1][0] * m2[3][1] + data[2][0] * m2[3][2] + data[3][0] * m2[3][3],
            data[0][1] * m2[3][0] + data[1][1] * m2[3][1] + data[2][1] * m2[3][2] + data[3][1] * m2[3][3],
            data[0][2] * m2[3][0] + data[1][2] * m2[3][1] + data[2][2] * m2[3][2] + data[3][2] * m2[3][3],
            data[0][3] * m2[3][0] + data[1][3] * m2[3][1] + data[2][3] * m2[3][2] + data[3][3] * m2[3][3]);
    return mat4(col0, col1, col2, col3);
}

/// Matrix/vector multiplication (m * v)
/// Assume v is a column vector (ie. a 4x1 matrix)
vec4 mat4::operator*(const vec4 &v) const {
    float f1 = data[0][0] * v[0] + data[1][0] * v[1] + data[2][0] * v[2] + data[3][0] * v[3];
    float f2 = data[0][1] * v[0] + data[1][1] * v[1] + data[2][1] * v[2] + data[3][1] * v[3];
    float f3 = data[0][2] * v[0] + data[1][2] * v[1] + data[2][2] * v[2] + data[3][2] * v[3];
    float f4 = data[0][3] * v[0] + data[1][3] * v[1] + data[2][3] * v[2] + data[3][3] * v[3];
    return vec4(f1, f2, f3, f4);
}

///----------------------------------------------------------------------
/// Matrix Operations
///----------------------------------------------------------------------

/// Returns the transpose of the input matrix (v_ij == v_ji)
mat4 mat4::transpose() const {
    vec4 col0 = vec4(data[0][0], data[1][0], data[2][0], data[3][0]);
    vec4 col1 = vec4(data[0][1], data[1][1], data[2][1], data[3][1]);
    vec4 col2 = vec4(data[0][2], data[1][2], data[2][2], data[3][2]);
    vec4 col3 = vec4(data[0][3], data[1][3], data[2][3], data[3][3]);
    return mat4(col0, col1, col2, col3);
}

/// Returns a column of the input matrix
const vec4 &mat4::col(unsigned int index) const { // const version
    return data[index];
}
vec4 &mat4::col(unsigned int index) {             // non-const version
    return data[index];
}

/// Scalar multiplication (c * m)
mat4 operator*(float c, const mat4 &m) {
    vec4 col0 = vec4(m[0][0] * c, m[0][1] * c, m[0][2] * c, m[0][3] * c);
    vec4 col1 = vec4(m[1][0] * c, m[1][1] * c, m[1][2] * c, m[1][3] * c);
    vec4 col2 = vec4(m[2][0] * c, m[2][1] * c, m[2][2] * c, m[2][3] * c);
    vec4 col3 = vec4(m[3][0] * c, m[3][1] * c, m[3][2] * c, m[3][3] * c);
    return mat4(col0, col1, col2, col3);
}

/// Vector/matrix multiplication (v * m)
/// Assume v is a row vector (ie. a 1x4 matrix)
vec4 operator*(const vec4 &v, const mat4 &m) {
    float f1 = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
    float f2 = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
    float f3 = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
    float f4 = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];
    return vec4(f1, f2, f3, f4);
}

/// Prints the matrix to a stream in a nice format
std::ostream &operator<<(std::ostream &o, const mat4 &m) {
    return
            o << "(" << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << ")\n"
            << "(" << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << ")\n"
            << "(" << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << ")\n"
            << "(" << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << ")";
    //Line spacing is incorrect according to QT Creator, but the spacing it prefers is far worse for some reason.
}
