#define _USE_MATH_DEFINES

#include <cmath>
#include <unordered_map>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/point.hpp"
#include "common/geometry/torus.hpp"

// Helper to compute torus points
Point3D torusPoint(float R, float r, float beta, float alpha) {
    float x = (R + r * cos(alpha)) * cos(beta);
    float y = r * sin(alpha);
    float z = (R + r * cos(alpha)) * sin(beta);
    return Point3D(x, y, z);
}

TorusGeometry::~TorusGeometry() = default;

TorusGeometry::TorusGeometry(int majorRadius, int minorRadius, int slices, int sides) {
    this->_kind = GEOMETRY_TORUS;

    float sliceStep = 2 * M_PI / slices;
    float sideStep = 2 * M_PI / sides;

    struct TempVertex {
        Point3D position;
        Vector3<float> normalSum;
        Vector2<float> uv;
    };

    std::vector<std::vector<unsigned int>> indexGrid;
    std::vector<TempVertex> tempVertices;

    for (int i = 0; i <= slices; ++i) {
        float beta = i * sliceStep;
        std::vector<unsigned int> row;

        for (int j = 0; j <= sides; ++j) {
            float alpha = j * sideStep;

            Point3D pos = torusPoint(majorRadius, minorRadius, beta, alpha);

            Point3D center(majorRadius * cos(beta), 0, majorRadius * sin(beta));
            Vector3<float> normal = Vector3<float>(
                                        pos.getX() - center.getX(),
                                        pos.getY() - center.getY(),
                                        pos.getZ() - center.getZ())
                                        .normalized();

            float u = static_cast<float>(i) / slices;
            float v = static_cast<float>(j) / sides;

            TempVertex tv;
            tv.position = pos;
            tv.normalSum = Vector3<float>(0.0f, 0.0f, 0.0f);  // será somado depois
            tv.uv = Vector2<float>(u, v);

            row.push_back(static_cast<unsigned int>(tempVertices.size()));
            tempVertices.push_back(tv);
        }

        indexGrid.push_back(row);
    }

    for (int i = 0; i < slices; ++i) {
        for (int j = 0; j < sides; ++j) {
            unsigned int i1 = indexGrid[i][j];
            unsigned int i2 = indexGrid[i + 1][j];
            unsigned int i3 = indexGrid[i][j + 1];
            unsigned int i4 = indexGrid[i + 1][j + 1];

            const Point3D& p1 = tempVertices[i1].position;
            const Point3D& p2 = tempVertices[i2].position;
            const Point3D& p3 = tempVertices[i3].position;
            const Point3D& p4 = tempVertices[i4].position;

            Vector3<float> v1(p1.getX(), p1.getY(), p1.getZ());
            Vector3<float> v2(p2.getX(), p2.getY(), p2.getZ());
            Vector3<float> v3(p3.getX(), p3.getY(), p3.getZ());
            Vector3<float> v4(p4.getX(), p4.getY(), p4.getZ());

            // Triângulo 1
            Vector3<float> n1 = (v3 - v1).cross(v2 - v1).normalized();
            tempVertices[i1].normalSum += n1;
            tempVertices[i3].normalSum += n1;
            tempVertices[i2].normalSum += n1;
            this->indices.insert(this->indices.end(), {i1, i3, i2});

            // Triângulo 2
            Vector3<float> n2 = (v4 - v3).cross(v2 - v3).normalized();
            tempVertices[i3].normalSum += n2;
            tempVertices[i4].normalSum += n2;
            tempVertices[i2].normalSum += n2;
            this->indices.insert(this->indices.end(), {i3, i4, i2});
        }
    }

    for (const auto& tv : tempVertices) {
        this->vertices.push_back(tv.position);
        this->normals.push_back(tv.normalSum.normalized());
        this->uvs.push_back(tv.uv);
    }
}

std::vector<Point3D> TorusGeometry::copyVertices() {
    std::vector<Point3D> ret;
    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }
    return ret;
}

std::vector<Vector3<float>> TorusGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;
    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }
    return ret;
}

std::vector<Vector2<float>> TorusGeometry::copyUVs() {
    std::vector<Vector2<float>> ret;
    for (auto uv : this->uvs) {
        ret.push_back(uv.copy());
    }
    return ret;
}

std::vector<unsigned int> TorusGeometry::copyIndices() {
    std::vector<unsigned> ret;
    for (auto i : this->indices) {
        ret.push_back(i);
    }
    return ret;
}
