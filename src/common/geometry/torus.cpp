#include "common/geometry/torus.hpp"

#include "common/parser.hpp"

TorusGeometry::~TorusGeometry() = default;

TorusGeometry::TorusGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

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
