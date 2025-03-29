#include "common/geometry/obj.hpp"
#include "common/parser.hpp"

ObjGeometry::~ObjGeometry() = default;

ObjGeometry::ObjGeometry(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

std::vector<Point3D> ObjGeometry::serialize() {
    throw 1;
}
