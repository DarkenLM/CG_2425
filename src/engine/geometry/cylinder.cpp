#include "common/geometry/cylinder.hpp"

#include "common/parser.hpp"

CylinderGeometry::~CylinderGeometry() = default;

CylinderGeometry::CylinderGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_CYLINDER;
}

std::vector<Point3D> CylinderGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> CylinderGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> CylinderGeometry::serielizeIndices() {
    throw 1;
}

CylinderGeometry* CylinderGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new CylinderGeometry(points, normals, indices);
    return nullptr;
}