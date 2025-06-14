#include "common/geometry/box.hpp"

BoxGeometry::~BoxGeometry() = default;

BoxGeometry::BoxGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<Vector2<float>> uvs, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;
    this->uvs = uvs;

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

std::vector<Vector2<float>> BoxGeometry::copyUVs() {
    throw 1;
}