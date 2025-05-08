#include "common/geometry/sphere.hpp"

SphereGeometry::~SphereGeometry() = default;

SphereGeometry::SphereGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;

    this->_kind = GEOMETRY_SPHERE;
}

std::vector<Point3D> SphereGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> SphereGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> SphereGeometry::copyIndices() {
    throw 1;
}
