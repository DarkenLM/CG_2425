#include "common/geometry/cylinder.hpp"

#include "common/parser.hpp"

CylinderGeometry::~CylinderGeometry() = default;

CylinderGeometry::CylinderGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_CYLINDER;
}

std::vector<Point3D> CylinderGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> CylinderGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> CylinderGeometry::copyIndices() {
    throw 1;
}
