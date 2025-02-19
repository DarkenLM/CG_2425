#include "common/geometry/point.hpp"

Point3D::Point3D() : x(0), y(0), z(0) {}
Point3D::Point3D(double x, double y, double z) : x(x), y(y), z(z) {}

void Point3D::setX(double xVal) { x = xVal; }
void Point3D::setY(double yVal) { y = yVal; }
void Point3D::setZ(double zVal) { z = zVal; }
void Point3D::set(double xVal, double yVal, double zVal) {
    x = xVal;
    y = yVal;
    z = zVal;
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

// Output Stream Overload
std::ostream& operator<<(std::ostream& os, const Point3D& point) {
    os << "(" << point.getX() << ", " << point.getY() << ", " << point.getZ() << ")";
    return os;
}