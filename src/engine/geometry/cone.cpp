#include "common/geometry/cone.hpp"

#include "common/parser.hpp"

ConeGeometry::~ConeGeometry() = default;

ConeGeometry::ConeGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_CONE;
}

std::vector<Point3D> ConeGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> ConeGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> ConeGeometry::serielizeIndices() {
    throw 1;
}

ConeGeometry* ConeGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new ConeGeometry(points, normals, indices);
    return nullptr;
}