#include "common/geometry/sphere.hpp"

#include "common/parser.hpp"

SphereGeometry::~SphereGeometry() = default;

SphereGeometry::SphereGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

std::vector<Point3D> SphereGeometry::serialize() {
    throw 1;
}

SphereGeometry* SphereGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    Parser3D::load3DFile(filePath, points);

    return new SphereGeometry(points);
    return nullptr;
}