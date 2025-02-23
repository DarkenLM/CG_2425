#include "common/geometry/plane.hpp"
#include "common/parser.hpp"

PlaneGeometry* PlaneGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    Parser3D::load3DFile(filePath, points);

    return new PlaneGeometry(points);
}