// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/cone.hpp"
#include "common/geometry/point.hpp"
#include "common/util/vectors.hpp"

ConeGeometry::~ConeGeometry() = default;

ConeGeometry::ConeGeometry(int radius, int height, int slices, int stacks) {
    this->_kind = GEOMETRY_CONE;

    float alpha = (2 * M_PI) / slices;
    float stackHeight = static_cast<float>(height) / stacks;
    Point3D baseCenter(0.0f, 0.0f, 0.0f);
    Point3D apex(0.0f, static_cast<float>(height), 0.0f);

    std::unordered_map<VertexKey, unsigned int> vertexMap;
    std::unordered_map<unsigned int, Vector3<float>> normalSums;
    std::unordered_map<unsigned int, int> normalCounts;

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& faceNormal, float u, float v) {
        VertexKey key{pos};
        unsigned int index;

        auto it = vertexMap.find(key);
        if (it != vertexMap.end()) {
            index = it->second;
        } else {
            index = static_cast<unsigned int>(this->vertices.size());
            vertexMap[key] = index;
            this->vertices.push_back(pos);
            this->normals.emplace_back(0.0f, 0.0f, 0.0f);  // placeholder
            this->uvs.emplace_back(u, v);
        }

        normalSums[index] += faceNormal;
        normalCounts[index]++;
        return index;
    };

    // --- Base ---
    Vector3<float> down(0.0f, -1.0f, 0.0f);
    for (int i = 0; i < slices; i++) {
        float angle1 = i * alpha;
        float angle2 = (i + 1) * alpha;

        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);

        Point3D p1(x1, 0.0f, z1);
        Point3D p2(x2, 0.0f, z2);

        // UVs for base using polar mapping
        auto uv = [](float x, float z, float r) {
            return Vector2<float>(0.5f + x / (2.0f * r), 0.5f + z / (2.0f * r));
        };

        unsigned int i1 = addVertex(p2, down, uv(p2.getX(), p2.getZ(), radius).first, uv(p2.getX(), p2.getZ(), radius).second);
        unsigned int i2 = addVertex(baseCenter, down, 0.5f, 0.5f);
        unsigned int i3 = addVertex(p1, down, uv(p1.getX(), p1.getZ(), radius).first, uv(p1.getX(), p1.getZ(), radius).second);

        this->indices.insert(this->indices.end(), {i1, i2, i3});
    }

    // --- Side ---
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            float t1 = static_cast<float>(j) / stacks;
            float t2 = static_cast<float>(j + 1) / stacks;

            float y1 = t1 * height;
            float y2 = t2 * height;

            float scale1 = 1.0f - t1;
            float scale2 = 1.0f - t2;

            float angle1 = i * alpha;
            float angle2 = (i + 1) * alpha;

            float x1 = radius * cos(angle1);
            float z1 = radius * sin(angle1);
            float x2 = radius * cos(angle2);
            float z2 = radius * sin(angle2);

            Point3D p1(x1 * scale2, y2, z1 * scale2);
            Point3D p2(x1 * scale1, y1, z1 * scale1);
            Point3D p3(x2 * scale2, y2, z2 * scale2);
            Point3D p4(x2 * scale1, y1, z2 * scale1);

            // Normals for two triangles
            Vector3<float> n1 = (p3.toVector3() - p1.toVector3()).cross(p2.toVector3() - p1.toVector3()).normalized();
            Vector3<float> n2 = (p3.toVector3() - p2.toVector3()).cross(p4.toVector3() - p2.toVector3()).normalized();

            float u1 = static_cast<float>(i) / slices;
            float u2 = static_cast<float>(i + 1) / slices;
            float v1 = 1.0f - t1;
            float v2 = 1.0f - t2;

            unsigned int i1 = addVertex(p1, n1, u1, v2);
            unsigned int i2 = addVertex(p2, n1, u1, v1);
            unsigned int i3 = addVertex(p3, n1, u2, v2);

            unsigned int i4 = addVertex(p3, n2, u2, v2);
            unsigned int i5 = addVertex(p2, n2, u1, v1);
            unsigned int i6 = addVertex(p4, n2, u2, v1);

            this->indices.insert(this->indices.end(), {i1, i3, i2});
            this->indices.insert(this->indices.end(), {i4, i6, i5});
        }
    }

    // --- Normalize normals ---
    for (size_t i = 0; i < this->normals.size(); ++i) {
        if (normalCounts[i] > 0) {
            Vector3<float> avg = normalSums[i] * (1.0f / static_cast<float>(normalCounts[i]));
            this->normals[i] = avg.normalized();
        }
    }
}

std::vector<Point3D> ConeGeometry::copyVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> ConeGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> ConeGeometry::copyIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}