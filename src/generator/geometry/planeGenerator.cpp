#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/plane.hpp"
#include "common/geometry/point.hpp"
#include "common/util/vectors.hpp"

PlaneGeometry::~PlaneGeometry() = default;

PlaneGeometry::PlaneGeometry(int length, int gridSize) {
    this->_kind = GEOMETRY_PLANE;

    float flen = (float)length;
    float subDivUnit = flen / gridSize;

    int numVerts = (gridSize + 1);

    this->vertices.reserve(numVerts * numVerts);
    this->normals.resize(numVerts * numVerts, Vector3<float>(0.0f, 0.0f, 0.0f));
    this->uvs.reserve(numVerts * numVerts);

    for (int i = 0; i <= gridSize; ++i) {
        for (int j = 0; j <= gridSize; ++j) {
            float x = -flen / 2 + subDivUnit * j;
            float z = -flen / 2 + subDivUnit * i;

            this->vertices.push_back(Point3D(x, 0.0f, z));

            float u = (float)j / gridSize;
            float v = (float)i / gridSize;
            this->uvs.push_back(Vector2<float>(u, v));
        }
    }

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            int topLeft = i * (gridSize + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (gridSize + 1) + j;
            int bottomRight = bottomLeft + 1;

            // Triângulo 1
            this->indices.insert(this->indices.end(), {(unsigned int)topLeft,
                                                       (unsigned int)bottomLeft,
                                                       (unsigned int)topRight});

            {
                Point3D p0 = this->vertices[topLeft];
                Point3D p1 = this->vertices[bottomLeft];
                Point3D p2 = this->vertices[topRight];

                Vector3<float> v0(p0.getX(), p0.getY(), p0.getZ());
                Vector3<float> v1(p1.getX(), p1.getY(), p1.getZ());
                Vector3<float> v2(p2.getX(), p2.getY(), p2.getZ());

                Vector3<float> normal = (v1 - v0).cross(v2 - v0).normalized();
                this->normals[topLeft] += normal;
                this->normals[bottomLeft] += normal;
                this->normals[topRight] += normal;
            }

            // Triângulo 2
            this->indices.insert(this->indices.end(), {(unsigned int)topRight,
                                                       (unsigned int)bottomLeft,
                                                       (unsigned int)bottomRight});

            {
                Point3D p0 = this->vertices[topRight];
                Point3D p1 = this->vertices[bottomLeft];
                Point3D p2 = this->vertices[bottomRight];

                Vector3<float> v0(p0.getX(), p0.getY(), p0.getZ());
                Vector3<float> v1(p1.getX(), p1.getY(), p1.getZ());
                Vector3<float> v2(p2.getX(), p2.getY(), p2.getZ());

                Vector3<float> normal = (v1 - v0).cross(v2 - v0).normalized();
                this->normals[topRight] += normal;
                this->normals[bottomLeft] += normal;
                this->normals[bottomRight] += normal;
            }
        }
    }

    for (size_t i = 0; i < this->normals.size(); ++i) {
        this->normals[i] = this->normals[i].normalized();
    }
}

std::vector<Point3D> PlaneGeometry::copyVertices() {
    std::vector<Point3D> ret;

    for (size_t i = 0; i < this->vertices.size(); ++i) {
        ret.push_back(this->vertices[i].copy());
    }

    return ret;
}

std::vector<Vector3<float>> PlaneGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;

    for (size_t i = 0; i < this->normals.size(); ++i) {
        ret.push_back(this->normals[i].copy());
    }

    return ret;
}

std::vector<unsigned int> PlaneGeometry::copyIndices() {
    std::vector<unsigned int> ret;

    for (size_t i = 0; i < this->indices.size(); ++i) {
        ret.push_back(this->indices[i]);
    }

    return ret;
}
