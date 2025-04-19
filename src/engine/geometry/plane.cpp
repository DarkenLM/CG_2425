#include "common/geometry/plane.hpp"

#include "common/parser.hpp"

PlaneGeometry::~PlaneGeometry() = default;

PlaneGeometry::PlaneGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_PLANE;
}

std::vector<Point3D> PlaneGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> PlaneGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> PlaneGeometry::serielizeIndices() {
    throw 1;
}

PlaneGeometry* PlaneGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new PlaneGeometry(points, normals, indices);
}