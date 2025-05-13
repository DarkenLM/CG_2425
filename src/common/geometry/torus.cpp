#include "common/geometry/torus.hpp"

TorusGeometry::~TorusGeometry() = default;

TorusGeometry::TorusGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<Vector2<float>> uvs, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;
    this->uvs = uvs;

    this->_kind = GEOMETRY_TORUS;
}

std::vector<Point3D> TorusGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> TorusGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> TorusGeometry::copyIndices() {
    throw 1;
}

std::vector<Vector2<float>> TorusGeometry::copyUVs() {
    throw 1;
}
