#include "common/geometry/icosphere.hpp"

#include "common/parser.hpp"

IcosphereGeometry::~IcosphereGeometry() = default;

IcosphereGeometry::IcosphereGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
    this->_kind = GEOMETRY_ISCOSPHERE;
}

std::vector<Point3D> IcosphereGeometry::serialize() {
    throw 1;
}

IcosphereGeometry* IcosphereGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    // Parser3D::load3DFile(filePath, points);

    return new IcosphereGeometry(points);
    return nullptr;
}