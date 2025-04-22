// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/plane.hpp"
#include "common/geometry/point.hpp"
#include "engine/glContext.hpp"

PlaneGeometry::~PlaneGeometry() = default;

PlaneGeometry::PlaneGeometry(int length, int gridSize) {
    this->_kind = GEOMETRY_PLANE;

    float flen = (float)length;
    float subDivUnit = length * 1.0f / gridSize;

    // Vertices and normal calculation
    for (int i = 0; i <= gridSize; ++i) {
        for (int j = 0; j <= gridSize; ++j) {
            float x = -flen / 2 + subDivUnit * j;
            float z = -flen / 2 + subDivUnit * i;

            this->vertices.push_back(Point3D(x, 0.0f, z));
            this->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        }
    }

    // Indices calculation
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            int topLeft = i * (gridSize + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (gridSize + 1) + j;
            int bottomRight = bottomLeft + 1;

            // Triangle 1
            this->indices.push_back(topLeft);
            this->indices.push_back(bottomLeft);
            this->indices.push_back(topRight);

            // Triangle 2
            this->indices.push_back(topRight);
            this->indices.push_back(bottomLeft);
            this->indices.push_back(bottomRight);
        }
    }
}

std::vector<Point3D> PlaneGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> PlaneGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> PlaneGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}