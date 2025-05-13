#include "common/geometry/cylinder.hpp"

CylinderGeometry::~CylinderGeometry() = default;

CylinderGeometry::CylinderGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<Vector2<float>> uvs, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;
    this->uvs = uvs;

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

std::vector<Vector2<float>> CylinderGeometry::copyUVs() {
    throw 1;
}