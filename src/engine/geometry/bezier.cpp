#include "common/geometry/bezier.hpp"

#include "common/parser.hpp"

BezierGeometry::~BezierGeometry() = default;

BezierGeometry::BezierGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_BOX;
}

std::vector<Point3D> BezierGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> BezierGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> BezierGeometry::serielizeIndices() {
    throw 1;
}

BezierGeometry* BezierGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new BezierGeometry(points, normals, indices);
    return nullptr;
}