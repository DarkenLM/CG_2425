#include "common/geometry/bezier.hpp"

#include "common/parser.hpp"

BezierGeometry::~BezierGeometry() = default;

BezierGeometry::BezierGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

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