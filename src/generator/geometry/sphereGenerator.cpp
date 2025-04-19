// M_PI does not work in Visual Studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/point.hpp"
#include "common/geometry/sphere.hpp"

SphereGeometry::~SphereGeometry() = default;

SphereGeometry::SphereGeometry(float radius, int slices, int stacks) {
    this->_kind = GEOMETRY_SPHERE;

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sliceStep = 2 * M_PI / slices;
    float stackStep = M_PI / stacks;
    float sectorAngle, stackAngle;

    std::vector<std::vector<Point3D>> points;

    for (int i = 0; i <= stacks; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;  // de π/2 a -π/2
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        std::vector<Point3D> row;
        for (int j = 0; j <= slices; ++j) {
            sectorAngle = j * sliceStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            row.push_back(Point3D(x, y, z));
        }
        points.push_back(row);
    }

    // Criar os triângulos da esfera
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            Point3D p1 = points[i][j];
            Point3D p2 = points[i + 1][j];
            Point3D p3 = points[i][j + 1];
            Point3D p4 = points[i + 1][j + 1];

            // Criar dois triângulos para cada face da esfera
            this->vertices.push_back(p1);
            this->vertices.push_back(p2);
            this->vertices.push_back(p3);

            this->vertices.push_back(p3);
            this->vertices.push_back(p2);
            this->vertices.push_back(p4);
        }
    }
}

std::vector<Point3D> SphereGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> SphereGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> SphereGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}
