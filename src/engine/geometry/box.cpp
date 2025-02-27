#include "common/geometry/box.hpp"
#include "common/parser.hpp"

BoxGeometry::~BoxGeometry() = default;

BoxGeometry::BoxGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

std::vector<Point3D> BoxGeometry::serialize() {
    throw 1;
}

BoxGeometry* BoxGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    // Parser3D::load3DFile(filePath, points);

    return new BoxGeometry(points);
    return nullptr;
}