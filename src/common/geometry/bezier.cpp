#include "common/geometry/bezier.hpp"

BezierGeometry::~BezierGeometry() = default;

BezierGeometry::BezierGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<Vector2<float>> uvs, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;
    this->uvs = uvs;

    this->_kind = GEOMETRY_BOX;
}

std::vector<Point3D> BezierGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> BezierGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> BezierGeometry::copyIndices() {
    throw 1;
}

std::vector<Vector2<float>> BezierGeometry::copyUVs() {
    throw 1;
}