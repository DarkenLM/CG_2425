#include "common/geometry/torus.hpp"

#include "common/parser.hpp"

TorusGeometry::~TorusGeometry() = default;

TorusGeometry::TorusGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

std::vector<Point3D> TorusGeometry::serialize() {
    throw 1;
}

TorusGeometry* TorusGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    // Parser3D::load3DFile(filePath, points);

    return new TorusGeometry(points);
    return nullptr;
}