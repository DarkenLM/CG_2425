#include "common/geometry/flatRing.hpp"

FlatRingGeometry::~FlatRingGeometry() = default;

FlatRingGeometry::FlatRingGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<Vector2<float>> uvs, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;
    this->uvs = uvs;

    this->_kind = GEOMETRY_FLATRING;
}

std::vector<Point3D> FlatRingGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> FlatRingGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> FlatRingGeometry::copyIndices() {
    throw 1;
}

std::vector<Vector2<float>> FlatRingGeometry::copyUVs() {
    throw 1;
}