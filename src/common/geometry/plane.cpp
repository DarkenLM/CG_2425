#include "common/geometry/plane.hpp"

PlaneGeometry::~PlaneGeometry() = default;

PlaneGeometry::PlaneGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_PLANE;
}

std::vector<Point3D> PlaneGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> PlaneGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> PlaneGeometry::copyIndices() {
    throw 1;
}
