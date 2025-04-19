#include "common/geometry/icosphere.hpp"

#include "common/parser.hpp"

IcosphereGeometry::~IcosphereGeometry() = default;

IcosphereGeometry::IcosphereGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_ICOSPHERE;
}

std::vector<Point3D> IcosphereGeometry::serielizeVertices() {
    throw 1;
}

std::vector<Vector3<float>> IcosphereGeometry::serielizeNormals() {
    throw 1;
}

std::vector<unsigned int> IcosphereGeometry::serielizeIndices() {
    throw 1;
}

IcosphereGeometry* IcosphereGeometry::deserialize(std::string filePath) {
    std::vector<Point3D> points;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    // Parser3D::load3DFile(filePath, points);

    return new IcosphereGeometry(points, normals, indices);
    return nullptr;
}