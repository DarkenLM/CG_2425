#include "common/geometry/cone.hpp"

#include "common/parser.hpp"

ConeGeometry::~ConeGeometry() = default;

ConeGeometry::ConeGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_CONE;
}

std::vector<Point3D> ConeGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> ConeGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> ConeGeometry::copyIndices() {
    throw 1;
}
