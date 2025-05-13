#define _USE_MATH_DEFINES

#include <cmath>
#include <unordered_map>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/point.hpp"
#include "common/geometry/sphere.hpp"

SphereGeometry::~SphereGeometry() = default;

SphereGeometry::SphereGeometry(float radius, int slices, int stacks) {
    this->_kind = GEOMETRY_SPHERE;

    std::unordered_map<VertexKey, unsigned int> vertexMap;

    struct TempVertex {
        Point3D position;
        Vector3<float> normalSum;
        Vector2<float> uv;
    };

    std::vector<TempVertex> tempVertices;

    float sliceStep = 2.0f * M_PI / slices;
    float stackStep = M_PI / stacks;

    std::vector<std::vector<unsigned int>> indexGrid;

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float y = radius * sinf(stackAngle);

        std::vector<unsigned int> row;

        for (int j = 0; j <= slices; ++j) {
            float sectorAngle = j * sliceStep;

            float x = xy * cosf(sectorAngle);
            float z = xy * sinf(sectorAngle);

            Point3D pos(x, y, z);
            Vector3<float> normal = Vector3<float>(x, y, z).normalized();
            float u = static_cast<float>(j) / slices;
            float v = static_cast<float>(i) / stacks;

            VertexKey key{pos};
            auto it = vertexMap.find(key);
            if (it != vertexMap.end()) {
                row.push_back(it->second);
            } else {
                unsigned int idx = static_cast<unsigned int>(tempVertices.size());
                vertexMap[key] = idx;
                row.push_back(idx);

                TempVertex tv;
                tv.position = pos;
                tv.normalSum = Vector3<float>(0.0f, 0.0f, 0.0f);  // to be summed later
                tv.uv = Vector2<float>(u, v);
                tempVertices.push_back(tv);
            }
        }

        indexGrid.push_back(row);
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            unsigned int i1 = indexGrid[i][j];
            unsigned int i2 = indexGrid[i][j + 1];
            unsigned int i3 = indexGrid[i + 1][j];
            unsigned int i4 = indexGrid[i + 1][j + 1];

            const Point3D& p1 = tempVertices[i1].position;
            const Point3D& p2 = tempVertices[i2].position;
            const Point3D& p3 = tempVertices[i3].position;
            const Point3D& p4 = tempVertices[i4].position;

            Vector3<float> v1(p1.getX(), p1.getY(), p1.getZ());
            Vector3<float> v2(p2.getX(), p2.getY(), p2.getZ());
            Vector3<float> v3(p3.getX(), p3.getY(), p3.getZ());
            Vector3<float> v4(p4.getX(), p4.getY(), p4.getZ());

            // Triangle 1
            Vector3<float> n1 = (v2 - v1).cross(v3 - v1).normalized();
            tempVertices[i1].normalSum += n1;
            tempVertices[i2].normalSum += n1;
            tempVertices[i3].normalSum += n1;
            this->indices.insert(this->indices.end(), {i1, i2, i3});

            // Triangle 2
            Vector3<float> n2 = (v4 - v2).cross(v3 - v2).normalized();
            tempVertices[i2].normalSum += n2;
            tempVertices[i4].normalSum += n2;
            tempVertices[i3].normalSum += n2;
            this->indices.insert(this->indices.end(), {i2, i4, i3});
        }
    }

    // Final assignment
    for (const auto& tv : tempVertices) {
        this->vertices.push_back(tv.position);
        this->normals.push_back(tv.normalSum.normalized());
        this->uvs.push_back(tv.uv);
    }
}

std::vector<Point3D> SphereGeometry::copyVertices() {
    std::vector<Point3D> ret;

    for (size_t i = 0; i < this->vertices.size(); ++i) {
        ret.push_back(this->vertices[i].copy());
    }

    return ret;
}

std::vector<Vector3<float>> SphereGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;

    for (size_t i = 0; i < this->normals.size(); ++i) {
        ret.push_back(this->normals[i].copy());
    }

    return ret;
}

std::vector<unsigned int> SphereGeometry::copyIndices() {
    std::vector<unsigned int> ret;

    for (size_t i = 0; i < this->indices.size(); ++i) {
        ret.push_back(this->indices[i]);
    }

    return ret;
}
