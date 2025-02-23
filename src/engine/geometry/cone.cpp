#include "common/geometry/cone.hpp"

#include "common/parser.hpp"

ConeGeometry::~ConeGeometry() = default;

ConeGeometry::ConeGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

std::vector<Point3D> ConeGeometry::serialize() {
    throw 1;
}

ConeGeometry* ConeGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    Parser3D::load3DFile(filePath, points);

    return new ConeGeometry(points);
    return nullptr;
}