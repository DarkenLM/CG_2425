#include "common/geometry/point.hpp"

#include "common/util/vectors.hpp"

Point3D::Point3D() : x(0), y(0), z(0) {}
Point3D::Point3D(float x, float y, float z) : x(x), y(y), z(z) {}

void Point3D::setX(float xVal) { x = xVal; }
void Point3D::setY(float yVal) { y = yVal; }
void Point3D::setZ(float zVal) { z = zVal; }
void Point3D::set(float xVal, float yVal, float zVal) {
    x = xVal;
    y = yVal;
    z = zVal;
}

Point3D Point3D::withY(float y) const {
    return Point3D(this->x, y, this->z);
}

Point3D Point3D::withX(float x) const {
    return Point3D(x, this->y, this->z);
}

Point3D Point3D::withZ(float z) const {
    return Point3D(this->x, this->y, z);
}

Point3D Point3D::copy() {
    return Point3D(this->x, this->y, this->z);
}

bool Point3D::operator==(const Point3D& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool Point3D::operator!=(const Point3D& other) const {
    return !(*this == other);
}

Point3D operator+(const Point3D& a, const Point3D& b) {
    return Point3D(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

Point3D Point3D::operator-(const Point3D& other) const {
    return Point3D(x - other.x, y - other.y, z - other.z);
}

Vector3<float> Point3D::toVector3() {
    return Vector3<float>(this->x, this->y, this->z);
}

Point3D operator*(const Point3D& point, float scalar) {
    return Point3D(point.getX() * scalar, point.getY() * scalar, point.getZ() * scalar);
}

Point3D operator*(float scalar, const Point3D& point) {
    return point * scalar;
}

// Output Stream Overload
std::ostream& operator<<(std::ostream& os, const Point3D& point) {
    os << "(" << point.getX() << ", " << point.getY() << ", " << point.getZ() << ")";
    return os;
}