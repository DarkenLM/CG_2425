#include "common/geometry/obj.hpp"

ObjGeometry::~ObjGeometry() = default;

ObjGeometry::ObjGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<Vector2<float>> uvs, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;
    this->uvs = uvs;

    this->_kind = GEOMETRY_OBJ;
}

std::vector<Point3D> ObjGeometry::copyVertices() {
    throw 1;
}

std::vector<Vector3<float>> ObjGeometry::copyNormals() {
    throw 1;
}

std::vector<unsigned int> ObjGeometry::copyIndices() {
    throw 1;
}

std::vector<Vector2<float>> ObjGeometry::copyUVs() {
    throw 1;
}