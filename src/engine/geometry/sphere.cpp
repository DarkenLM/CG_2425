#include "common/geometry/sphere.hpp"

#include "common/parser.hpp"

SphereGeometry::~SphereGeometry() = default;

SphereGeometry::SphereGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_SPHERE;
}

std::vector<Point3D> SphereGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> SphereGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> SphereGeometry::serielizeIndices() {
    throw 1;
}

SphereGeometry* SphereGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new SphereGeometry(points, normals, indices);
    return nullptr;
}