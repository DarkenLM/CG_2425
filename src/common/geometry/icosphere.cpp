#include "common/geometry/icosphere.hpp"

IcosphereGeometry::~IcosphereGeometry() = default;

IcosphereGeometry::IcosphereGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_ICOSPHERE;
}

std::vector<Point3D> IcosphereGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> IcosphereGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> IcosphereGeometry::copyIndices() {
    throw 1;
}
