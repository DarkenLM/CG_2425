#include "common/geometry/plane.hpp"
#include "common/parser.hpp"

PlaneGeometry::~PlaneGeometry() = default;

PlaneGeometry::PlaneGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}


std::vector<Point3D> PlaneGeometry::serialize() {
    throw 1;
}

PlaneGeometry* PlaneGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    Parser3D::load3DFile(filePath, points);

    return new PlaneGeometry(points);
    return nullptr;
}