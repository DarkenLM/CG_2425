#include "common/geometry/flatRing.hpp"

#include "common/parser.hpp"

FlatRingGeometry::~FlatRingGeometry() = default;

FlatRingGeometry::FlatRingGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

std::vector<Point3D> FlatRingGeometry::serialize() {
    throw 1;
}

FlatRingGeometry* FlatRingGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    Parser3D::load3DFile(filePath, points);

    return new FlatRingGeometry(points);
    return nullptr;
}