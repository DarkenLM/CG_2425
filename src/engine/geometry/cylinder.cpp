#include "common/geometry/cylinder.hpp"

#include "common/parser.hpp"

CylinderGeometry::~CylinderGeometry() = default;

CylinderGeometry::CylinderGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

std::vector<Point3D> CylinderGeometry::serialize() {
    throw 1;
}

CylinderGeometry* CylinderGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    Parser3D::load3DFile(filePath, points);

    return new CylinderGeometry(points);
    return nullptr;
}