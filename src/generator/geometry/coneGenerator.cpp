// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/cone.hpp"
#include "common/geometry/point.hpp"

ConeGeometry::~ConeGeometry() = default;

ConeGeometry::ConeGeometry(int radius, int height, int slices, int stacks) {
    this->_kind = GEOMETRY_CONE;
    float alpha = (2 * M_PI) / slices;
    float stackHeight = static_cast<float>(height) / stacks;
    int nTriangles = 0;

    Point3D center(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < slices; i++) {
        float x1 = radius * cos(i * alpha);
        float z1 = radius * sin(i * alpha);
        float x2 = radius * cos((i + 1) * alpha);
        float z2 = radius * sin((i + 1) * alpha);

        this->vertices.push_back(center);
        this->vertices.push_back(Point3D(x1, 0.0f, z1));
        this->vertices.push_back(Point3D(x2, 0.0f, z2));

        for (int j = 0; j < stacks; j++) {
            float y1 = j * stackHeight;
            float y2 = (j + 1) * stackHeight;

            x1 = radius * cos(i * alpha) * (1 - (float)(j + 1) / stacks);
            z1 = radius * sin(i * alpha) * (1 - (float)(j + 1) / stacks);
            x2 = radius * cos((i + 1) * alpha) * (1 - (float)(j + 1) / stacks);
            z2 = radius * sin((i + 1) * alpha) * (1 - (float)(j + 1) / stacks);

            float x3 = radius * cos(i * alpha) * (1 - (float)j / stacks);
            float z3 = radius * sin(i * alpha) * (1 - (float)j / stacks);
            float x4 = radius * cos((i + 1) * alpha) * (1 - (float)j / stacks);
            float z4 = radius * sin((i + 1) * alpha) * (1 - (float)j / stacks);

            this->vertices.push_back(Point3D(x2, y2, z2));
            this->vertices.push_back(Point3D(x3, y1, z3));
            this->vertices.push_back(Point3D(x1, y2, z1));

            this->vertices.push_back(Point3D(x2, y2, z2));
            this->vertices.push_back(Point3D(x4, y1, z4));
            this->vertices.push_back(Point3D(x3, y1, z3));
        }
    }
}

std::vector<Point3D> ConeGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> ConeGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> ConeGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}