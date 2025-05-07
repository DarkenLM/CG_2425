#include "common/geometry/box.hpp"

#include "common/parser.hpp"

BoxGeometry::~BoxGeometry() = default;

BoxGeometry::BoxGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_BOX;
}

std::vector<Point3D> BoxGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> BoxGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> BoxGeometry::copyIndices() {
    throw 1;
}
