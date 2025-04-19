#include "common/geometry/box.hpp"

#include "common/parser.hpp"

BoxGeometry::~BoxGeometry() = default;

BoxGeometry::BoxGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_BOX;
}

std::vector<Point3D> BoxGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> BoxGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> BoxGeometry::serielizeIndices() {
    throw 1;
}

BoxGeometry* BoxGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new BoxGeometry(points, normals, indices);
    return nullptr;
}