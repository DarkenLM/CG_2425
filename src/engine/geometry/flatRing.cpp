#include "common/geometry/flatRing.hpp"

#include "common/parser.hpp"

FlatRingGeometry::~FlatRingGeometry() = default;

FlatRingGeometry::FlatRingGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_FLATRING;
}

std::vector<Point3D> FlatRingGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> FlatRingGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> FlatRingGeometry::serielizeIndices() {
    throw 1;
}

FlatRingGeometry* FlatRingGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new FlatRingGeometry(points, normals, indices);
    return nullptr;
}