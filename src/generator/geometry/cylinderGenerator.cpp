// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/cylinder.hpp"
#include "common/geometry/point.hpp"

CylinderGeometry::~CylinderGeometry() = default;

CylinderGeometry::CylinderGeometry(int radius, int height, int slices) {
    this->_kind = GEOMETRY_CYLINDER;

    float alpha = (2 * M_PI) / slices;
    Point3D center(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < slices; i++) {
        float x1 = radius * cos(i * alpha);
        float z1 = radius * sin(i * alpha);
        float x2 = radius * cos((i + 1) * alpha);
        float z2 = radius * sin((i + 1) * alpha);

        // Desenhar triangulos base
        this->vertices.push_back(Point3D(x2, 0.0f, z2));
        this->vertices.push_back(center);
        this->vertices.push_back(Point3D(x1, 0.0f, z1));

        // Desenhar triangulos topo
        this->vertices.push_back(Point3D(x1, height, z1));
        this->vertices.push_back(Point3D(0.0f, height, 0.0f));
        this->vertices.push_back(Point3D(x2, height, z2));

        // Desenhar as laterais
        this->vertices.push_back(Point3D(x1, 0.0f, z1));
        this->vertices.push_back(Point3D(x1, height, z1));
        this->vertices.push_back(Point3D(x2, 0.0f, z2));
        //
        this->vertices.push_back(Point3D(x1, height, z1));
        this->vertices.push_back(Point3D(x2, height, z2));
        this->vertices.push_back(Point3D(x2, 0.0f, z2));
    }
}

std::vector<Point3D> CylinderGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> CylinderGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> CylinderGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}
