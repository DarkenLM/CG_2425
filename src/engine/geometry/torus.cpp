#include "common/geometry/torus.hpp"

#include "common/parser.hpp"

TorusGeometry::~TorusGeometry() = default;

TorusGeometry::TorusGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_TORUS;
}

std::vector<Point3D> TorusGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> TorusGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> TorusGeometry::serielizeIndices() {
    throw 1;
}

TorusGeometry* TorusGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new TorusGeometry(points, normals, indices);
    return nullptr;
}